> [!NOTE]
> This documentation was written for the standalone DUNE_ND_GeoEff repository. The code now lives in
> `lblpwgtools/CAFAna/PRISM/GeoEff`; paths such as `app/`, `include/`, `src/`, `macros/` and `data/` are relative to that directory.
> Build and run instructions are in [../README.md](../README.md).

# This folder contains all the necessary documentation for the geometric efficiency of FD Events at the ND 

The code as it is right now is taking events from the FD (information from the FD events is saved in FD ntuples - 
more info than the standard FD CAFs, they have info regarding simulated hadronic energy deposits at a given x, y, z position,
simulated muon momentum and vertex coordinates at the start and end position, type of neutrino interaction (CC or NC) etc

These FD ntuples are the starting point in the Geo Efficiency of FD events at the ND.

The FD events are then translated to the ND where random throws are generated and the hadron and muon geometric efficiency of a given FD event at the ND is then calculated on a data-driven approach. The last part of the code is regarding the analysis of the FD events translated to the ND, more explicitly how to translate the obtained results in terms of a PRISM-dedicated analysis. The main goal (so far) is to be able to obtain a distribution of the selected FD events in the ND that would match the linearly combined selected ND events obtained from the ND-CAFs with a model-dependent MC efficiency. To fulfill this goal a very important variable is the **energy**, as we want to make sure the same variable is comparred. Currently we use the deposited energy (obtained from GEANT 4) for the hadrons in the ND + true muon energy. For the ND-CAFs this variable is already defined within the CAFAna framework and is called ```ERec```. For the FD events in the ND this variable is further referred to as ```VisEtrim``` and is the sum between the hadronic energy deposited inside ND-LAr active volume (```TrimEnergy```) and true muon energy. 

The information below tries to give an overview of the several steps used in the analysis to obtain the end-goal comparison between the FD selected events at the ND and the linearly combined selected ND events as obtained from MC NDCAFs.

## Analysis sketch

```
| -------------------------------           ------------------------------------------               -----------------------------------------
| 0. FD neutrinos simulations   |          | 1. Hadron Geom. Eff. of FD Events       |              | 2. Muon Geom Eff. of FD Events          |
| ->(FD ntuples)                |          |  in the ND -> OutputHadronGeoEff.root   |              | in the ND -> OutputMuonFDGeoEff.root    |
|                               |          |                                         |              |                                         |
| MyEnergyAnalysis.cc           |          | runGeoEffFDEvtSim.cpp                   |              | FD_maketree.py                          |
| -> From Flynn's repository    |          | -> this repository                      |              | -> this repository                      |
|                               |          |                                         |              |                                         |
|                               | ------>  |                                         | -----------> |                                         |
| - neutrino interaction type   |          |  - throws FD event in ND at diff. vtxX  |              |  - throws events that passed the hadrons|
| - neutrino energy             |          |   and detPos                            |              | selection and evaluates muon efficiency |
| - hadronic energy deposits    |          |                                         |              |                                         |
|   - amount of E + coordinates |          | - throws passing the hadron cuts        |              |     - uses a NN which take the muon     |
| - muon energy                 |          |     - trim energy per throw             |              | momentum and position vector of each    |
|   - muon track length,        |          |     - vtxX + detPos                     |              | throw passing the hadron selection and  |
|   start + end track           |          |                                         |              | calulcates the muon efficiecny          |
|                               |          |                                         |              |                                         |
 --------------------------------           ------------------------------------------               ------------------------------------------  


------------------------------------           ---------------------------------------              -----------------------------------------
| 5. Compare selected FD events    |           | 4. Get results for ALL FD events    |              | 3. Get Etrim distribution of FD events|
|  in the ND with PRISM            |           |                                     |              | selected (muon and hadron sel. cuts)  |
|  linearly combined ND data       |           |  SumEtrimHistos.C                   |              |                                       |
|                                  |           | -> this repository                  |              | NtupleOutVetoAndTrimE_....            |
|PlotAllOAPosWithCAFLikeCutClean.C |           |                                     |              | -> this repository                    |
| -> this repository               |           |- sum results from the otuput files  |              |                                       |
|                                  |  <-----   |  produced in steps 3.               |  <-------    |  Selected FD events in the ND vs      | 
|  - end comparison to check       |           |                                     |              | Etrim vs off-axis position            |
|   final result                   |           |                                     |              |                                       |
|                                  |           |                                     |              |                                       |
------------------------------------           ---------------------------------------               ----------------------------------------  
```



## Analysis steps 

In principle the working steps are the following:
1. Get the hadron geometric efficiency of FD events at the ND
2. Get the muon geometric efficiency (NN) of FD events that pass the hadron geo eff -> get the combined efficiecny (passing both hadron cuts and muon cuts)
3. Analyse the FD events selected at the ND (access Etrim histograms of each passing throw from hadron geo Eff, weight them by the probability the event passes the muon cuts, Pmu(Emu,throws)
4. Sum together all FD events translated to the ND -> evaluate the ND response to the entire FD spectrum
5. Compare All selected FD events at the ND with PRISM linearly combined ND data

More details about each individual steps: 
## 1. Hadron Geometric efficiency of FD events at the ND

The code for this step is in `lblpwgtools/CAFAna/PRISM/GeoEff` (originally the ```master``` branch of https://github.com/icaracas/DUNE_ND_GeoEff/).

  - the main code of interest is in ```app/runGeoEffFDEvtSim.cpp```
  
  - header with various dimensions and variables / constants definition is at ```include/step1_helpers.h```

  - file with most of the geometric efficiency coded functions is at  ```src/geoEff.cpp```

The code itself is heavily based on Flynn's previous work.
  ## Big picture 

We work with neutrino events simulated inside the FD. We take the (only CC interactions)  events from the FD ntuples located at ```/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_53993115```: 1 neutrino event has inofrmation about the hadron energy depoits and muon (coordinates x, y, z and amount of energy and momentum) in the FD. We then translate the FD event (with all the hadron and muon info) to the ND (accounting for Earth curvature).

We then put the ND at different positions with respect to the neutrino beam axis: variable to set the ND position is ```ND_LAr_dtctr_pos_vec```. In the current version of the code we only set the ND on-axis, i.e ```ND_LAr_dtctr_pos_vec.emplace_back(0);``` but you can easily change this with different off-axis positions. For example if interested in having the detector at 30m off axis use ```ND_LAr_dtctr_pos_vec.emplace_back(-3000)```. detector positions are given in cm. If you want to run with different detector positions at once just add all the desired detector positions in this variable.

The loop of events is the following: loop over neutrino events in FD (CC interactions only) -> loop over detector positions -> loop over vtx_x inside the ND -> loop over throws. Therefore, each neutrino event simulated in the FD which has a CC interaction, will be put in the ND. Then: 
  
  - for a fixed detector position (let's say on axis for now) we place the event at different vtx_x position inside the ND. This is done using the ```generatePoints``` function inside the code, and picking from an array of ```non_dead_regions``` in the ND. Once the vtx_x position has been fixed :
      - for a fixed vtx_x position inside the ND: we generate random throws of the event (particularly of the hadron energy deposits) at vtx_y, vtx_z and with a given rotation. basically once the event is posistioned at a fixed vtx_x position inside the ND we start randomly `throwing` (or moving) the event at various positions inside the ND in y and z coordinates, as well as rotating the event under different configurations.
          - for each throw we can access the **passing result** ```throw_result```, i.e if the given event under the given configuration (vtx_x, vtx_y, vtx_z and rotation) has passed the hadronic veto cut (i.e less than 30 MeV deposited in the outer 30cm of the ND) , the amount of hadronic energy deposited inside ND-LAr active volume (further referred to as **Trim Energy, Etrim** ```TrimEnergy``` in the code)  the amount of energy deposited outside the ND-LAr active volume, the amount of energy deposited in the veto region. Since this is inside the throws loop, every variable we save here (for example trim energy) will have a given distribution (many throws) at fixed vtx_x and detector position 
      - Once the throw loop is exited, using the information above, we can get the ```hadron geometric efficency``` = PassingThrows/ValidThrows at a fixed vtx_x position and fixed detector position  

After having run this code we have information regarding the hadron geometric efficiency of FD events as seen by the ND accounting for the different volumes between the FD and the ND: since FD has a much bigger volume, events with high hadronic energy deposits would never be seen entirely by the ND, much of this energy would be deposited outside of the ND-LAr active volume, which is why we in fact want to get the ND sensitivity to FD events with respect to the energy deposited inside the ND-LAr active volume, the trim energy Etrim -> we can get information regarding the efficiency and hadronic deposited energy of a given FD event inside the ND-LAr active volume (Etrim)

### 1.1 Read information from the FD nutples
The FD ntuples used as an input can be found at ```/pnfs/dune/persistent/users/flynnguo/myFDntuples/myntuple_53993115```
Several variables like neutrino energy, hadron energy deposits and their coordinates in the FD (x,y,z) as well as muon momentum (px, py, pz) and muon energy at the start and end position, etc are read from these ntuples via ```t->SetBranchAddress()```  

One can add additional variables of interest from the ntuple to the analysis by simply reading-in a new branch

### 1.2 Translate the FD event in the ND
Take the FD event (from the ntuple) and put it in the ND:     
  - 1. account for Earth curvature rotation: function that does this is ```eff->getEarthCurvature()``` defined in ```src/geoEff.cpp```
  - 2. FD event to ND: after Earth curvature rotation: get hadron positions and muon start/end positions after Earth curvature rotation

        -- variables: ```ND_RandomVtx_Sim_mu_start_v, ND_RandomVtx_Sim_mu_end_v, ND_RandomVtx_Sim_mu_start_p, ND_RandomVtx_Sim_hadronic_hit_xyz```
  - 3. ND: move back to the beam center (once we properly accounted for the Earth curvature rotation we want to move the event in the beam center in the ND) :

       -- variables: ```ND_OnAxis_Sim_mu_start_v, ND_OnAxis_Sim_mu_end_v, ND_OnAxis_Sim_mu_start_p, ND_OnAxis_Sim_hadronic_hit, ND_OnAxis_Sim_hadronic_hit_xyz```

       -- function: ```eff->getTranslations```defined in ```src/geoEff.cpp```

  - 4. Translate from On-axis to Off-axis: by default, at step 3 we put the neutrino (so the event) on axis. this step is moving the event from on-axis to off-axis according to the off-axis position

       -- variables: ```ND_OffAxis_Unrotated_Sim_mu_start_v_xyz_LAr, ND_OffAxis_Unrotated_Sim_mu_end_v_xyz_LAr, ND_OffAxis_Unrotated_Sim_mu_start_p_xyz_LAr, ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz_LAr```

    - only change the x component ( our detector is moved along the x direction) , y and z components remain unchanged for muon
       ```bash
       ND_OffAxis_Unrotated_Sim_mu_start_v[0] = ND_OnAxis_Sim_mu_start_v[0] + i_ND_off_axis_pos + i_vtx_vx;
       ```
    - for hadron part, use the already rotate muon (again just x direction is actually different) and translate the hadron deposits at a different off-axis position
      ```bash
      ND_OffAxis_Unrotated_Sim_hadronic_hit_xyz.emplace_back((float)eff->getTranslations(ND_OnAxis_Sim_hadronic_hit_array, ND_OnAxis_Sim_mu_start_v, ND_OffAxis_Unrotated_Sim_mu_start_v, i));
      ```
  - 5. ND Get eigen rotated vectors for step 4 ': here nothing is changed for the muons (we don't rotate the muons in this code), as the function ` eff->RemainUnchanged` is used, but this is wehere we define the variables further used for the off axis positions:

        -- variables for muons at different off-axis positions: ```ND_OffAxis_Sim_mu_start_v_xyz, ND_OffAxis_Sim_mu_end_v_xyz, ND_OffAxis_Sim_mu_start_p_xyz_vtx, ND_OffAxis_Sim_mu_start_E_xyz_vtx ```

        -- variables for hadrons: ```ND_OffAxis_Sim_hadronic_hit```

        -- function : ```Eff->getOffAxisHadronHitV``` defined in ```src/geoEff.cpp```

       At this point we have information about the position of the hadron hits inside the ND for each off axis position (at each detector positions chosend and at each vtx X positions generated),  ```ND_OffAxis_Sim_hadronic_hit``` and can access variables like total hadron energy deposited in the FD, as taken from ntuples, ```totEnergyFDatND_float```, energy deposited in the ND veto region (30 cm) , ```vetoEnergyFDatND_float```, or hadron energy deposited outside the ND active volume, ```outEnergyFDatND_float```. This energy deposited outside the ND active volume is energy that our ND would never see and that is why we trim it out.
      
  - 6. Generate random throws
       At a fixed ```vtx_x``` position the event is thrown randomly at different ```vtx_y, vtx_z ``` with different rotations. The values for these throws are generated using the ```eff->throwTransforms()``` function and they are further saved in the output file using:
    ```bash       
      throwVtxY.clear();
      throwVtxZ.clear();
      throwRot.clear();
      throwVtxY = eff->getCurrentThrowTranslationsY();
      throwVtxZ = eff->getCurrentThrowTranslationsZ();
      throwRot  = eff->getCurrentThrowRotations();
      ThrowsFD->Fill();
    ```
      the result of each throw (passing the hadron veto cut or not) is saved in the variable ```hadron_throw_result``` , calculated using the function ```eff->getHadronContainmentThrows_FD_GEC(false)``` defined in ```src/geoEff.cpp```
  
      -- functions that return the veto energy, ```eff->getVetoEPerThrow_FD_GEC()``` and the trimmed energy (```TrimE = totEnergyFDatND_float - outEnergyFDatND_float```) , ``` eff->getTrimEPerThrow_FD_GEC()``` on a throw by throw basis are defined in ```src/geoEff.cpp```

      -- while looping over each throw (each detector position, each vtxX position, each throw) we only save information about passing throws (passing throws has a non zeo hadron result: ```throw_result != 0 ``` . information we need later in the analysis is the trim energy of each passing throw, ```TrimEnergyEventsPass.emplace_back(trimEThrow[0][0][(counter5-1)*64 + ithrow])```

      -- also save the number of times an event passed the throws, ```hadronpass```, and then divide to the total number of throws, ```validThrows```, to get the hadron geometric efficiency of the particular FD Event, at a given detector position in a given vtx_x position inside the detector:
    ```bash
    if ( validthrows > 0 ) hadron_contain_eff = hadronpass*1.0/validthrows;
    ND_GeoEff = hadron_contain_eff;
    ```

    ### 1.3 Write important variables to the output file:

    - variables further used for the muon efficiency part:
            
       muon info: ```ND_OffAxis_Sim_mu_start_v_xyz_LAr, ND_OffAxis_Sim_mu_start_p_xyz_LAr", ```

       throw info: ```throwVtxY, throwVtxZ, throwRot,  hadron_throw_result_LAr```
      

    - variables used in the end analysis:
   
        vertex_x and detector postions: ```ND_LAr_vtx_pos, ND_LAr_dtctr_pos ```

        GeoEff and throws info: ```ND_GeoEff, NPassedThrows, TrimEnergyEventsPass```

        Event info: ```ND_Gen_numu_E, ND_E_vis_true, totEnergyFDatND_f, muonEdep_f, muonTrackLength_f, ND_OffAxis_Sim_mu_start_E_xyz_LAr```
      
        
More variables (than the ones enumerated here) are written in the output file, but I think these are the most important ones so far. 


## 2. Muon Geometric efficiency of FD events at the ND

The code for this step is in `lblpwgtools/CAFAna/PRISM/GeoEff` (originally the ```MuonCode``` branch of https://github.com/icaracas/DUNE_ND_GeoEff/).
  
  - the main code of interest is in ```app/make_tree_fd.py```

The code itself is based on Flynn's previous work but has several additions / changes. However the NN part has not been touched - don't have more details regarding to how this one works..
 ## Big picture 

 This part of the code is using information obtained from the previous, hadron geometric efficiency step. The input file needed for this step is the output file obtained after having run the hadron geometric efficiency. Since in the end we are interested in events that pass both the hadron and muon selection cuts, the muon geometric efficiency is only calculated for events that have already passed the hadron geometric efficiency cut (i.e an event that doesn't pass the hadron veto cut would not be selected at the ND so it is useless to calculate the muon efficiency for such an event).  The muon geometric efficency is however following the same approach. A FD event is translated to the ND, the ND is positioned at different off-axis positions with respect to the neutrino beam, variable in this code that sets the ND position is ```LAr_position```. It is important to set the same value here as you set in the hadron geometric efficiency part. For each of the throws that pass the hadron geometric selection, for a fixed detector position we put the event at different vtx_x positions (same values for vtx_x as used in the hadron geo efficiency step). For each of these configurations of the event we use a NN that tells us whether the muon is either contained in ND-LAr or further matched int the downsream tracker, TMS. Once the NN ran we can acces the information related to : proability muon is contained in NDLAr, ```nnContained``` , probability the muon is matched in TMS, ```nnTracker``` or the combined muon selection efficiency = ```nnContained + `nnTracker``` per throw. Again since we run this code on events that have already passed the hadron selection, the results obtained from ```nnContained``` resp ```nnTracker``` are already in terms of **combined efficiecny = muon(contained or tracker) and hadron efficency** (hadron efficiency is already applied). However since in the end we care about muon probability to be either contained in ND-LAr *OR* matched in tracker, the variable of interested is `weightPmuon = nnContProb*hadresult + nnTrackerProb*hadresult` which gives us exaclty this probability. This is saved on a throw by throw basis as well and can further be used in the end analysis ( see point 3 ) 

### 2.1 Read information from the root files obtained from the hadron geo eff part

If for example your output file from the hadron geo eff part is called OutputHadronGeoEff.root than run the muon efficiency code as: ```python FD_maketree.py OutputHadronGeoEff.root``` . As a result you will obtain a file with information regarding the combined (hadron and muon efficiency).

  - variables needed as an input:

    muon info: position and momentum of the muon startign point (x, y, z) `ND_OffAxis_Sim_mu_start_v_xyz_LAr`, `ND_OffAxis_Sim_mu_start_p_xyz_LAr`

    throws info: result of the hadron throws (only saved for throws that pass the hadron selection cuts), `hadron_throw_result_LAr`, throws coordinates: `throwVtxY`, `throwVtxZ`, `throwRot`
 
### 2.2 Get Muon momentum direction vector

Need to get the corresponding coordinates of the muon direction at a givent detector position and vtx_x position inside the detector. For each neutrino events in the FD -> for each detector position -> for each vtx_x position -> for each passing throws from the hadron geo eff part:
    #### Get variables needed to evaluate muon neural network for each throw
   - Prepare vertex positions for each random throw:

        x coordinate is fixed `this_vtx_x` and repeated for all throws (same values as in hadron geo eff). -> `throw_x`

        y, z, and phi are taken from the random throws (throwsFD) and corrected by detector offsets -> `throw_y`, `throw_z`, `throw_phi`

  - Determine the neutrino production point

       Use the vertex x-position to estimate the neutrino decay position along the beamline.

       Convert this decay position from beamline coordinates → detector coordinates.

  - Build direction vectors from production point

       Compute the vector from the decay point → original vertex.

       Compute vectors from the decay point → each randomly thrown vertex.

  - Compute rotation needed due to vertex translation

       Calculate the angle between the original and translated vectors.

       Compute the rotation axis using the cross product.

       Convert axis + angle into a rotation vector.

  - Compute additional random rotation

       Apply a random φ rotation around the neutrino direction for each throw.

  - Build rotation matrices

       One rotation accounts for the change in neutrino direction due to vertex translation.

       Another rotation accounts for the random φ rotation around that direction.

  - Rotate the muon momentum

       Apply the translation-induced rotation.

       Apply the φ rotation.

       The resulting momentum vector is used for the neural network evaluation -> `this_p`
 
### 2.3 Run NN to get the probability of muon passing the muon selection cuts

Give the input variables (muon momentum and throws coordinates) to the network: 
```bash
features=np.column_stack((this_p[:,0], this_p[:,1], this_p[:,2], throw_x, throw_y, throw_z))
```

Evaluate the network:
```bash
with torch.no_grad(): # Evaluate neural network #neural network output is 2D array of probability a set of events being contained-detected, tracker-detected,
                        netOut=net(features) # or not detected #I don't use the 3rd column (not-detected probability)
                        netOut=torch.nn.functional.softmax(netOut).detach().numpy
```
Access the result (this is on a throw by throws basis): 

  -- variables: `nnContained`, `nnTracker`
  
  -- define probability that muon is either contained ND-LAr, or futhrer matched in downstream tracker, Pmu: 
  ```bash
  weightPmuon.push_back([nnContProb*hadresult + nnTrackerProb*hadresult])
  ```
  


### 2.4 Save important variables in an output file

Variables saved and further used in the analysis file:
  - ```hadron_selected_eff```
  
  - ``` muon_tracker_eff```
  
  - ```muon_contained_eff```
  
  - ```muon_selected_eff```
  
  - ```combined_eff``` - this is the combined (hadron and muon selection cuts) efficiency

  - ```vtxX"```
  
  - ```weightPmuon```


As a result of running the muon efficiency part an output file, i.e `OutputMuonFDGeoEff.root` is created and contains the above mentioned variables.


## 3.  Analyse the FD events selected at the ND 

The code for this step is in `lblpwgtools/CAFAna/PRISM/GeoEff` (originally the ```EtrimAnalysis``` branch of https://github.com/icaracas/DUNE_ND_GeoEff/). 

The 2 main files of interest are: ```app/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND.C``` and ```app/NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND.C```
The difference between those 2 files are whether one wants to work with the oscillated spectrum or with the non-oscillated one -- codes are very similar but will try to highlight the main differences below.

This part is using information obtained from the hadron geoemtric efficiency and muon geometric efficiency parts and puts it together towards the final desired distribution of selected FD events in the ND. 

## Big picture
As already mentioned, the end goal is a direct **comparison between the FD events slected at the ND (GeoEFFcorrected) and the linearly combined ND events**. The main variable of interest here is **ETrim**, which is the total amount of hadronic energy deposited by a FD event inside the active volume of the ND. Since the ND volume is smaller than the FD one, the amount of energy deposited inside the ND will be smaller. This is why we need to trim out any amount of hadronic energy that is deposited outside the active volume of the ND. This is already done in step 1 - Hadronic geometric efficiency section. Here we want to properly access this information and get our end results in terms of **selected events FD events when put in the ND as a function of the total amount of energy deposited in the ND and at a given off-axis position**. 

The main idea here is that every FD event is translated to the ND, the ND is placed at different positions (in the framework so far the ND is only put on-axis and then here in the analysis we assume same efficiency at all off-axis positions), and then for each ND position we put the event in different vtx_x position inside the detector. For each of this vtx_x position we randomly translate and rotate the event and we evaluate both it's hadron efficiency (does the event pass the hadron veto cut? -- done in step 1 Hadron Geometric efficiency ) as well as the muon efficiency (for events that pass the hadron cut, what is the probability the muon would pass the selection criteria -- either contained in ND-Lar or further match in the downstream TMS -- done in step 2 Muon Geometric efficiency). 

For each event that passes the hadron geometric efficiency selection (we will have many passing throws at a fixed detector position and fixes vtx_x) we have a corresponding **trim energy: `Etrim`**, a corresponding **probability that the muon passes the selection criteria: `weightPmuon`** and the corresponding **muon energy `Emu`**. (NOTE: muon energy `Emu` is the true muon energy of the event so therefore this will be the same for 1 FD event regardless where it is put in the ND. However it is still used on a throw by throw basis for consistency.) That means that taking 1 FD event, for each fixed detector position and fixed vtx_x value we have a **distribution (histogram) of Etrim values**. For this 1 FD event (fixed detector position, fixed vtx_x) the amount of entries in this Etrim histogram will be equal to the amount of passing throws (from hadron geo efficiency). Since each of the passing throws in the hadron geo efficiency has a corresponding muon probability (for muon selection) and since we want to have the distribution of events passing ALL selection criteria, each event in this histogram will further be weighted by this muon probability, `weightPmuon` . 

At this point we have the `vis Etrim` (vis Etrim = Etrim + Emu is what we call "the visible trimmed energy" and is nothing else than the sum between the hadron energy deposited inside ND LAr and the muon energy) distribution of this particular FD event when translated to the ND, at a fixed vtx_x position inside the ND and fixed detector postion with respect to the neutrino beam axis. We can get such **distributions for each vtx_x and detPos** of interest. So we can then **represent this FD event in a 2D distribution, -- 2D histogram -- as a function of off-axis position ```OAPos = vtx_x + detPos``` and visible trimmed energy**  (vis Etrim = Etrim + Emu). -- i.e remember that at different vtx_x positions inside the detector we had different efficiencies and also different amount of energy deposited inside the ND-LAr active volume: more likely to deposit more energy outside the detector (so lower Etrim values) when the event is close to the ND edges. 

An additional and very important step that has to be accounted for is the *FD event rate at the ND*. This is described in more detail below but it essentially account for the fact that neutrinos with different energies would be seen with different probabilities in the ND, depending on the ND off-axis position. Remember that a detector located at 30m off-axis is sampling mainly very low energy neutrinos (high probability to see low energy neutrinos far off-axis and low probablitiy to see the high energy neutrinos at such off-axis positions). For an on-axis ND the neutrino spectrum extends to higher energies with a much wider coverage in energy (lower probability to see very lower energy neutrinos but higher probability for high energy neutrinos). 

In short, **each FD event can be mapped into the ND as a 2D distribution of off-axis position versus visible trimmed energy, capturing the geometric effects of both hadron and muon selection efficiencies**, allowing a **direct comparison between ND-observed and FD-derived events**. The last steps in the analysis is looping over all FD events (all ntuples) and summing them together in such a 2D histogram. This is the `SelectedEventsVsOAPosVsTotalETrim` histogram in the code, which is created per each event and is further summed for all events in the final (step 5) step of comparing the selected FD events with the ND linearly combined data.  

Below there is a more detailed explanation of the code itself and the additional cuts (i.e CAF-like cut for low energy muons + FD EventRate at ND ) that are applied to this `SelectedEventsVsOAPosVsTotalETrim` histogram.


### 3.1 Read information from the root files obtained from the hadron geo eff and muon geo eff part

The code is run using both the hadron geo eff and muon geo eff output files. i.e assuming you have ` OutputHadronGeoEff.root` from hadron and `OutputMuonFDGeoEff.root` from muon, you would run:

-- for no oscillations
```bash
./NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND OutputHadronGeoEff.root OutputMuonFDGeoEff.root
```
-- for oscillations
```bash
./NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND OutputHadronGeoEff.root OutputMuonFDGeoEff.root
```

We read in more variables (than we actually use) but I think it's always useful to have access to more information. 

 - variables needed as an input (from the hadron geo eff ):
   - `iwritten` -- event counter (goes from 0 to nr of FD events in the original FD ntuple used as input for hadron and muon eff)
   - `ND_LAr_dtctr_pos` -- detector off-axis position with respect to neutrino beam (so far this is only equal to 0 in the current analysis , but would be useful for the future when using different detector poistions)
   - `ND_LAr_vtx_pos` -- vtx_x choices inside ND-LAr
   - `ND_GeoEff` -- this is the hadron geometric efficiency -> 1 value / vtxX / dePos/ FDEvent
   - `validThrows` -- this is the total number of throws of a given FD event -> 1 value/ vtxX / detPos / FD Event
   - `NPassedThrows` -- number of throws that passed the hadron selection cut (for each vtxX position this number of passing throws is different) -> 1 value/ vtxX / detPos / FD Event
   - `TrimEnergyEventsPass` -- trimmed energy (i.e hadronic energy deposited inside ND-LAr active volume) of events that pass the hadron selection -- here we have one value for each passing throw -> 1 value/ vtxX /PassingThrowAtVtx /  detPos / FD Even

   - `totEnergyFDatND_f` -- total hadronig energy deposited inside FD Active volume (this is what we trim from) -> 1 value / FD Event
   - `muonEdep_f` -- deposited energy of muon in FD -> 1 value / FD Event
   - `muonTrackLength_f` -- muon track length -> 1 value / FD Event
   - `ND_Gen_numu_E` -- true neutrino energy at generator level -> 1 value / FD Event
   - `ND_E_vis_true` -- true visible neutrino energy -> 1 value / FD Event
   - `ND_OffAxis_Sim_mu_start_E_xyz_LAr` -> muon energy momentum vector: used to get muon energy -> 1 value / FD Event

- variables needed as input (from muon geo eff):
   - `combined_eff` -- combined (hadron and muon) efficiency -> 1 value/ vtxX / detPos / FD Event
   - `muon_selected_eff` -- muon efficiency -> 1 value/ vtxX / detPos / FD Event
   - `muon_tracker_eff` -- muon tracker efficiency (i.e only passing the tracker cuts) -> 1 value/ vtxX / detPos / FD Event
   - `weightPmuon` -- muon probability to be either contained in ND-LAr *OR* matched in tracker -> 1 value/ vtxX/ PassingThrowAtVtx / detPos / FD Event (PassingThrowAtVtx is taken from the hadron eff part)

### 3.2 Analysis specifics variables and functions

- variables we need and use / define within the code:
  - `a_ND_off_axis_pos_vec` -- these are the detector positions we need to use, same as those for the ND CAFs. Since currently we assume the same efficiency at all detector positions, we only run the hadron and muon geometric efficiencies for a detector situated on-axis and we then use different off-axis positions within this analysis part. The idea here is the detector is moved at each inidivudal position inside this vector, and then the efficiency vs vtxX is 'copied' as being the same at every one of these positions. The detector positions used are: `0, -1.75, -2, -4, -5.75, -8, -9.75, -12, -13.75, -16, -17.75, -20, -21.75, -24, -25.75, -26.25, -28, -28.25, -28.5` in meters.
NOTE: if in the future one chooses to not assume equal efficiency at all detector positions then this would be changed within the muon and hadron geometric efficiency part and this vector in the code would not be used, instead you shoud use all the detector possitions used in the geo eff and muon code, saved in the `ND_LAr_dtctr_pos` variable
   - `a_ND_vtx_vx_vec` -- vtx_x positions, this variable is recreating the same vtx_x positions used in the hadron and muon geo eff code.
   - `OAPos` -- this variable is calculating the off-axis position at each detector and vtx_x position: `OAPos = vtx_x+det_pos` -> this is the Off-axis position variable we also use in the PRISM analysis and represents the off-axis position of a given event
   - `CoefficientsAtOAPos` -- these are the off-axis coefficients; in the current version of the analysis we don't use them anymore, I am fairly sure one should use either the coefficients or the FD Event Rate at ND (more details about this below) but in any case they are still in the code and can be accessed at any time
   - `WeightEventsAtOaPos` -- this is a weight that accounts for different (non-flat) distribution of events at different detector positions: our detector covers 4m (from -2 m to 2m) width in x direction; since we move the detector at several locations, more often than every 4 m, we are covering some off-axis regions more often than others and have to correct for this. By using this weight we account for *how many times have we put a FD event in a given off-axis position, given our ND detector positions*. If we had detector stops every 4m-only, then every event would only be put once in each off-axis position and this weight would be equal to 1. Together with the *FD Event rate at ND* function this weight is accounting for how often have we put the event at an off-axis position and with which probability. I.e if a low neutrino-energy FD event (higher probability to come from far off-axis than from on-axis) has been put more times on-axis (lots of detector movements very close around the on-axis position) than far off-axis(less detector stops far off-axis), the event's main contribution would still come from far off-axis, where the FD event rate at the ND is higher. If you are interested in how this weight looks like check the `HistOAPos` histogram. 
   - `EnuTrue` -- variable where we save the true neutrino energy / FD Event
   - `TotalLeptonMom` -- variable with muon total energy / FD Event

### FD Event Rate at ND Functions (Try to use Ehad - Elep to avoid model dependence):

As mentioned above, we have to account for the FD event rate at the ND. Since we take the FD events and put them in the ND, at different off-axis positions, we have to account for this rate. This is related to the fact that neutrinos with a given true energy have different probabilities of appearing at a different off-axis position in the ND. Take for example a very low energy neutrino: this would have a much higher chance to appear from somewhere far off-axis than from the on-axis (left figure below). Similarly an event with a high neutrino energy will have a much higher probability to appear from an on-axis ND then from very far off-axis (right side figure below) -- remember the further we go off-axis with the ND the sampled neutrino spectra peaks at lower and lower energies and gets narrower and narrower. Therefore each event in the FD will have a corresponding probability, `FDEventRateAtND(E, OAPos)` to appear at a given off-axis position depending on its energy. This probability is normalized to 1 over the entire off-axis range. If we don't account for this **FD Event Rate at the ND**, we wrongly give an equal probability for any FD Event, regardless of its energy, to appear in the ND, at any off-axis position, which is wrong!
<p align="center">
  <img src="https://github.com/user-attachments/assets/48e29025-2acc-4e70-9bc5-9b18ead9ac97" width="45%">
  <img src="https://github.com/user-attachments/assets/08e91540-a3f8-4f75-8c59-19c3f56d821b" width="44%">
</p>

Depending on the energy variable used -- in this example I mentioned true neutrino energy, but one can use different energies, for example hadron energy and muon energy of a given event,  hadron +muon energy etc. The true neutrino energy should in principle be the most correct variable. However by using true neutrino energy we add model-dependence. So it would be ideal to be able to use energy variable that we can directly see in the detector. 

There are 2 possibilities of using such a function in the current code:
  - true energy - dependent: `FDEventRateAtND_ETrue(ETrue, OAPos)`
    ```bash
    double FDEventRateAtND_ETrue(FunctionCache& cache, double Etrue, double OAPos)
    ```
    This function is reading the values of the FDEventRateAtND function for different true neutrino energy bins (or ranges) from an output file called `Splines_FDEventRateAtND_ETrue.root`. This root file is in `data/step3/`; it is created by `macros/step3_etrim/EffFitFunctionCorrectEbinningSelectedEventsSplineETrue.C`. The `Splines_FDEventRateAtND_ETrue.root` gives the value of this probability over the entire off-axis range of interest for fixed true neutrino energy bins. The bin edges corresponding to these bins, `kTrueEBinEdges` are also saved in the file (i.e `f_norm_etrue1` is the proability of FD event rate at the ND for neutrino energy ranging from bin edge0 to bin edge 1 etc)

    The function can further be used and called within the code as
    ```bash
    FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos)`
    ```
     where
    ```bash
    FunctionCache cacheEtrue;
    cacheEtrue.loadFunctions("Splines_FDEventRateAtND_ETrue.root", FunctionType::ETrue);
    ```
    and `EnuTrue[i_iwritten]` is the true neutrino energy of the event and `OAPos` is the corresponding off-axis position within the ND.

    -> use this function if you need to cross check any miss-match in the final result is not due to this step of the analysis.
    
  - hadron energy and lepton energy `FDEventRateAtND(Ehad, Elep, OAPos)` : 1 given neutrino event has a specific hadron energy deposited inside the detector and a given muon energy; this can be further used in order to define this event rate function for a fixed hadron energy and muon energy
    ```bash
    double FDEventRateAtND(FunctionCache& cache, double Ehad, double Emu, double OAPos)
    ```
    Since we use the ND selected events to obtain this FD Event Rate at the ND probability, and since we are interesed in how a given FD event would be seen by the ND, the `Ehad` used here should be the hadronic energy deposited inside the ND-LAr active volume -> so `Ehad` here is in fact our trimmed energy `Etrim`.

    This function is reading the value of the FDEventRateAtND function for different - fixed `Ehad` and `Elep` bins over the entire off-axis range, from an ouput file called `Splines_FDEventRateAtND.root`. This root file is in `data/step3/`; it is created by `macros/step3_etrim/EffFitFunctionCorrectEbinningSelectedEventsSplineEhadElep.C`. The `Splines_FDEventRateAtND.root` gives the value of this probability over the entire off-axis range of interest for a fixed hadron energy and lepton energy bin. The bin edges corresponding to these bins, `kHadBinEdges` and `kLepBinEdges` are also saved in the file (i.e `f_norm_lep1_had1` is the proability of FD event rate at the ND for an event with hadronic energy ranging from bin HadBinEdge0 to bin HadBinEdge1 and a muon energy ranging from LepBinEdge0 to LepBinEdge1.. etc)

    The function can further be used and called within the code as
    ```bash
    FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
    ```
    where
    ```bash
    FunctionCache cacheLepHad;
    cacheLepHad.loadFunctions("Splines_FDEventRateAtND.root", FunctionType::LepHad);
    ```
    and ```info.Etrim *1E-3``` is the trimmed energy of a given throw in GeV, `info.Emu*1E-3` is the muon energy in GeV, and `OAPos` is the corresponding off-axis position in the ND.
    
    **This is the function that should be used - if possible all the time in the analysis, rather than the true energy one**. By using Ehad and Elep , which are the deposited energies in the detector (at this point we use true muon energy in the entire analysis simply because we have no muon reconstruction but this would eventually change) we avoid any kind of neutrino interaction model dependency. The probability as a function of true energy should in principle the most correct one and can be used to cross check there is nothing wrong in the analysis regarding this particular step but should in principle be avoided.

    -> once you cross checked (by using the Etrue version of the function) that this step of the analysis is doing what it should (i.e you get very similar results by using the Etrue version and by using the EhadElep version of the function) you should stick to using this Ehad-Elep version!

### Down-weighting muons at low energies: CAF-like weight

In the current ND CAFs there is an additional cut (besides the muon being either contained in ND-LAr or further matched in the tracker) regarding **low energy muons which could not be reconstructed**. This cut is regarding:
    - minimum track length of a muon:  muons have to have more than 1 m long track in order to be reconstructed 
    - too much energy deposited energy per unit length: muons should deposit less than 3 MeV / cm along the track in order to be properly reconstructed 

Since those are not geometric related cuts (rather related to the muon reconstruction / identification itself) we didn't have them in our geometric efficiency approach. In order to account for such a cut (we want to make sure we apply *exactly the same selection* as done in the PRISM analysis using the NDCAFs, the folowing `weightCAFLike` weight is defined. 

```bash
weightCAFLike[i_iwritten] = 1.;
if(muonTrackLength_f < 100. || muonEdep_f/muonTrackLength_f > 3.){
   cout<<"** this event will not be selected!! "<<" ev nr "<<i_iwritten<<" muon e dep: "<<muonEdep_f<<" muon track length: "<<muonTrackLength_f<<endl;
   weightCAFLike[i_iwritten] = 0.;
}
```
-- by accessing the muon track length and energy deposit (saved from the geometric efficiency stage) we define the `weightCAFLike` weight which is equal to 1 for all events except of those with a too short muon track or too much energy deposited / track. Thus by weighting our selected events histogram with this CAF-like weight we corrently account for these additional cut (this selection cut is mainly affect low energy muons, with energies < 300 MeV or so).
 
 ### 3.3  Code structure, loops and histograms 
 
`ThrowInfo` structure is storing all information of the passing throws, on a throw by throw basis: `Etrim` - hadronic energy deposited inside ND-LAr, `Emu` - muon energy, `weightPmuon` muon efficiency (either ND-LAr contained or TMS matcher) of the inidivual throw that passes the hadron geo eff .  

Since each FD event has a number of passing throws , `NPassedThrows`, at a given vtx_x position, the throw information is saved in AllThrowInfo  
```bash
std::vector<std::vector<std::vector<ThrowInfo>>> AllThrowInfo;
```
The looping sequence is very similar to what is done in the hadron and muon geometric efficiency codes:

  1. Loop over all FD Events
    -- this is where variables that have one value / FD event (independent on throws) are accessed and written out: i.e true neutrino energy, true muon neutrino energy etc
    -- within the same loop there are variables that also have one value / FD event but that have been saved for each vtx postion (terrible I know sorry..): for this variables we choose a fixed value of vtx_x (can be whatever value that has been used in the hadron geo eff part) in order to only write them once / event in the output histogram. Those are variables like:
       - `totEnergyFDatND_f` : tot hadronic energy deposited inside FD (this is what we trim from in the ND)
       - `muonEdep_f` : muon deposited energy
       - `muonTrackLength_f` muon track length -> used together with `muonEdep_f` to define the `weightCAFLike`

  2. Loop over all vtx_x:
       - `nPassThrowsPerEvent` : total number of passing throws per FD Event
       - `nPassThrowsPerVtx` : total nr of passing throws per vtxX
      2.1 Loop over the number of passing throws at each vtx_x
         - access and save the throw info, `Etrim`, `Emu` and `weightPmuon ` into the `AllThrowInfo` vector -> saved here and used later on
     
  Up until now we only saved and defined variables and information that we need in the later part of the code. At this point we have everything we need and can go towards the end-goal of obtaining Etrim Vs Off-axis Positions distributions of FD events in the ND (geo eff corrected for):

  - define same energy binning as used in the PRISM analysis (CAFAna) -- currently same binning as for `fine_prism` option in CAFAna (can see different binning options in CAFAna at `lblpwgtools/CAFAna/PRISM/Axes.cxx `
  - define several histograms of interest
  - start looping

1. Loop over all FD Events : `for (Int_t i_iwritten = 0; i_iwritten<nFDEvents; i_iwritten++)`
   - define histograms that will be saved as per FD event:  most of the histograms of interest are per 1 FD event, i.e we want to see different distribution of a FD event in the ND after being put at several vtx_x and detPos
       - `SelectedEventsVsOAPosVsTotalETrim[i_iwritten]` : this is the most important histogram, showing the **2D distribution (off-axis vs VisEtrim) of a FD event as seen by the ND**, capturing the geometric effects of both hadron and muon selection efficiencies in the ND. The histogram is saved as per 1 FD event, but has a significant number of entries, as it is composed by all passing throws (at all vtx_x positions and detector positions )-- in principle it should have something like nvtXPositionsUsed*nDetPos * TotalPassingThrows entries / event 
       - `AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]` : this is also a very important histogram, showing the  **2D distribution (off-axis vs VisEtrim) of a FD event as seen by the ND** , as if all throws would pass i.e 100% efficiency. this histogram is basically containing all the events in `SelectedEventsVsOAPosVsTotalETrim[i_iwritten]` but is weighted so that the total number of entries is equal to the total number of throws -> this is done just so one can have some *diagnosis efficiency plot* in the end by dividing the 2 histograms: `SelectedEventsVsOAPosVsTotalETrim[i_iwritten]/AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]`

       - `HistEtrimAllVtxXTimesCoeff[i_iwritten]`:  this is a histogram showing the trimmed energy distribution (entries vs Etrim) of a FD event when translated to the ND - no FD event rate at ND applied
           - one can apply the linear combination coefficients to such a histogram (see below)
       - `HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]`: this is a histogram showing the trimmed energy distribution (entries vs Etrim) of a FD event when translated to the ND - with FD event rate at ND applied
           - one can apply the linear combination coefficients to such a histogram, but you shouldn't-- once you applied the FD event rate at ND applying the coefficients further on is wrong (see below)
       
       Will come back with additional info about these histos

   - define plots that will be saved as per FD event:  
       - `PlotCombinedEfficiencyVsVtxX[i_iwritten]` -- plot containing combined (hadron and muon) efficiency of a FD event at the ND as a function of its vtx_x position inside the ND
       - `PlotEfficiencyVsVtxX[i_iwritten]` -- plot containgly ONLY hadron geom efficiency of a FD event at the ND as a function of its vtx_x position inside the ND
       - `PlotMuonEfficiencyVsVtxX[i_iwritten]` -- plot containing ONLY muon efficiency (contained in ND-LAr || tracker matched) -> temporarily commented to save memory and storage for the end-result output files but can uncomment if interested in how this looks
       - `PlotMuTrackerEfficiencyVsVtxX[i_iwritten]` -- plot containing ONLY muon tracker efficiency -> temporarily commented to save memory and storage for the end-result output files but can uncomment if interested in how this looks
    
  2. Loop over vtx_x positions: `for (Double_t i_ND_LAr_vtx_pos: a_ND_vtx_vx_vec)`
     - define histograms that have information as per 1FD event per vtxX: these are in principle histograms containing throws related information (remeber: many throws at a fixed vtx_x position, fixed det_pos / FD Event)
     - fill in plots only dependent on vtxX (i.e everything in terms of value vs vtxX, where no PerThrow info is needed)
     - access the **throw structure**: `const auto& throwList = AllThrowInfo[i_iwritten][i_vtxX_plot];`

  As already mentioned above, the current code assumes equal efficiency at all detector positions. This is why we add the desired positions as part of this code and then assume the same efficiency at the same vtx_x value inside the ND, regardless where the ND is positioned. This is what is done further on, where the loop over the detector positions starts (and is within the vtx_x loop)

  3. Loop over detector positions: `for (Double_t i_ND_LAr_dtctr_pos: a_ND_off_axis_pos_vec)`

     Once the detector position is set we can defined the off-axis position: `OAPos = ND_LAr_vtx_pos/100.0 + a_ND_off_axis_pos_vec[i_detpos-1];`
     - define and access histograms and plots that are related to 1 FD event info / vtxX / detPos or 1FD event / OAPos
       - HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1] : here we define 1 histogram / per FD event / per vtxX /per detPos containing the Etrim distribution of all passing throws; no FD event rate at ND will be applied here
       - HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]:  here we define 1 histogram / per FD event / per vtxX /per detPos containing the Etrim distribution of all passing throws; we apply FD event rate at ND 
     - access the coefficients at the correponding OAPos : ` CoefficientsAtOAPos = CoefficientsHist->GetBinContent(CoefficientsHist->FindBin(OAPos));`
     - define the weight that accounts for different (non-flat) distribution of events at different detector positions: `WeightEventsAtOaPos = HistOAPos[i_iwritten]->GetBinContent(HistOAPos[i_iwritten]->FindBin(OAPos));`

  4. Loop over the passing (hadron geo eff passing) throws: `for (const auto& info : throwList)`

      Here we want to access throw-specific information, i.e trim energy, muon energy, muon efficiency
      Fill in the end-result histograms with this info:
      ```bash
      SelectedEventsVsOAPosVsTotalETrim[i_iwritten]->Fill((info.Etrim + info.Emu)/1000 ,OAPos, info.weightPmuon* 1.0/WeightEventsAtOaPos  * weightCAFLike[i_iwritten] * FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos));//FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
      ```
      This histograms will have the total number of entries = totalNumberOfPassingThrows * nvtxX positions * detector positions -- remember totalNumberOfPassingThrows is going to be the sum of passing throws at different vtxX positions
     
      you can see we fill the histogram (again this is per 1 FD Event, but it has in fact information from each vtx_x, detPos and passing throw) with the VisEtrim (= Etrim + Emu) in Gev, OAPos, and we then apply the weights:
     
        - `info.weightPmuon` : muon efficiency of this throw passing the hadron geo eff cut
        - `1.0/WeightEventsAtOaPos` : downweight events to account for several detector positions overlapping, i.e more sampling of several off-axis positions
        - `weightCAFLike[i_iwritten] ` : weight to account for very low energy muons (with very short tracks or too much energy depositions) not being selected in the ND
        - `FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos)` : FD event rate at ND
     In the current version the FD event rate at the ND as a function of true neutrino energy has been used. this is because I wanted to cross-check the results for this case and compare them with the Ehad-Elep FD event rate at ND function. At least as far as I could see the results don't differ very much, so my adivce is to use the Ehad-Elep version. To do so comment out `FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos))` and use `FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos)` instead

      ```bash
      AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]->Fill((info.Etrim + info.Emu)/1000 , OAPos, double(validThrows)/throwList.size()* 1.0/WeightEventsAtOaPos * FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos));// FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
      ```
      The weights here are the same as in the SelectedEvents histogram case, with the additional wait of `double(validThrows)/throwList.size()`. Since we want to have here *All Throws*, i.e in principle the events before any selection has been applied, we would want this histogram to have the total number of entries equal to the total number of throws (in fact it would be equal to total number of throws * nvtxX positions * detector positions). This is why add this additonal weight, `(validThrows)/throwList.size()` -- i.e by default for each fixed vtx_x and detPos we have `throwList.size()`, which is equal to the nr of passing throws at that vtx_x . Since we would want to have the total number of throws, i.e `(validThrows)` then we add this weight. You might also notice the lack of the `weightCAFLike[i_iwritten]` and `info.weightPmuon` weights, which is also on purpose since we don't want to apply any selections.

Be careful though, numerically the efficiency is going to be correct if we divide these 2 histograms. However since we only have information (i.e `Etrim`) from the throws that pass the hadron geo eff selcetion and not from every single throw, the energy dependence of this efficiency would not be 100% correct. This is because some of the throws that have not passed the hadron geo efficiency cut will for sure have a different `Etrim` value than those that passed. By creating the histogram the way we do here, we assume all the throws that didn't pass would have had the same `Etrim` values as those that passed. Since this histogram is in any case not used in the final analysis and is rather saved as a mean of cross checking things I don't think this is going to be significant issue. If you would want to be 100% correct then you would need to save all throw information from the hadron geo efficiency part. This shouldn't be too difficult to do but will for sure increase your computational needs.
     

In the same loop there are 2 additional histograms that are filled in and that you may need to access at some point (we don't have to currently but just in case, for cross-check etc). These two histograms are 1D histos and saving basically the same inofrmation, i.e the distribution of a FD event when translated to the ND, with hadron and muon geometric efficiency corrected for, as a function of *visible trimmed Energy (i.e total amount of hadronic energy deposited inside ND-LAr active volume + true muon energy* only. 
```bash
HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Fill(info.Etrim + info.Emu , info.weightPmuon * weightCAFLike[i_iwritten]);
HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Fill(info.Etrim + info.Emu , info.weightPmuon  * weightCAFLike[i_iwritten] * FDEventRateAtND_ETrue(cacheEtrue, EnuTrue[i_iwritten], OAPos));//FDEventRateAtND(cacheLepHad, info.Etrim *1E-3 , info.Emu*1E-3, OAPos));
```
Notice that in one of the histograms the FD event rate is applied, while in the other one it isn't. This is because I used them to make a point that applying both the FD event rate and the coefficients gives us wrong results and to make a point that either one or the other is needed. So I would recommend, if you want to make a point, to try to apply first linear combination coefficients to `HistEtrimDetPosNoFDEventRate` and compare it with `HistEtrimDetPosWithFDEventRate` without coefficients applied. You can then run again and apply the coefficients to `HistEtrimDetPosWithFDEventRate` and show doing both coefficients and FD event rate results in very bad match - because you are in principle accounting for the rate at which neutrinos with a given energy in the FD appear in the ND twice.

You can see that these histograms are per FD event/ per vtx_x /per detector position and filled for each passing throw. Which means they will each have a nr of Passing throws /FDEvent / vtx_x/ detpos entries.

Once we *exit the passing throws loop* these histograms are further scaled to the total number of `validThrows` so that the integral of each histogram would be equal to the geometric efficiency of this FD event in the ND at the given vtx_x position and detpos. i.e this one FD event would be seen as a 0.2 events in the ND at vtx_x and detpos, with this Etrim distribution. 
```bash
HistEtrimDetPosNoFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Scale(1.0/validThrows );//* 1.0/WeightEventsAtOaPos);// * CoefficientsAtOAPos * 1.0/WeightEventsAtOaPos);
HistEtrimDetPosWithFDEventRate[i_iwritten][i_vtxX_plot-1][i_detpos-1]->Scale(1.0/validThrows );//* 1.0/WeightEventsAtOaPos);// * CoefficientsAtOAPos * 1.0/WeightEventsAtOaPos);
```
Since these histograms are per vtx_x and per detPos applying the `WeightEventsAtOaPos` is not needed here, unless you apply the coefficients, which are function of off-axis positions - we want to apply this weight only when looking at events *everywhere -- over all off-axis positions*  in the ND, not at a fixed point only.  Currently in the code that *coefficients* part is commented out, but if you want to applyu the off-axis coefficients (again if you apply the FD event rate at the ND applying the coefficents is wrong) you can uncomment the `* CoefficientsAtOAPos * 1.0/WeightEventsAtOaPos);` part.

As these hisotgrams are per FD Event / per vtx_x / per det pos (you can save them individually as well to cross check. however since by doing so we would have many more histograms in the output file, so much larger size and much slower computational process this is not done in the current verion), and we would want to have in the end the distribution of 1 FD event as a functio of E trim *everywhere in the ND*, at all detector positions and vtx_x positions inside the detector, i.e at all off-axis positions of interest, we save the sum of such hsitograms over the entire detector positions and vtx_x positions used: `HistEtrimAllVtxXTimesCoeff[i_iwritten]` respectively `HistEtrimAllVtxXTimesCoeffWithFDEvRate[i_iwritten]`. So now those histograms show the trimmed energy distribution of 1 FD event when translated to the ND and when the ND is moved at several off-axis positions.

The way the two histograms are currently in the code and still written to the output root file (no coeffs applied) are for visual purposes, in case you would want to see how does your event look in the FD as a function of trimmed energy. However you can obtain this info fom the main used histo `SelectedEventsVsOAPosVsTotalETrim`

 ### 3.4 Output histograms

 The results are saved in an output root file, called either `FileWithHistEtrim_MuAndHaddEff_VisEtrim_FDEvRateAtND_NDFV4m_NoOsc_NoCoeffsApplied_2DHistosWithSelectedAndThrownEvents_WithCAFLikeMuCut.root` or `FileWithHistEtrim_MuAndHaddEff_VisEtrim_WithOscProb_FDEvRateAtND_NDFV4m_2DHistosWithSelectedAndThrownEvents_WithCAFLikeMuCut.root` depending if the with or without oscillations code was saved. The resulting output file will contain results for the same amount of FD events as stored in the files obtained from the hadron and muon geo eff, which are used as input for running the code. The following are saved:
   - `SelectedEventsTwoDHisto_FDEvt_` -- as already mentioned several times, the most important histogram, containing the 2D distribution of a FD event, as seen by the ND (with hadron and muon efficiency applied), as a function of the off-axis position and visible trimmed energy (again VisEtrim = Etrim_had + true muon energy : the total amount of hadronic energy deposited in ND-LAr active volume + true muon energy)
   - `AllThrownEventsVsOAPosVsTotalETrim_FDEvt_` : contains the 2D distribution of all throws of a FD event when translated to the ND -> can be further used to get some proxy for the efficiency of FD events in the ND   
   - `HistEtrimPmuWeightedAllVtxXTimesCoeff_FDEvRateAtND_FDEvt_` - this is the `HistEtrimAllVtxXTimesCoeffWithFDEvRate` histogram. the way the code is right now this is nothing else but the 1D distribution of a FD event as seen by the ND (with hadron and muon efficiency applied) as a function of visible trimmed energy and with the FD event rate at the ND applied --> it is basically the 1D version of the above 2D histogram with SelectedEvents.  No coefficients are applied to this histo in the current code, name is confusing sorry. but as mentioned you can always apply the coefficients to this histogram if needed
   - `HistEtrimPmuWeightedAllVtxXTimesCoeff_NoFDEvRate_FDEvt_` - this is the `HistEtrimAllVtxXTimesCoeff` histogram: same 1 D distribution of a FD event as seen by the ND as a function of visible trimmed energy, but no FD event rate at ND applied.

   - `GraphEffficiency_FDEventNr_ ` - plot with hadron geo eff of the FD event vs vtx_x position at a fixed detpos (since we consider efficiency is the same at each detector position it doesn't matter which detpos this is for)
   - `GraphEffficiency_AllNDDetPos_FDEventNr_` - plot with hadron geo eff of the FD event vs off-axis positions: this is a nice plot to visualize how we basically replicate the same efficiency for each detpos used in this analysis
   - `GraphCombinedEffficiency_FDEventNr_` - plot with combined (muon and hadron) of the FD event vs vtx_x position at a fixed detpos
     
   - `hist_FDTotEnergy` - histogram with total amount of hadronic energy of the FD events in the FD
   - `hist_muEdep` - histogram with muon deposited energy
   - `hist_muTrackLength` - histogram with muon track length
   - `LepMomTot` - histogram with true muon energy
   - `hist_EnuFDEnergy` - histogram with true FD neutrino energy spectrum (always unoscillated)
   - `hist_visEnuFDEnergy` - histogram with visible energy (from FD events)
     
   If the oscillated version is run there will also be:
   - `Oschist_EnuFDEnergy` - histogram with oscillated FD neutrino energy spectrum

 ### 3.5 Remarks about the oscillated spectrum

The oscillation in this analysis is applied directly to the FD spectrum. I.e we *take the events from a FD oscillated spectrum* -- that being said the idea here is we have access to whatever we observed in the FD. So we then take the events from the FD oscillated spectrum and translate them to the ND etc etc.

The oscillation probability is applied only at this stage - i.e stage 3 of the analysis. This means that *you can use the same output from the hadron and muon geometric efficiency for both oscillated and unoscillated procedure*. Depending which code you are running, either `./NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND` or `NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND` you run the analysis for an oscillated FD spectrum or a non-oscillated one. 

Specific headers used in the *with oscillations* code are:
```bash
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/OscCalcPMNSOpt.h"
```
  - the oscillation object, `IOscCalc` is the same as used within CAFAna and is defined in the code as `osc::IOscCalc* calc = ana::DefaultOscCalc();` -> you can check `lblpwgtools/CAFAna/Analysis/Calcs.cxx` (the same file used by CAFAna/PRISM) for the default oscillation parameters values.
  - you can also change the oscillation parameters by using the `calc->SetTh23()` function for theta23 parameters etc. -- functions also defined in `lblpwgtools/CAFAna/Analysis/Calcs.cxx`


The oscillation itself is done by applying the **oscillation probability** to the FD events:
```bash
calc->P(14,14,ND_Gen_numu_E)
```
this is the oscillation probability -- actually survival probability -- for muon to muon neutrino (first 2 numbers represent the neutrino species, from 14 to 14 means from muon neutrino to muon neutrino) of events with neutrino energy `ND_Gen_numu_E`. This neutrino energy, `ND_Gen_numu_E` is the true neutrino energy as take from the input FD ntuples.

You can notice that the oscillation proability is applied to FD events and the corresping spectrum of oscillated neutrino spectrum at FD is saved in the `Oschist_EnuFDEnergy` . I recommend to always save this histogram as this should be one of the first cross checks with the PRISM analysis: make sure same oscillation parameters are used.

Further on the oscillation probability (function of neutrino energy of the FD event) is also applied to the final distribution of a given FD event in the ND: `SelectedEventsTwoDHisto_FDEvt_` , `HistEtrimPmuWeightedAllVtxXTimesCoeff_FDEvRateAtND_FDEvt_` etc:
```bash
HistEtrimAllVtxXTimesCoeffWithFDEvRateOscillated[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));

SelectedEventsVsOAPosVsTotalETrim[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
AllThrownEventsVsOAPosVsTotalETrim[i_iwritten]->Scale(calc->P(14,14,EnuTrue[i_iwritten]));
```
Since each FD event can be represented in a 2D or 1D distribution in the ND, and since we save and care about this ditributions anyways, we can then apply the corresponding oscillation probability directly to these distributions: i.e if instead of 1 FD event (non-oscillated case) now I have 0.3 FD events (30% oscillation probability), the ND event would still see this FD event the same (energy of this event doesn't change, so ND efficiency also doesn't change). So all we need to do is just scale down this 1 FD event to 0.3 FD events so it is able to have a 1 to 1 comparison to the FD spectrum.

NOTE: always make sure to use the same oscillation parameters in the geometric efficiency and in the PRISM prediction you want to compare to!

## 4. Obtain the distribution of ALL FD events, as seen (geo eff corrected for) by the ND

To sum up the results of all FD events (from all FD ntuples), you can use the `macros/step3_etrim/SumEtrimHistos.C` code. To run this code do:
```bash
SumEtrimHistos("/pnfs/dune/scratch/users/icaracas/EtrimAnalysis_FromFDGeoEffinND/CombinedEffWithCAFLikeMuCuts/AllOAPosEtrueProb/OscSpectrumNonNegativeEvRateVals", {"hist_visEnuFDEnergy", "hist_EnuFDEnergy","Oschist_EnuFDEnergy", "LepMomTot", "hist_FDTotEnergy", "hist_muEdep", "hist_muTrackLength", "AllThrownEventsVsOAPosVsTotalETrim_FDEvt_", "SelectedEventsTwoDHisto_FDEvt_"}
```

- where the first parameter is the path to your ouptut root files obtained at the last stage (after step 4) of the analysis.
- the following parameters are the names of the histograms / distributions in the output file that you wish to sum together

  You can put any of the histograms obtained from the Etrim analysis step 3 here. The absolutely mandatory one is `SelectedEventsTwoDHisto_FDEvt_` as this is the end 2D distribution of FD events in the ND. I also highly recommend saving the FD true neutrino energy spectrum, i.e `hist_EnuFDEnergy` will be the unoscillated spectrum in the FD and `Oschist_EnuFDEnergy` the oscillated spectrum in the FD. If you run for the non-oscillated case then you will have no `Oschist_EnuFDEnergy` histogram so don't add it as a parameter.

- the output file, NAMEOFOUTPUT file will then contain the final distributions (i.e true neutrino energy in FD, 2D distribution of all FD Events
when translated to the ND and geometrically corrected for, as a function of VisEtrim and off-axis position etc) of all the neutrino events
in a given FD spectrum.


## 5. Compare the distribution of FD events as seen by the ND (hadron + muon geometrically corrected for) with the Linearly Combined selected events in the ND.

If everything works well in the analysis, then **the spectrum of the FD events translated in the ND and geometrically corrected for, with the ND being moved in exactly the same off-axis detector positions (as in the ND-CAFs) and accounting for the FD event rate at the ND, should be identical with the linearly combined spectrum of selected ND events**. A sketch of the full approach (with the end desired match between ND linearly combined data and the FD events as seen by the ND highlighted with the red circle) is shown below. 

<img width="1437" height="764" alt="image" src="https://github.com/user-attachments/assets/75efcdd1-db47-4607-b0b3-56aa9dcf612a" />


NOTE: in the ND case we apply the off-axis coefficients to reproduce a desired *oscillated* or *non-oscillated* FD spectrum. In the case of the FD events translated to the ND, these are events already taken from the FD *oscillated* or *non-oscillated* spectrum, which is why applying the linear combination coefficients to these events is not correct (we already account for how often 1FD event appears in the ND for different ND positions by using the FD events rate function. Already discussed this in more details above). Howevere this is why it is important to make sure the same oscillation parameters are used in both cases of interest. This is why it's also always good to save the FD true neutrino spectrum (oscillated or non-oscillated) and further compare it with the true neutrino energy spectrum obtained from CAFAna-PRISM stage.  

The code that does this comparison, using the end results obtained from the analysis so far, `macros/step3_etrim/PlotAllOAPosWithCAFLikeCutClean.C`. The code uses 2 input files:

1. The file obtained using the standard ND-CAFs, `PRISMPred_ERecFromDepWithNeutronEnergyAndTrueEmu_NoSysts_NoOsc_FlatRunPlan_AllOAPos_correcPerFileWeight_NDEffVsEreco.root`
respectively PUT THE NAME OF WITH OSCILLATIONS THE FILE HERE) for the "oscillated" case comparison.

  These PRISMPred files are obtained by using the classic CAFAna framework used in the PRISM analysis. The main differences (from the standard case) are    related to the analysis variable: we need to use the same energy variable for a proper comparison. This means we need to use the hadron energy deposits in the ND + true muon energy. (in the case of PRISM oscillation analysis the smeared reconstructed energy is used.) To do so you should **use the `ERec` variable in CAFAna** . This is defined in `lblpwgtools/CAFAna/Core/SpectrumLoader.cxx` as:

```bash
sr.eRec_FromDep = sr.eRecoP + sr.eRecoN + sr.eRecoPip + sr.eRecoPim +
                  sr.eRecoPi0 + sr.eRecoOther + sr.LepE;
```

  The main histograms read from the PRISMPred file:
  
   - `PRISMDataTimesCoeff` : The linearly combined ND data (after ND background subtraction) as a function of energy deposited inside the ND, (in this case true muon E + hadron energy deposits), which is what we want to be able to reproduce with the FD events translated to the ND (and geo eff corrected for); i.e the blue line in the sketch above
   - `FDTargetSpectrumETrue`: this is the FD target spectrum, in true neutrino energy, that we want to reproduce with PRISM (this should always look the same as the `Oschist_EnuFDEnergy` or `hist_EnuFDEnergy` --depending on the oscillated or non-oscillated study case-- from the GeoEff side.) - this is how we cross check the same oscillation parameters are used in both cases.
   - `LinCombMatchSpectrumETrue` : this is the linearly combined ND data vs true neutrino energy - can be plot on the same graph with `FDTargetSpectrumETrue` just to see how well we are able to match the spectrum.
   - `PRISMNDEff293` : this is the efficiecny obtained from the PRISM analysis - i.e MC efficiency; by detault in CAFAna this efficiency is as a function of true energy. I tweaked the code a bit to have it as a function of reconstructed (in this case `ERec`) energy. in any case this is not directly used in the code but mainly used as a comparison with what we get from the Geo eff side. NOTE: since this efficiency is for ND data only (i,e no coefficients applied) you can only use it as a cross check for the non-oscillated, and even there is mostly as a proxy
  
2. The file obtained from translating ALL FD events to the ND (muon and hadron geo eff corrected for) : `NAMEOFTHEFILEFROMSUMETRIMHISTOS` -- this is the file obtained after having summed all Etrim output root files, the file obtained at step 4. Depending on the case of interest, one should use either the results from the oscillated or non-oscillated study case.

The most important histograms read and used from this file are:
   - `SelectedEvGeoEff` this is the 2D distribution of ALL FD events selected in the ND and geo eff corrected for (accounting for FD event rate at ND)  -> this is our end result distribution of FD events observed in the ND (magenta line in the sketch above) and this is what we want to match the ND linear combination mentioned above
   - `AllThrowsGeoEff` this is the 2D distribution of ALL FD events translated to the ND, no ND selection applied -> this contains all throws in the ND (not only the passing ones). for more details check the `AllThrownEventsVsOAPosVsTotalETrim_FDEvt_0` histogram details from step 3.
   - `AllGenEvGeoEffTrueE` this is the FD spectrum, in true neutrino energy, from which we take the events and put them in the ND.
     
NOTE: you should also make sure the same oscillation parameters have been used both in PRISMPred and in the Geometric Efficiency case (remember you set
the oscillation parameters via `lblpwgtools/CAFAna/Analysis/Calcs.cxx` in the Geo Efficiency case , and you can set the oscillation parameters value in the PRISMPredGrid.fcl
file for the PRISM case)

The end goal of this code is to compare the distribution of ALL FD events as seen by the ND (hadron + muon geometrically corrected for),
`SelectedEventsTwoDHisto_FDEvt_` with the ND linearly combined data, as a function of energy deposited inside the ND, `VisEtrim` (= hadronic energy deposited inside ND-LAr acive volume + muone energy). To do so, the 2D distribution of the FD events in the ND, which is a function of off-axis position and visEtrim has to first be projected on the visEtrim axis (i.e we don't care about the off-axis distribution anymore so sum over all off-axis positions). 

### Normalize to POT desired -> normalized to integral for now
If we had the same amount of simulated neutrino events in both ND-CAFs and FD ntuples we would be done at this stage. However since this is not the case we need to normalize so that we can properly compare the 2 distributions. Ideally we should normalize to the POT used in the simulation. We have this information in the ND-CAFs and could easily use it for the linear combination of ND events. However the problem is this POT information is not saved in the FD ntuples that we use a start point of this analysis. -> TODO (re-run more ntuples and save the POT info as well - I have personally not created any ntupled myself so far so I am not sure what is the process for this , perhaps Wei Shi might have more info here)

However in order to be able to have some comparison I am normalizing both of the distribution so that they have the same integral (in the current analysis they are both normalized so that their integral = 1).

Finally we can overlap the 2 distributions and get the final result. You can see that at the current stage we get a very nice match for the non-oscillated spectrum, but not a satisfying match for the non-oscillated spectrum. 
<p align="center">
  <img src="https://github.com/user-attachments/assets/ffcaf119-9e13-40a9-9927-1506508a609b" width="45%">
  <img src="https://github.com/user-attachments/assets/6377002e-90a8-4210-837c-11c12c3a9e26" width="47%">
</p>

The **Documentation** part itself ends here . Below I am giving just some ideas that I think still need to be done / tried / tested / going on in order to manage to get a good match for the oscillated scenario as well. Feel free to dicuss / share / update this section :) 

----
## TO DOs:






