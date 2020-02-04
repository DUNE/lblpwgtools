
# This is a Python version of the standard rootlogon.C code

# To be used with Python3.X with ROOT libs
# Can be added to one's Python code by importing it:
# from /path/to/rootlogon import *

import array
from ROOT import TColor, TLatex, TStyle, gROOT, TGaxis


def novaplotstyle():
    print("\nWelcome to the ROOT of all evils")

    novaStyle = TStyle("novaStyle", "NOvA Style")

    # Center title
    novaStyle.SetTitleAlign(22)
    novaStyle.SetTitleX(.5)
    novaStyle.SetTitleY(.95)
    novaStyle.SetTitleBorderSize(0)

    # Remove stats box
    novaStyle.SetOptStat(0)

    # Set background color to white
    novaStyle.SetFillColor(10)
    novaStyle.SetFrameFillColor(10)
    novaStyle.SetCanvasColor(10)
    novaStyle.SetPadColor(10)
    novaStyle.SetTitleFillColor(0)
    novaStyle.SetStatColor(10)

    # No colored frames around plots
    novaStyle.SetFrameBorderMode(0)
    novaStyle.SetCanvasBorderMode(0)
    novaStyle.SetPadBorderMode(0)

    # Set the default line color for a fit function to be red
    novaStyle.SetFuncColor(2)

    # Marker settings
    novaStyle.SetMarkerStyle(20)

    # No border on legends
    novaStyle.SetLegendBorderSize(0)

    # Disabled for violating NOvA style guidelines
    # Scientific notation on axes
    TGaxis.SetMaxDigits(3)

    # Axis titles
    novaStyle.SetTitleSize(.055, "xyz")
    novaStyle.SetTitleOffset(.8, "xyz")
    novaStyle.SetTitleOffset(.9, "y")
    novaStyle.SetTitleSize(.055, "")
    novaStyle.SetTitleOffset(.8, "")

    # Axis labels (numbering)
    novaStyle.SetLabelSize(.04, "xyz")
    novaStyle.SetLabelOffset(.005, "xyz")

    # Set histogram minimum to exactly zero
    novaStyle.SetHistMinimumZero()

    # Thicker lines
    novaStyle.SetHistLineWidth(2)
    novaStyle.SetFrameLineWidth(2)
    novaStyle.SetFuncWidth(2)

    # Set the number of tick marks
    novaStyle.SetNdivisions(506, "xyz")

    # Set tick marks on all sides
    novaStyle.SetPadTickX(1)
    novaStyle.SetPadTickY(1)

    # Fonts
    kNovaFont = 42
    novaStyle.SetStatFont(kNovaFont)
    novaStyle.SetLabelFont(kNovaFont, "xyz")
    novaStyle.SetTitleFont(kNovaFont, "xyz")
    novaStyle.SetTitleFont(kNovaFont, "")

    # Apply same settings to titles
    novaStyle.SetTextFont(kNovaFont)
    novaStyle.SetLegendFont(kNovaFont)

    # Get modified colors for colz
    nCont = 255
    stops = [0.00, 0.34, 0.61, 0.84, 1.00]
    red = [0.00, 0.00, 0.87, 1.00, 0.51]
    green = [0.00, 0.81, 1.00, 0.20, 0.00]
    blue = [0.51, 1.00, 0.12, 0.00, 0.00]
    s = array.array('d', stops)
    r = array.array('d', red)
    g = array.array('d', green)
    b = array.array('d', blue)
    npoints = len(s)
    TColor.CreateGradientColorTable(npoints, s, r, g, b, nCont)
    novaStyle.SetNumberContours(nCont)
    gROOT.SetStyle("novaStyle")
    #gROOT->ForceStyle()


# Add NOvA Preliminary to the top right corner of the plot
# Since it returns a TLatex, one must use this in own code by doing
# prelim = preliminary()
# prelim.Draw()
def preliminary():
    prelim = TLatex(.9, .95, "NO#nuA Preliminary")
    prelim.SetTextColor(4)
    prelim.SetNDC()
    prelim.SetTextSize(2 / 30.)
    prelim.SetTextAlign(32)
    return prelim


# Add NOvA Preliminary on the side
# Since it returns a TLatex, one must use this in own code by doing
# prelim = preliminaryside()
# prelim.Draw()
def preliminaryside():
    prelim = TLatex(.93, .9, "NO#nuA Preliminary")
    prelim.SetTextColor(4)
    prelim.SetNDC()
    prelim.SetTextSize(2 / 30.)
    prelim.SetTextAngle(270)
    prelim.SetTextAlign(12)
    return prelim


# Add NOvA Simulation to the top right corner of the plot
# Since it returns a TLatex, one must use this in own code by doing
# sim = simulation()
# sim.Draw()
def simulation():
    prelim = TLatex(.9, .95, "NO#nuA Simulation")
    prelim.SetTextColor(921)
    prelim.SetNDC()
    prelim.SetTextSize(2 / 30.)
    prelim.SetTextAlign(32)
    return prelim


# Add NOvA Simulation on the side
# Since it returns a TLatex, one must use this in own code by doing
# sim = simulationside()
# sim.Draw()
def simulationside():
    prelim = TLatex(.93, .9, "NO#nuA Simulation")
    prelim.SetTextColor(921)
    prelim.SetNDC()
    prelim.SetTextSize(2 / 30.)
    prelim.SetTextAngle(270)
    prelim.SetTextAlign(12)
    return prelim


# Center titles of all axes
def centertitles(histo):
    histo.GetXaxis().CenterTitle()
    histo.GetYaxis().CenterTitle()
    histo.GetZaxis().CenterTitle()
