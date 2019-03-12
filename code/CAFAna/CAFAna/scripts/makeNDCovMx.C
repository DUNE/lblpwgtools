#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TMatrixD.h"
#include "TVectorT.h"
#include "TCanvas.h"

const int n_Ebins = 22;
const int n_ybins = 7;
double Ebins[23] = { 0., 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 3.75, 4., 4.25, 4.5, 5., 5.5, 6., 7., 8., 10. };
double ybins[8] = { 0., 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0 };

double plbins[29] = { -1., -0.75, -0.5, -0.25, 0., 0.25, 0.5, 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 4., 4.5, 5., 5.5, 6., 7., 8., 9., 10., 10.5 };
double ptbins[17] = { 0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1., 1.2, 1.4, 1.6, 1.8, 2., 2.5, 3., 3.25 };
double hbins[22] = { 0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.2, 1.4, 1.6, 1.8, 2., 2.5, 3., 3.5, 4., 5., 5.25 };

const int nu = 100;

int get1Dbin( int bx, int by )
{
    return (bx-1) * n_ybins + by;
}

void get2Dbins( int b1D, int &eb, int &yb )
{
  eb = ((b1D-1) / n_ybins) + 1;
  yb = ((b1D-1) % n_ybins) + 1;
}

void fix( TMatrixD &cov )
{
  // get rid of super-tiny negative eigenvalues
  TVectorD evals;
  const TMatrixD evecs = cov.EigenVectors( evals );
  TMatrixD evalmat(evals.GetNrows(), evals.GetNrows());
  for( int i = 0; i < evals.GetNrows(); ++i ) {
    evalmat(i, i) = std::max(1e-14, evals[i]);
  }

  TMatrixD evecs_inv(TMatrixD::kTransposed, evecs);
  cov = evecs*evalmat*evecs_inv;
}

void makeNDCovMx()
{

  TRandom3 * rando = new TRandom3(12345);

  // Get acceptance uncertainty histograms
  TFile * tf_AccUnc = new TFile( "/dune/data/users/marshalc/CAFs/mcc11_v3/ND_eff_syst.root" );
  TH2D * hMuUnc = (TH2D*) tf_AccUnc->Get( "unc" );
  TH1D * hHadUnc = (TH1D*) tf_AccUnc->Get( "hunc" );

  TFile * cafFile = new TFile( "/dune/data/users/marshalc/CAFs/mcc11_v3/ND_FHC_CAF.root" );
  TTree * cafTree = (TTree*) cafFile->Get( "cafTree" );

  // CAF variables we need
  double vtx_x, vtx_y, vtx_z, LepE, LepNuAngle, Ehad_veto, Ev, Ev_reco, Elep_reco;
  double eRecoP, eRecoN, eRecoPip, eRecoPim, eRecoPi0;
  double eP, eN, ePip, ePim, ePi0;
  int LepPDG, reco_numu, muon_contained, muon_tracker, reco_q;
  cafTree->SetBranchAddress( "vtx_x", &vtx_x );
  cafTree->SetBranchAddress( "vtx_y", &vtx_y );
  cafTree->SetBranchAddress( "vtx_z", &vtx_z );
  cafTree->SetBranchAddress( "LepE", &LepE );
  cafTree->SetBranchAddress( "LepNuAngle", &LepNuAngle );
  cafTree->SetBranchAddress( "Ehad_veto", &Ehad_veto );
  cafTree->SetBranchAddress( "Ev", &Ev );
  cafTree->SetBranchAddress( "Ev_reco", &Ev_reco );
  cafTree->SetBranchAddress( "Elep_reco", &Elep_reco );
  cafTree->SetBranchAddress( "LepPDG", &LepPDG );
  cafTree->SetBranchAddress( "reco_numu", &reco_numu );
  cafTree->SetBranchAddress( "muon_contained", &muon_contained );
  cafTree->SetBranchAddress( "muon_tracker", &muon_tracker );
  cafTree->SetBranchAddress( "reco_q", &reco_q );
  cafTree->SetBranchAddress( "eRecoP", &eRecoP );
  cafTree->SetBranchAddress( "eRecoN", &eRecoN );
  cafTree->SetBranchAddress( "eRecoPip", &eRecoPip );
  cafTree->SetBranchAddress( "eRecoPim", &eRecoPim );
  cafTree->SetBranchAddress( "eRecoPi0", &eRecoPi0 );
  cafTree->SetBranchAddress( "eP", &eP );
  cafTree->SetBranchAddress( "eN", &eN );
  cafTree->SetBranchAddress( "ePip", &ePip );
  cafTree->SetBranchAddress( "ePim", &ePim );
  cafTree->SetBranchAddress( "ePi0", &ePi0 );

  cafTree->SetBranchStatus( "*", 0 );
  cafTree->SetBranchStatus( "vtx_x", 1 );
  cafTree->SetBranchStatus( "vtx_y", 1 );
  cafTree->SetBranchStatus( "vtx_z", 1 );
  cafTree->SetBranchStatus( "LepPDG", 1 );
  cafTree->SetBranchStatus( "LepE", 1 );
  cafTree->SetBranchStatus( "LepNuAngle", 1 );
  cafTree->SetBranchStatus( "reco_numu", 1 );
  cafTree->SetBranchStatus( "muon_contained", 1 );
  cafTree->SetBranchStatus( "muon_tracker", 1 );
  cafTree->SetBranchStatus( "reco_q", 1 );
  cafTree->SetBranchStatus( "Ehad_veto", 1 );
  cafTree->SetBranchStatus( "Ev", 1 );
  cafTree->SetBranchStatus( "Ev_reco", 1 );
  cafTree->SetBranchStatus( "Elep_reco", 1 );
  cafTree->SetBranchStatus( "eRecoP", 1 );
  cafTree->SetBranchStatus( "eRecoN", 1 );
  cafTree->SetBranchStatus( "eRecoPip", 1 );
  cafTree->SetBranchStatus( "eRecoPim", 1 );
  cafTree->SetBranchStatus( "eRecoPi0", 1 );
  cafTree->SetBranchStatus( "eP", 1 );
  cafTree->SetBranchStatus( "eN", 1 );
  cafTree->SetBranchStatus( "ePip", 1 );
  cafTree->SetBranchStatus( "ePim", 1 );
  cafTree->SetBranchStatus( "ePi0", 1 );

  // Universe histograms in analysis bins
  TH2D * histCV = new TH2D( "histCV", ";Reconstructed E_{#nu};Reconstructed y", n_Ebins, Ebins, n_ybins, ybins );
  TH2D * hists[nu];
  TH2D * histsAccOnly[nu];
  TH2D * histsEscaleOnly[nu];

  // Uncertainties for each universe
  TH2D * muAccThrow[nu];
  TH1D * hAccThrow[nu];
  TF1 * EtotThrow[nu];
  TF1 * EmuThrowLAr[nu];
  TF1 * EmuThrowGAr[nu];
  TF1 * EhadThrow[nu];
  TF1 * EEMThrow[nu];
  TF1 * EneutThrow[nu];
  double EmuRes[nu];
  double EhadRes[nu];
  double EEMRes[nu];
  double EneutRes[nu];

  for( int u = 0; u < nu; ++u ) {
    hists[u] = new TH2D( Form("h%03d", u), ";Reco E_{#nu} (GeV);Reco y", n_Ebins, Ebins, n_ybins, ybins );
    histsAccOnly[u] = new TH2D( Form("hAO%03d", u), ";Reco E_{#nu} (GeV);Reco y", n_Ebins, Ebins, n_ybins, ybins );
    histsEscaleOnly[u] = new TH2D( Form("hEO%03d", u), ";Reco E_{#nu} (GeV);Reco y", n_Ebins, Ebins, n_ybins, ybins );

    muAccThrow[u] = new TH2D( Form("muAccThrow%03d", u), ";Muon p_{L};Muon p_{T}", 28, plbins, 16, ptbins );
    hAccThrow[u] = new TH1D( Form("hAccThrow%03d", u), ";Hadronic energy", 21, hbins );

    // All the energy systematics as TF1 vs. energy
    EtotThrow[u]   = new TF1( Form("Etot%03d", u),   "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );
    EmuThrowLAr[u] = new TF1( Form("EmuLAr%03d", u), "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );
    EmuThrowGAr[u] = new TF1( Form("EmuGAr%03d", u), "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );
    EhadThrow[u]   = new TF1( Form("Ehad%03d", u),   "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );
    EEMThrow[u]    = new TF1( Form("EEM%03d", u),    "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );
    EneutThrow[u]  = new TF1( Form("Eneut%03d", u),  "[0] + [1]*x + [2]*pow(x+0.1,-0.5)", 0., 100. );

    EmuRes[u] = rando->Gaus(0., 0.1);
    EhadRes[u] = rando->Gaus(0., 0.1);
    EEMRes[u] = rando->Gaus(0., 0.1);
    EneutRes[u] = rando->Gaus(0., 0.3);

    // set the parameters
    EtotThrow[u]->SetParameter( 0, rando->Gaus(0., 0.02) );
    EtotThrow[u]->SetParameter( 1, rando->Gaus(0., 0.01) );
    EtotThrow[u]->SetParameter( 2, rando->Gaus(0., 0.02) );

    EmuThrowLAr[u]->SetParameter( 0, rando->Gaus(0., 0.02) );
    EmuThrowLAr[u]->SetParameter( 1, rando->Gaus(0., 0.005) );
    EmuThrowLAr[u]->SetParameter( 2, rando->Gaus(0., 0.02) );

    EmuThrowGAr[u]->SetParameter( 0, rando->Gaus(0., 0.01) );
    EmuThrowGAr[u]->SetParameter( 1, rando->Gaus(0., 0.00001) );
    EmuThrowGAr[u]->SetParameter( 2, rando->Gaus(0., 0.01) );

    EhadThrow[u]->SetParameter( 0, rando->Gaus(0., 0.05) );
    EhadThrow[u]->SetParameter( 1, rando->Gaus(0., 0.05) );
    EhadThrow[u]->SetParameter( 2, rando->Gaus(0., 0.05) );

    EEMThrow[u]->SetParameter( 0, rando->Gaus(0., 0.05) );
    EEMThrow[u]->SetParameter( 1, rando->Gaus(0., 0.05) );
    EEMThrow[u]->SetParameter( 2, rando->Gaus(0., 0.05) );

    EneutThrow[u]->SetParameter( 0, rando->Gaus(0., 0.2) );
    EneutThrow[u]->SetParameter( 1, rando->Gaus(0., 0.3) );
    EneutThrow[u]->SetParameter( 2, rando->Gaus(0., 0.3) );
  }

  // Build throw histograms for acceptance uncertainties        
  // for each bin, throw the uncertainty, as if totally uncorrelated bin to bin
  for( int u = 0; u < nu; ++u ) {
    for( int b = 1; b <= hHadUnc->GetNbinsX(); ++b ) {
      if( hHadUnc->GetBinContent(b) > 0. ) {
        hAccThrow[u]->SetBinContent( b, rando->Gaus(0., hHadUnc->GetBinContent(b)) );
      }
    }
    for( int bx = 1; bx <= hMuUnc->GetNbinsX(); ++bx ) {
      for( int by = 1; by <= hMuUnc->GetNbinsY(); ++by ) {
        if( hMuUnc->GetBinContent(bx, by) > 1.E-6 ) {
          muAccThrow[u]->SetBinContent( bx, by, rando->Gaus(0., hMuUnc->GetBinContent(bx,by)) );
        }
      }
    }
    // for smoothing, set unfilled bins to the value of their neighbors
    for( int bx = 1; bx <= hMuUnc->GetNbinsX(); ++bx ) {
      for( int by = 1; by <= hMuUnc->GetNbinsY(); ++by ) {
        if( hMuUnc->GetBinContent(bx,by) < 1.E-6 ) { // bin not filled
          int near_by = by - 1;
          while( near_by && hMuUnc->GetBinContent(bx, near_by) < 1.E-6 ) --near_by;
          if( !near_by ) {
            int near_bx = bx+1;
            while( hMuUnc->GetBinContent(near_bx, by) < 1.E-6 ) ++near_bx;
            muAccThrow[u]->SetBinContent( bx, by, muAccThrow[u]->GetBinContent(near_bx, by) );
          } else {
            muAccThrow[u]->SetBinContent( bx, by, muAccThrow[u]->GetBinContent(bx, near_by) );
          }
        }
      }
    }
    // Now smooth it, so that it allows any smooth function in the envelope of the uncertainty          
    hAccThrow[u]->Smooth(2);
    muAccThrow[u]->Smooth(1);
  }

  // some validation plots
  TH2D * val_Ev[nu];
  TH2D * val_y[nu];
  for( int u = 0; u < nu; ++u ) {
    val_Ev[u] = new TH2D( Form("val_Ev_%03d",u), ";Reco E_{#nu};Shifted E_{#nu}", 100, 0., 10., 100, 0., 10. );
    val_y[u] = new TH2D( Form("val_y_%03d",u), ";Reco y;Shifted y", 100, 0., 1., 100, 0., 1. );
  }
    
  // Loop over events and fill the analysis bin histograms
  int N = cafTree->GetEntries();
  for( int ii = 0; ii < N; ++ii ) {
    cafTree->GetEntry(ii);

    if( ii % 100000 == 0 ) printf( "Event %d of %d...\n", ii, N );

    // FV cut
    if( abs(vtx_x) > 300. || abs(vtx_y) > 100. || vtx_z < 50. || vtx_z < 350. ) continue;
    // true numu CC cut
    if( LepPDG != 13 ) continue;
    // reco numu CC cut
    bool numuCC = (reco_numu && reco_q == -1 && (muon_contained || muon_tracker));
    if( !numuCC ) continue;

    // determine quantities for acceptance uncertainties, including overflow bins
    double p = sqrt(LepE*LepE - 0.105658*0.105658);
    double pl = p*cos(LepNuAngle);
    if( pl > 10.25 ) pl = 10.25; // overflow
    double pt = p*sin(LepNuAngle);
    if( pt > 3.2 ) pt = 3.2; // overflow
    double ehad = Ev_reco - Elep_reco;
    if( ehad > 5.1 ) ehad = 5.1;

    histCV->Fill( Ev_reco, (Ev_reco-Elep_reco)/Ev_reco, 1. );
    for( int u = 0; u < nu; ++u ) {
      double wgt_mu = 1. + muAccThrow[u]->GetBinContent( muAccThrow[u]->FindBin(pl,pt) );
      double wgt_had = 1. + hAccThrow[u]->GetBinContent( hAccThrow[u]->FindBin(ehad) );

      // determine the shifted energies
      double shiftChargedHad = EhadThrow[u]->Eval(eRecoP + eRecoPip + eRecoPim);
      double shiftEM = EEMThrow[u]->Eval(eRecoPi0);
      double shiftN = EneutThrow[u]->Eval(eRecoN);
      double shiftMu = ( muon_contained ? EmuThrowLAr[u]->Eval(Elep_reco) : EmuThrowGAr[u]->Eval(Elep_reco) );
      double shiftTot = EtotThrow[u]->Eval(Ev_reco - Elep_reco);

      double Ehad_reco_shift = Ev_reco - Elep_reco;
      Ehad_reco_shift += shiftChargedHad*(eRecoP + eRecoPip + eRecoPim);
      Ehad_reco_shift += shiftEM*eRecoPi0;
      Ehad_reco_shift += shiftN*eRecoN;

      double Elep_reco_shift = Elep_reco*(1.+shiftMu);

      Ehad_reco_shift *= (1.+shiftTot);
      if( muon_contained ) Elep_reco_shift *= (1.+shiftTot);

      // resolution uncertainties
      Elep_reco_shift += (LepE - Elep_reco)*EmuRes[u];
      Ehad_reco_shift += ((eP + ePip + ePim) - (eRecoP + eRecoPip + eRecoPim))*EhadRes[u];
      Ehad_reco_shift += (ePi0 - eRecoPi0)*EEMRes[u];
      Ehad_reco_shift += (eN - eRecoN)*EneutRes[u];

      double Ev_reco_shift = Elep_reco_shift + Ehad_reco_shift;

      hists[u]->Fill( Ev_reco_shift, Ehad_reco_shift/Ev_reco_shift, wgt_mu*wgt_had );
      histsAccOnly[u]->Fill( Ev_reco, (Ev_reco - Elep_reco)/Ev_reco, wgt_mu*wgt_had );
      histsEscaleOnly[u]->Fill( Ev_reco_shift, Ehad_reco_shift/Ev_reco_shift, 1. );

      val_Ev[u]->Fill( Ev_reco, Ev_reco_shift );
      val_y[u]->Fill( (Ev_reco - Elep_reco)/Ev_reco, Ehad_reco_shift/Ev_reco_shift );
    }
  }

  TFile * valtf = new TFile( "validations.root", "RECREATE" );
  for( int u = 0; u < nu; ++u ) {
    val_Ev[u]->Write();
    val_y[u]->Write();

    hAccThrow[u]->Write();
    muAccThrow[u]->Write();

    EtotThrow[u]->Write();
    EmuThrowLAr[u]->Write();
    EmuThrowGAr[u]->Write();
    EhadThrow[u]->Write();
    EEMThrow[u]->Write();
    EneutThrow[u]->Write();
  }

  // Now determine the actual covariance
  int n_bins = n_Ebins * n_ybins;

  TMatrixD cov( n_bins, n_bins );
  TMatrixD covAcc( n_bins, n_bins );
  TMatrixD covEscale( n_bins, n_bins );
  for( int b0 = 0; b0 < n_bins; ++b0 ) {
    for( int b1 = 0; b1 < n_bins; ++b1 ) {
      cov[b0][b1] = 0.;
      covAcc[b0][b1] = 0.;
      covEscale[b0][b1] = 0.;
    }
  }
  for( int b0 = 0; b0 < n_bins; ++b0 ) {
    int bE0, by0;
    get2Dbins( b0+1, bE0, by0 );
    double cv0 = histCV->GetBinContent( bE0, by0 );

    for( int b1 = 0; b1 < n_bins; ++b1 ) {
      int bE1, by1;
      get2Dbins( b1+1, bE1, by1 );
      double cv1 = histCV->GetBinContent( bE1, by1 );

      for( int u = 0; u < nu; ++u ) {
        double u0 = hists[u]->GetBinContent( bE0, by0 );
        double u1 = hists[u]->GetBinContent( bE1, by1 );

        double u0a = histsAccOnly[u]->GetBinContent( bE0, by0 );
        double u1a = histsAccOnly[u]->GetBinContent( bE1, by1 );

        double u0e = histsEscaleOnly[u]->GetBinContent( bE0, by0 );
        double u1e = histsEscaleOnly[u]->GetBinContent( bE1, by1 );

        // fractional covariance, dividing out number of universes at the same time
        if( cv0*cv1 > 1.E-12 ) {
          cov[b0][b1] += (u0-cv0)*(u1-cv1)/(cv0*cv1*nu);
          covAcc[b0][b1] += (u0a-cv0)*(u1a-cv1)/(cv0*cv1*nu);
          covEscale[b0][b1] += (u0e-cv0)*(u1e-cv1)/(cv0*cv1*nu);
        }
      }
    }
  }

  // matrices are not positive definite due to numerical precision; make them positive definite
  fix( cov );
  fix( covAcc );
  fix( covEscale );

  // test that they are invertible, this will barf if they are singular
  TMatrixD covInv( TMatrixD::kInverted, cov );
  TMatrixD covAccInv( TMatrixD::kInverted, covAcc );
  TMatrixD covEscaleInv( TMatrixD::kInverted, covEscale );

  TFile * outfile = new TFile( "ND_syst_cov.root", "RECREATE" );
  cov.Write("nd_frac_cov");
  covAcc.Write("nd_frac_cov_accOnly");
  covEscale.Write("nd_frac_cov_EscaleOnly");

  TCanvas * c = new TCanvas();
  covAcc.Draw("colz");
  c->Print( "ND_syst_cov_acc.png" );
  covEscale.Draw("colz");
  c->Print( "ND_syst_cov_Escale.png" );
  cov.Draw("colz");
  c->Print( "ND_syst_cov.png" );
  c->SetLogz(1);
  c->Print( "ND_syst_cov_log.png" );    
  c->SetLogz(0);


  // Energy projection, for display purposes only
  TH2D * covE_acc = new TH2D( "covE_acc", ";Neutrino energy (GeV);Neutrino energy (GeV)", n_Ebins, Ebins, n_Ebins, Ebins );
  TH2D * covE_scale = new TH2D( "covE_scale", ";Neutrino energy (GeV);Neutrino energy (GeV)", n_Ebins, Ebins, n_Ebins, Ebins );

  for( int b0 = 1; b0 <= n_Ebins; ++b0 ) {
    double cv0 = histCV->ProjectionX()->GetBinContent( b0 );
    for( int b1 = 1; b1 <= n_Ebins; ++b1 ) {
      double cv1 = histCV->ProjectionX()->GetBinContent( b1 );

      for( int u = 0; u < nu; ++u ) {
        double u0a = histsAccOnly[u]->ProjectionX()->GetBinContent( b0 );
        double u1a = histsAccOnly[u]->ProjectionX()->GetBinContent( b1 );

        double u0e = histsEscaleOnly[u]->ProjectionX()->GetBinContent( b0 );
        double u1e = histsEscaleOnly[u]->ProjectionX()->GetBinContent( b1 );

        // fractional covariance, dividing out number of universes at the same time
        if( cv0*cv1 > 1.E-12 ) {
          covE_acc->Fill( covE_acc->GetXaxis()->GetBinCenter(b0), covE_acc->GetXaxis()->GetBinCenter(b1), (u0a-cv0)*(u1a-cv1)/(cv0*cv1*nu) );
          covE_scale->Fill( covE_acc->GetXaxis()->GetBinCenter(b0), covE_acc->GetXaxis()->GetBinCenter(b1), (u0e-cv0)*(u1e-cv1)/(cv0*cv1*nu) );
        }
      }
    }
  }

  covE_acc->SetMaximum(0.0005);
  covE_acc->Draw("colz");
  c->Print( "ND_syst_cov_projE_acc.png" );
  covE_scale->SetMaximum(0.025);
  covE_scale->Draw("colz");
  c->Print( "ND_syst_cov_projE_scale.png" );

}
















