import ROOT
import ctypes
ROOT.gROOT.SetBatch()

variables = ["ss2th13", "dmsq32", "ssth23", "dcp"]

def flip(h):
    hf = ROOT.TH2D( "flipped_%s" % h.GetName(), ";%s;%s" % (h.GetYaxis().GetTitle(), h.GetXaxis().GetTitle()), h.GetYaxis().GetNbins(), h.GetYaxis().GetBinLowEdge(1), h.GetYaxis().GetBinLowEdge(h.GetYaxis().GetNbins()+1), h.GetXaxis().GetNbins(), h.GetXaxis().GetBinLowEdge(1), h.GetXaxis().GetBinLowEdge(h.GetXaxis().GetNbins()+1) )

    for bx in range( 1, h.GetXaxis().GetNbins()+1 ):
        for by in range( 1, h.GetYaxis().GetNbins()+1 ):
            hf.SetBinContent( by, bx, h.GetBinContent(bx,by) )
    return hf

def retitle(tit):
    return tit[9:]

if __name__ == "__main__":

    tf100 = ROOT.TFile( "bf_100ktMWyrs_dcpMinusPiOver4.root" )
    tf1000 = ROOT.TFile( "bf_1000ktMWyrs_dcpMinusPiOver4.root" )

    h1D100 = [None for v in variables]
    h1D1000 = [None for v in variables]
    for v in range(3):
        tmpc = tf100.Get( "PFdcp_vs_PF%s" % variables[v] )
        h1D100[v] = tmpc.ProjectionX("a%d"%v).Clone( "%s_1D_100" % variables[v] )

        tmpm = tf1000.Get( "PFdcp_vs_PF%s" % variables[v] )
        h1D1000[v] = tmpm.ProjectionX("b%d"%v).Clone( "%s_1D_1000" % variables[v] )
    tmpdc = tf100.Get( "PFdcp_vs_PFss2th13" )
    h1D100[3] = tmpdc.ProjectionY("a4").Clone( "dcp_1D_100" )
    tmpdm = tf1000.Get( "PFdcp_vs_PFss2th13" )
    h1D1000[3] = tmpdm.ProjectionY("b4").Clone( "dcp_1D_1000" )

    c = ROOT.TCanvas("c", "c", 1000, 1000)

    c.SetRightMargin(0.05)
    c.SetLeftMargin(0.2)
    c.SetBottomMargin(0.3)
    c.SetTopMargin(0.05)
    c.Divide(4,4,0.,0.)


    # now the 100 side 2Ds
    for x in range(4):
        for y in range(x+1,4):
            c.cd(4*y+x+1)
            ROOT.gPad.SetTopMargin(0.0001)
            if 4*y+x+1 >= 13:
                ROOT.gPad.SetBottomMargin(0.2)
            else:
                ROOT.gPad.SetBottomMargin(0.0001)

            h2 = tf100.Get( "PF%s_vs_PF%s" % (variables[y],variables[x]) )

            h2.GetXaxis().SetTitle(retitle(h2.GetXaxis().GetTitle()))
            h2.GetYaxis().SetTitle(retitle(h2.GetYaxis().GetTitle()))
            if x == 0 and y == 3:
                h2.GetXaxis().SetTitleSize(0.08)
                h2.GetXaxis().SetLabelSize(0.06)
                h2.GetYaxis().SetTitleSize(0.08)
                h2.GetYaxis().SetLabelSize(0.06)
                h2.GetXaxis().SetTitleOffset(1.)
                h2.GetYaxis().SetTitleOffset(1.2)
            else:
                h2.GetXaxis().SetTitleSize(0.10)
                h2.GetXaxis().SetLabelSize(0.07)
                h2.GetYaxis().SetTitleSize(0.10)
                h2.GetYaxis().SetLabelSize(0.07)
                h2.GetXaxis().SetTitleOffset(0.8)
                h2.GetYaxis().SetTitleOffset(1.)
            h2.GetXaxis().CenterTitle()
            h2.GetYaxis().CenterTitle()
            h2.GetXaxis().SetNdivisions(505)
            h2.GetYaxis().SetNdivisions(505)
            h2.GetZaxis().SetNdivisions(0)

            h2.Draw("COLZ")

    # now the 1000 side 2Ds
    fh2 = [None for j in range(6)] # keep them in scope?
    j = 0
    for y in range(4):
        for x in range(y+1,4):
            c.cd(4*y+x+1)
            ROOT.gPad.SetTopMargin(0.0001)
            ROOT.gPad.SetBottomMargin(0.0001)


            h2 = tf1000.Get( "PF%s_vs_PF%s" % (variables[x],variables[y]) )
            fh2[j] = flip(h2)
            fh2[j].GetXaxis().SetNdivisions(505)
            fh2[j].GetYaxis().SetNdivisions(505)
            fh2[j].GetZaxis().SetNdivisions(0)
            fh2[j].Draw("COLZ")
            j += 1

    # draw the diagonals
    for d in range(4):
        c.cd(1+d*5)
        ROOT.gPad.SetTopMargin(0.0001)
        if 1+d*5 >= 13:
            ROOT.gPad.SetBottomMargin(0.2)
        else:
            ROOT.gPad.SetBottomMargin(0.0001)
        h1D1000[d].Scale( 1. / h1D1000[d].Integral() )

        h1D1000[d].GetXaxis().SetTitle(retitle(h1D1000[d].GetXaxis().GetTitle()))
        h1D1000[d].GetXaxis().SetTitleSize(0.10)
        h1D1000[d].GetXaxis().SetLabelSize(0.07)
        h1D1000[d].GetXaxis().SetTitleOffset(0.8)
        h1D1000[d].GetYaxis().SetLabelSize(0.)
        h1D1000[d].GetYaxis().SetLabelOffset(999.)
        h1D1000[d].GetXaxis().CenterTitle()
        h1D1000[d].GetXaxis().SetNdivisions(505)
        h1D1000[d].GetYaxis().SetNdivisions(0)

        h1D1000[d].Draw("hist")
        h1D100[d].SetLineColor(ROOT.kRed)
        h1D100[d].Scale( 1. / h1D100[d].Integral() )
        h1D100[d].Draw("hist same")

        if d == 0:
            c.cd(1)
            ax = ROOT.TGaxis(0.05, 0., 0.05, h1D1000[d].GetMaximum()*1.05, 0.05, 0.15, 505, "")
            ax.SetLabelSize(0.07)
            ax.SetTitle( "sin^{2}2#theta_{13}" )
            ax.SetTitleSize(0.1)
            ax.CenterTitle()
            ax.Draw()

    c.Print( "SinglePoint_NH_upper_minusPiOver4.png" )
    c.Print( "SinglePoint_NH_upper_minusPiOver4.pdf" )
