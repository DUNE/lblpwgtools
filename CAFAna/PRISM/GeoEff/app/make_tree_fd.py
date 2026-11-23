#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#from timeit import default_timer as time
#start=time()
from uproot import concatenate, exceptions
from ROOT import std, TFile, TTree
import numpy as np
from glob import glob
from sys import argv
import sys

import torch
try:
    from .muon_eff_model import muonEffModel
except ImportError:
    from muon_eff_model import muonEffModel
from os.path import splitext, basename#, exists
from pathlib import Path
from scipy.spatial.transform import Rotation as R
from scipy.interpolate import interp1d
#from ROOT import TGraph
from array import array
#The code is currently quite slow, so it uses multiprocessing to speed things up
from multiprocessing import Pool

# SET NUMBER OF PROCESSORS HERE
NUM_PROCS=30
# ND coordinate offset.
offset=[0.,5.5,411.]
# Average neutrino decay position in beam coordinates as a function of vertex x (from Luke):
# Will be used to set the decay position event-by-event.
OffAxisPoints = array('f', [-30.5,   -28,    -25.5,   -23,     -20.5,   -18,     -15.5,   -13,    -10.5,   -8,      -5.5,    -3,    -0.5,     0,       0.5,    3,     5.5,    8,      10.5,   13,     15.5,   18,     20.5,  23,     25.5,   28,     30.5])
meanPDPZ = array('f', [ 57.7352, 59.1433, 60.8336, 62.4821, 65.005, 67.5822, 69.8348, 73.0865, 76.6664, 81.1443, 85.6266, 90.346, 93.362, 93.6072, 93.362,  90.346, 85.6266, 81.1443, 76.6664, 73.0865, 69.8348, 67.5822, 65.005, 62.4821, 60.8336, 59.1433, 57.7352]) #why are these discreet data sets?
#gDecayZ = TGraph(14, OffAxisPoints, meanPDPZ)
gDecayZ=interp1d(OffAxisPoints,meanPDPZ,fill_value='extrapolate')

# These are used to translate between the near detector coordinate system and the neutrino beamline coordinate system. We use this to calculate the average neutrino direction,
# assuming the mean neutrino production point as a function of neutrino interaction x, which is given in the arrays above.
beamRefDetCoord=[0.0, 0.05387, 6.66] #spherical coords, radians
detRefBeamCoord=[0, 0, 562.1179] #xyz coords of reference detector position
beamLineRotation=-0.101

# Fiducial volume definition
def isFV(x, y, z):
    inDeadRegion=False
    for i in [-3, -2, -1, 0, 1, 2, 3] :
        cathode_center=i*102.1
        if (x>cathode_center-0.75) and (x<cathode_center+0.75):inDeadRegion=True
        module_boundary=i*102.1+51.05
        if (i<=2) and (x>module_boundary-1.3) and (x<module_boundary+1.3):inDeadRegion=True
    for i in [1, 2, 3, 4]:
        module_boundary=i*101.8-0.6
        if (z>module_boundary-1.7) and (z<module_boundary+1.7):inDeadRegion=True
    return (abs(x)<300) and (abs(y)<100) and (z>50) and (z<350) and (not inDeadRegion)

# Vectorize fiducial volume function
isFV_vec = np.vectorize(isFV)

# Simple muon containment cut
def isContained(x, y, z) :
    if abs(x)>350:return False
    if abs(y)>150:return False
    if z<0 or z>500:return False
    return True

FV_cut=True
LAr_position=[-3000.]
# LAr_position=[-2800.,-1400.,0.]
# LAr_position=[-2800.,-2575.,-2400.,-2175.,-2000.,-1775.,-1600.,-1375.,-1200.,-975.,-800.,-575.,-400.,-175.,0.]
# vertex_position=[-299.,-292.,-285.,-278.,-271.,-264.,-216.,-168.,-120.,-72.,-24.,24.,72.,120.,168.,216.,264.,271.,278.,285.,292.,299.]
TreeVars=["ND_OffAxis_Sim_mu_start_v_xyz_LAr", "ND_OffAxis_Sim_mu_start_p_xyz_LAr", "hadron_throw_result_LAr","ND_LepNuAngle","ND_Gen_numu_E","ND_E_vis_true"]

# Read FD CAF
CAF_files = None
net = None

#This is the function where everything happens. Analyse one file at a time, otherwise memory explodes! As f is only 1 file, each file get assigned to a different cpu.
def processFiles(f):
    #output="/storage/shared/barwu/FDCAFIntegration4GEC_wei/"+splitext(basename(f))[0]+"_Eff.root"
    #output="/storage/shared/barwu/10thTry/FDEff/"+splitext(basename(f))[0]+"_Eff.root"
    output="Output_FDGeoEff.root"
    # if exists(output)==True:
    #      print("file already exists")
    #      return None
    try:
        # Get CAF TTree
        # GeoEffThrows=concatenate("{0}:GeoEffThrows".format(f), TreeVars, library="np")
        # cafTree=concatenate("{0}:cafTree;1".format(f), ["LepNuAngle"], library="np")
        FD_sim_Results=concatenate("{0}:effTreeND".format(f), TreeVars, library="np")
        throwsFD=concatenate("{0}:ThrowsFD".format(f), ['throwVtxY', 'throwVtxZ', 'throwRot'], library = "np")
    #leave except condition specification so that code crashes when there is another exception condition
    except exceptions.KeyInFileError as err:
        print("Couldn't find caf TTree in file {0} for {1}. Skipping.".format(f, err))
        return None
        #continue

    effs=std.vector('double')()
    effs_tracker=std.vector('double')()
    effs_contained=std.vector('double')()
    effs_selected=std.vector('double')()
    effs_combined=std.vector('double')()
    vtxX=std.vector('double')()
    nnContained_values =std.vector(std.vector('double'))()  # to store nnContained values for each event, detector, and vertex
    weightPmuon = std.vector(std.vector('double'))()  # to store Muon Prob values for each event, detector, and vertex for each passing throw
    tf=TFile.Open(output,"RECREATE")
    tree=TTree("event_data", "event_data")
    tree.Branch("hadron_selected_eff", effs)
    tree.Branch("muon_tracker_eff", effs_tracker)
    tree.Branch("muon_contained_eff", effs_contained)
    tree.Branch("muon_selected_eff", effs_selected)
    tree.Branch("combined_eff", effs_combined)
    tree.Branch("vtxX", vtxX)
    tree.Branch("muContained", nnContained_values)
    tree.Branch("weightPmuon", weightPmuon)

    # Add branch from FD CAF
    # # ND_OffAxis_Sim_mu_start_v_xyz_LAr = []
    # ND_OffAxis_Sim_mu_start_p_xyz_LAr = []
    # # # tree.Branch("ND_OffAxis_Sim_mu_start_v_xyz_LAr", ND_OffAxis_Sim_mu_start_v_xyz_LAr)
    # tree.Branch("ND_OffAxis_Sim_mu_start_p_xyz_LAr", ND_OffAxis_Sim_mu_start_p_xyz_LAr)
    # tree.Branch("ND_LepNuAngle", ND_LepNuAngle_a)

    # tree.Branch("ND_Gen_numu_E", ND_Gen_numu_E_a)
    # tree.Branch("ND_E_vis_true", ND_E_vis_true_a)
    # Event loop
    print ("len(FD_sim_Results['hadron_throw_result_LAr']) ", len(FD_sim_Results['hadron_throw_result_LAr']))
    for i_event in range(len(FD_sim_Results['hadron_throw_result_LAr'])):
        #if i_event!=1:
        #    continue
        print(" length: ", len(FD_sim_Results['hadron_throw_result_LAr']))
        event=FD_sim_Results['hadron_throw_result_LAr'][i_event]
        print("event", event)
        # num_0_effs=0
        # num_high_effs=0
        # #print("i_event=",end="")
        #print(i_event)
        #if i_event==10: break #use when debugging
        effs.clear() #remove past events' ND array efficiency data
        effs_tracker.clear()
        effs_contained.clear()
        effs_selected.clear()
        effs_combined.clear()
        vtxX.clear()
        nnContained_values.clear()
        weightPmuon.clear()
        # ND_OffAxis_Sim_mu_start_v_xyz_LAr.clear()
        # ND_OffAxis_Sim_mu_start_p_xyz_LAr.clear()
        # # ND_OffAxis_Sim_mu_start_v_xyz_LAr = FD_sim_Results["ND_OffAxis_Sim_mu_start_v_xyz_LAr"]
        # ND_OffAxis_Sim_mu_start_p_xyz_LAr = FD_sim_Results["ND_OffAxis_Sim_mu_start_p_xyz_LAr"]
        # # ND_LepNuAngle_a = FD_sim_Results["ND_LepNuAngle"]
        # ND_Gen_numu_E_a = FD_sim_Results["ND_Gen_numu_E"]
        # ND_E_vis_true_a = FD_sim_Results["ND_E_vis_true"]
        #print(cafTree['LepNuAngle'][i_event])
        print("len event: ",len(event))
        #for now only 1 det pos
        for det_pos in range(len(event)):
        # for det_pos in [0,14]: #14 is the last LAr position index
            # print("LAr pos=",end="")
            # print(effValues['ND_LAr_dtctr_pos'][det_pos])

            # Only pick on-axis
            # if (det_pos != 2):
            #     continue
            # print("det_pos=",end="")
            # print(det_pos)
            # print("LAr_position[det_pos]", end="")
            # print(LAr_position[det_pos])

            # effs.push_back(std.vector('double')())
            # effs_tracker.push_back(std.vector('double')())
            # effs_contained.push_back(std.vector('double')())
            # effs_selected.push_back(std.vector('double')())
            # effs_combined.push_back(std.vector('double')())
            #vtxX.push_back(('double')())
            nnContained_values.push_back(std.vector('double')())
            weightPmuon.push_back(std.vector('double')())
            print(" len(event[det_pos])", len(event[det_pos]), "event[det_pos]", event[det_pos])
            #loop over vtxX positions
            for vtx_pos in range(len(event[det_pos])):
            #for vtx_pos in [0,21]: #21 is the last x-position index
            #     print("vertex pos=",end="")
            #     print(effValues['ND_LAr_vtx_pos'][vtx_pos])
                #if vtx_pos != 25:
                #    continue

                # Accumulators for efficiency calculation
                thisEff=0. # Hadronic efficiency
                thisEff_tracker=0. # Tracker-match efficiency
                thisEff_contained=0. # Contained muon efficiency
                thisEff_combined=0. # Combined efficiency

                
                this_p_init=FD_sim_Results["ND_OffAxis_Sim_mu_start_p_xyz_LAr"][i_event][det_pos][vtx_pos] #this should be in the throws loop
                this_vtx_x = FD_sim_Results["ND_OffAxis_Sim_mu_start_v_xyz_LAr"][i_event][det_pos][vtx_pos][0]
                this_vtx_x_shifted=this_vtx_x-LAr_position[det_pos]
                this_vtx_y=FD_sim_Results["ND_OffAxis_Sim_mu_start_v_xyz_LAr"][i_event][det_pos][vtx_pos][1]
                this_vtx_z=FD_sim_Results["ND_OffAxis_Sim_mu_start_v_xyz_LAr"][i_event][det_pos][vtx_pos][2]
                #print(f'Initial Muon Momentum (this_p_init): {this_p_init}')
                #print(f'Initial Vertex Position (this_vtx_x, this_vtx_y, this_vtx_z): ({this_vtx_x}, {this_vtx_y}, {this_vtx_z})')

                #Check which throws are in the FV. throws_FV is a boolean array with one element per throw. #make sure that len(throwsFD["throwRot"][0])=4096
                throws_FV=isFV_vec([this_vtx_x_shifted]*len(throwsFD["throwRot"][0]),throwsFD["throwVtxY"][0]-offset[1],throwsFD["throwVtxZ"][0]-offset[2])

                # print(this_vtx_x)
                # print(throwsFD["throwVtxY"][0]-offset[1])
                # print(throwsFD["throwVtxZ"][0]-offset[2])
                # print(throws_FV)
                if FV_cut: NthrowsInFV=sum(throws_FV) #Count how many throws were in the FV. Will be useful later.
                else: NthrowsInFV=4096 #If you don't use the FV, then all throws should be included. #There are 64*64=4096 throws in total.
                if NthrowsInFV==0 and FV_cut:
                    effs.push_back(-1.)
                    effs_tracker.push_back(-1.)
                    effs_contained.push_back(-1.)
                    effs_combined.push_back(-1.)
                    continue

                # if (det_pos==14): print("throw #; y pos (cm); z pos (cm); throw rotation angle (rad); contained eff; tracker eff; z-momentum (GeV/c)")
                #throw_number=1
                # Loop through the hadronic geometric efficiency throw results. Each bitfield corresponds to 64 throws.
                # Hadronic veto bits are grouped into 64, to prevent processing overload for the neural network.

                for i_bitfield, bitfield in enumerate(event[det_pos][vtx_pos][0][0]):
                    bitfield=np.array([bitfield], dtype=np.uint64) #Converts the 64 bit integer into "bit" array
                    bitfield=np.unpackbits(np.array(bitfield, dtype='>i8').view(np.uint8)) #unpackbits converts a 256-basevalue into an array of binary integers
                    bitfieldTemp=np.copy(bitfield)
                    # Annoyingly, the array is backwards. Invert array order...
                    for j_bitfield in range(len(bitfield)): bitfield[-(1+j_bitfield)]=bitfieldTemp[j_bitfield]
                    # Calculate hadron efficiency. Sum the number of throws where the hadronic system was contained
                    # (bit in bitfield is 1) and the throw was in the fiducial volume.
                    if FV_cut: thisEff+=np.sum(np.logical_and(bitfield, throws_FV[i_bitfield*64:(i_bitfield+1)*64]))
                    else: thisEff+=np.sum(bitfield)
                    
                    this_p=this_p_init
                    
                    #if i_bitfield == 1: 
                    #    print(f'Second set of throws')
                    #    print(f'	this_p[0] = {this_p[0]}')
                    
                    # print("Y:",end=" ")
                    # print(throwsFD["throwVtxY"][0][i_bitfield*64:(i_bitfield+1)*64])
                    # print("Z:",end=" ")
                    # print(throwsFD["throwVtxZ"][0][i_bitfield*64:(i_bitfield+1)*64])
                    # print("Rotation:",end=" ")
                    # print(throwsFD["throwRot"][0][i_bitfield*64:(i_bitfield+1)*64])
                    # Get variables needed to evaluate muon neural network for each throw.
                    # x is not randomized. This is a convoluted way of repeating vtx_x the correct number of times
                    throw_x=[this_vtx_x]*len(throwsFD["throwVtxZ"][0][i_bitfield*64:(i_bitfield+1)*64])
                    throw_x_shifted = [this_vtx_x_shifted]*len(throwsFD["throwVtxZ"][0][i_bitfield*64:(i_bitfield+1)*64])
                    # Get y for each random throw.
                    throw_y=throwsFD["throwVtxY"][0][i_bitfield*64:(i_bitfield+1)*64]-offset[1]
                    # Get z for each random throw
                    throw_z=throwsFD["throwVtxZ"][0][i_bitfield*64:(i_bitfield+1)*64]-offset[2]
                    # Get phi for each random throw
                    throw_phi=throwsFD["throwRot"][0][i_bitfield*64:(i_bitfield+1)*64]

                    # Use vertex to determine mean decay point
                    # Get z-coordinate of neutrino production point *in beamline coordinates*
                    decayZbeamCoord=gDecayZ((this_vtx_x/100-detRefBeamCoord[0])*100) # in cm
                    # Convert neutrino production point to *near detector coordinates*
                    decayZdetCoord=(-1*detRefBeamCoord[2]*100+decayZbeamCoord)*np.cos(beamLineRotation)-(-1*detRefBeamCoord[1]*100*np.sin(beamLineRotation))+\
                        beamRefDetCoord[2]*100
                    decayYdetCoord=(-1*detRefBeamCoord[1]*100*np.cos(beamLineRotation))+(-1*detRefBeamCoord[2]*100+decayZbeamCoord)*np.sin(beamLineRotation)+\
                        beamRefDetCoord[1]*100
                    decayXdetCoord=-1*detRefBeamCoord[0]*100+beamRefDetCoord[0]*100
                    # Vector from neutrino production point to original event vertex
                    decayToVertex=[this_vtx_x-decayXdetCoord,this_vtx_y-decayYdetCoord,this_vtx_z-decayZdetCoord]
                    # Vector from neutrino production point to randomly thrown vertex.
                    decayToTranslated=[[throw_x[i]-decayXdetCoord, throw_y[i]-decayYdetCoord, throw_z[i]-decayZdetCoord] for i in range(len(throw_x))]
                    
                    #if i_bitfield == 0:
                    #    print(f'First set of throws')
                    #    print(f'	throw position 0 ({throw_x[0]}, {throw_y[0]}, {throw_z[0]})')
                    #    print(f'	decayToVertex = {decayToVertex}')
                    #    print(f'	decayToTranslated[0] = {decayToTranslated[0]}')	
                    #    print(f' 	decayToTranslated[0] - decayToVertex = {np.subtract(decayToTranslated[0],decayToVertex)}')

                    magDecayToVertex=np.sqrt(np.sum(np.square(decayToVertex)))
                    magDecayToTranslated=np.sqrt(np.sum(np.square(decayToTranslated), axis=1))
                    translationAngle=np.dot(decayToTranslated, decayToVertex)
                    translationAngle=np.divide(translationAngle, np.multiply(magDecayToVertex,magDecayToTranslated));
                    #for angleval in translationAngle: if angleval<=-1 or angleval>=1: print(i_event, angleval)
                    translationAngle = np.arccos(translationAngle);
                    translationAxis = np.cross(decayToVertex, decayToTranslated)
                    #if i_bitfield == 0:
                    #    print(f'translationAxis = {translationAxis[0]}') 
                        

                    translationAxis = [ thisV/np.linalg.norm(thisV) if np.linalg.norm(thisV) != 0 else np.zeros_like(thisV) for thisV in translationAxis]
                    translation_rot_vec = np.multiply(translationAxis, translationAngle[...,None])

                    decayToTranslated = [ thisV/np.linalg.norm(thisV) if np.linalg.norm(thisV) != 0 else np.zeros_like(thisV) for thisV in decayToTranslated ]
                    phi_rot_vec = np.multiply(decayToTranslated, throw_phi[...,None])

                    # print("decayToTranslated",end="=")
                    # print(decayToTranslated)

                    # Get rotation matrices due to:
                    # Vertex translation (which "rotates" the average neutrino direction)
                    translation_rot=R.from_rotvec(translation_rot_vec)
                    
                    # Random phi rotation around average neutrino direction
                    phi_rot=R.from_rotvec(phi_rot_vec)
                    # Rotate momentum
                    this_p=translation_rot.apply(this_p)
                    #if i_bitfield == 0:
                    #    #print(f'First set of throws')
                    #    print(f'	throw position 0 ({throw_x[0]}, {throw_y[0]}, {throw_z[0]})')
                    #   #print(f'	decayToVertex = {decayToVertex}')
                    #   print(f'	translation axis of rotation: {translationAxis[0]}')
                    #   print(f'	translation angle [0]: {translationAngle[...,None][0]}')
                    #    print(f'	np.linalg.norm(this_p[0]): {np.linalg.norm(this_p[0])}')
                    #   print(f'	this_p[0] after translation rotation applied: {this_p[0]}')
                    this_p=phi_rot.apply(this_p)
                    #if i_bitfield == 0:
                    #    print(f'	throw rotation angle [0]: {throw_phi[...,None][0]}')
                    #    print(f'	decayToTranslated[0] = {decayToTranslated[0]}')	
                    #    print(f'	np.linalg.norm(this_p[0]): {np.linalg.norm(this_p[0])}')
                    #    print(f'	this_p[0] after throw rotation applied: {this_p[0]}')
                    #    #print(f'	this_p[-1] after throw rotation applied: {this_p[-1]}')
                    
                    
                    #Shift the vertex to on-axis for the NN
                    #this_vtx_x=this_vtx_x-LAr_position[det_pos]
                    #throw_x=[this_vtx_x]*len(throwsFD["throwVtxZ"][0][i_bitfield*64:(i_bitfield+1)*64])
                    #print(f"this_vtx_x = {this_vtx_x}")
                    #print(f"throw_x[0] = {throw_x[0]}")
                    #print(f'throw_x[20] = {throw_x[20]}')

                    # Features contains randomized momentum and vertex, to be used in neural network.
                    features=np.column_stack((this_p[:,0], this_p[:,1], this_p[:,2], throw_x_shifted, throw_y, throw_z))
                    features=torch.as_tensor(features).type(torch.FloatTensor) # Convert to Pytorch tensor
                    with torch.no_grad(): # Evaluate neural network #neural network output is 2D array of probability a set of events being contained-detected, tracker-detected,
                        netOut=net(features) # or not detected #I don't use the 3rd column (not-detected probability)
                        netOut=torch.nn.functional.softmax(netOut).detach().numpy()

                    # Get contained probability for 64 throws
                    nnContained=np.array(netOut[:,0], dtype=float)
                    # Get tracker probability for 64 throws
                    nnTracker=np.array(netOut[:,1], dtype=float)
                    #print throws value for a given vtx_x
                    # if throw_x[0] == -298.55 and i_event == 1:
                    #     print(" event: ",i_event, " throw_x: ", throw_x, " nnTracker: ", nnTracker[0], " nnContained: ", nnContained, " hadron: ", bitfield)
                    # MUON CONTAINED AND HADRON CONTAINED, throw by throw
                    combinedEfficiencyContained=np.multiply(nnContained, bitfield)
                    # OR: MUON TRACKER AND HADRON CONTAINED, throw by throw
                    combinedEfficiencyTracker=np.multiply(nnTracker, bitfield)
                    # Combined hadron containment and muon selection efficiency, throw by throw
                    combinedEfficiency=np.add(combinedEfficiencyContained, combinedEfficiencyTracker)

                    #nnContained_values.push_back(nnContained)

                    #print(" event: ",i_event, " throw_x: ", throw_x, " nnTracker: ", nnTracker[0], " nnContained: ", nnContained, " hadron: ", bitfield, "throws_FV[i_bitfield*64:(i_bitfield+1)*64]", throws_FV[i_bitfield*64:(i_bitfield+1)*64])

                    slice_of_throws_FV = throws_FV[i_bitfield*64:(i_bitfield+1)*64]
                    # List to store indexes where value is False; if value = false -> not in FoV
                    false_indexes = []
                    for index, value in enumerate(slice_of_throws_FV):
                        if not value:
                            false_indexes.append(index)

                    #print("indexes with wrong val: ", false_indexes)
                    for fIndex in false_indexes:
                        #replace probability with -1 when not in FoV
                        nnContained[fIndex] = -1


                    # if throw_x[0] == 298.55 and i_event == 1:
                    #     # print("indexes with wrong val: ", false_indexes)
                    #     print("i_bitfield: ", i_bitfield, " index: ", index, "value", value)
                    #     print(" event: ",i_event, " throw_x: ", throw_x, " nnTracker: ", nnTracker[0], " nnContained: ", nnContained, " hadron: ", bitfield, "throws_FV[i_bitfield*64:(i_bitfield+1)*64]", throws_FV[i_bitfield*64:(i_bitfield+1)*64])

                    #===save results with throws in FoV only : nnContProb != -1 and also throws which pass the hadron veto: hadresult !=0: ==> this is the weight to be aplied to Etrim histos
                    for hadresult, nnContProb, nnTrackerProb in zip(bitfield,nnContained,nnTracker):
                        if nnContProb != -1 and hadresult !=0:
                            #print("nnProb: ", nnContProb, "had result: ", hadresult)
                            weightPmuon.push_back([nnContProb*hadresult + nnTrackerProb*hadresult]) #==> this is the weight to be aplied to Etrim histos, saved on throw by throw basis
                            nnContained_values.push_back([nnContProb])
                            #if throw_x[0] == 298.55 and i_event == 8:
                            #    print("nnProb: ", nnContProb, "nnTracker: ", nnTrackerProb, "weight Pmuon: ", nnContProb*hadresult + nnTrackerProb*hadresult)


                    #only save probability values != -1 (not interested in event that are not in FoV -> those throws never counted) -- no condition on hadron result
                    # for value in nnContained:
                    #     if value !=-1:
                    #         nnContained_values.push_back([value])


                    if FV_cut: #Count only throws which were in the fiducial volume and add them to the efficiency accumulators
                        thisEff_tracker+=np.sum(np.multiply(nnTracker, throws_FV[i_bitfield*64:(i_bitfield+1)*64]))
                        thisEff_contained+=np.sum(np.multiply(nnContained, throws_FV[i_bitfield*64:(i_bitfield+1)*64]))
                        thisEff_combined+=np.sum(np.multiply(combinedEfficiency, throws_FV[i_bitfield*64:(i_bitfield+1)*64]))
                        # nnContained_values.push_back(np.multiply(nnContained, throws_FV[i_bitfield*64:(i_bitfield+1)*64]))

                    else:
                        thisEff_tracker+=np.sum(nnTracker)
                        thisEff_contained+=np.sum(nnContained)
                        thisEff_combined+=np.sum(combinedEfficiency)

                    #xz_angle=np.arctan(this_p[:,0]/this_p[:,2])
                    #yz_angle=np.arctan(this_p[:,1]/this_p[:,2])
                    # for i in range(64):
                    #   if (det_pos==14):
                #         #if (xz_angle<0.1 and yz_angle<0.1):
                #         print(throw_number, end="; ")
                #         print(throw_y[i], end="; ")
                #         print(throw_z[i], end="; ")
                #         print(throw_phi[i], end="; ")
                #         print(nnContained[i], end="; ")
                #         print(nnTracker[i], end="; ")
                #         print(this_p[i,2])
                #         if nnTracker[i]<0.01: num_0_effs+=1
                #         if nnTracker[i]>0.9: num_high_effs+=1
                #         throw_number+=1

                # print("event ",end="#")
                # print(i_event,end=", ")
                # print("LAr position",end="=")
                # print(LAr_position[det_pos],end=", ")
                # print("vertex position",end="=")
                # print(vertex_position[vtx_pos],end=", ")
                # print("hadron efficiency: ",end="")
                # After looping through all throws, divide by number of throws in the fiducial volume to get average efficiency.
                # print(thisEff/NthrowsInFV)
                # effs.back().push_back(thisEff/NthrowsInFV)
                # effs_tracker.back().push_back(thisEff_tracker/NthrowsInFV)
                # effs_contained.back().push_back(thisEff_contained/NthrowsInFV)
                # muon_efficiency=(thisEff_contained+thisEff_tracker)/NthrowsInFV
                # # if muon_efficiency>1.:
                # #     print("muon selected efficiency", end="=")
                # #     print(muon_efficiency)
                # #     muon_efficiency=-2.
                # effs_selected.back().push_back(muon_efficiency)
                # effs_combined.back().push_back(thisEff_combined/NthrowsInFV)
                effs.push_back(thisEff/NthrowsInFV)
                effs_tracker.push_back(thisEff_tracker/NthrowsInFV)
                effs_contained.push_back(thisEff_contained/NthrowsInFV)
                muon_efficiency=(thisEff_contained+thisEff_tracker)/NthrowsInFV

                # if muon_efficiency>1.:
                #     print("muon selected efficiency", end="=")
                #     print(muon_efficiency)
                #     muon_efficiency=-2.
                effs_selected.push_back(muon_efficiency)
                effs_combined.push_back(thisEff_combined/NthrowsInFV)
                vtxX.push_back(this_vtx_x)
                #print("vtx x: ", this_vtx_x, "hadron eff", thisEff/NthrowsInFV, "muon eff: ", (thisEff_contained+thisEff_tracker)/NthrowsInFV, "eff combined: ", thisEff_combined/NthrowsInFV, "hadEff*MuEff", thisEff/NthrowsInFV * (thisEff_contained+thisEff_tracker)/NthrowsInFV)

        tree.Fill()
        # print("\n")
        # print("low effs", end=": ")
        # print(num_0_effs)
        # print("high effs", end=": ")
        # print(num_high_effs)
        # print("geometric tracker efficiency", end=": ")
        # print(thisEff_tracker/NthrowsInFV)
        # print("\n")
        print("event: ",i_event, " N in FV: ", NthrowsInFV )
    tree.Write()
    tf.Close()
    #end=time()
    #print(15*22*(end-start)/4)

def main(argv=None):
    global CAF_files, net

    if argv is None:
        argv = sys.argv[1:]
    if len(argv) != 1:
        raise SystemExit("Usage: make_tree_fd <FD-CAF-file>")

    CAF_files = str(argv[0])
    net = muonEffModel()
    model_path = Path(__file__).resolve().parent / "assets" / "muonEff30.nn"
    net.load_state_dict(torch.load(str(model_path), map_location=torch.device("cpu")))
    net.eval()
    return processFiles(CAF_files)


if __name__ == "__main__":
    main()
