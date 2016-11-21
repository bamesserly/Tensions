void Make2DPlots() {

  gStyle->SetOptStat(0);
  gROOT->ProcessLine(".x pittsburgh_style.C");

  bool W_RESTRICTIONS = true;
  bool T2KBINNING = true;

  std::cout << "W restriction is " << W_RESTRICTIONS << std::endl;
  std::cout << "T2K Binning is " << T2KBINNING << std::endl;

  unsigned int NCUTS;
  std::string INFILE;
  std::string PLOTDIR; 
  const std::string* cut_names;

  if(W_RESTRICTIONS){
    NCUTS = 9;
    const std::string cut_names_W[]= {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
    cut_names = cut_names_W;
    if(T2KBINNING){
      INFILE = "2DHists_T2Kbinning.root";
      PLOTDIR = "./plots_T2Kbinning";
    }
    else{
      INFILE = "2DHists_minervabinning.root";
      PLOTDIR = "./plots_minervabinning";
    }
  }
  else{
    NCUTS = 8;
    //INFILE = "2DHists_minervabinning_NOWRESTRICTIONS.root";
    //PLOTDIR = "./plots_minervabinning_NOWRESTRICTION";
    const std::string cut_names_noW[] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
    cut_names = cut_names_noW;
    if(T2KBINNING){
      INFILE = "2DHists_T2Kbinning_NOWRESTRICTIONS.root";
      PLOTDIR = "./plots_T2Kbinning_NOWRESTRICTION";
    }
    else{
      INFILE = "2DHists_minervabinning_NOWRESTRICTIONS.root";
      PLOTDIR = "./plots_minervabinning_NOWRESTRICTION";
    }
  }
  //INFILE = "2DHists_minervabinning.root";

  TFile *f = TFile::Open(INFILE.c_str());
  
  //get denominator histograms - efficiency
  TH2D* hTvTheta_pi_den = (TH2D*)f->Get("hTvTheta_c0_pi_sig");
  TH2D* hPvTheta_mu_den = (TH2D*)f->Get("hPvTheta_c0_mu_sig");
  TH2D* hWvQ2_den = (TH2D*)f->Get("hWvQ2_c0_sig");
  TH2D* hWexpvQ2_den = (TH2D*)f->Get("hWexpvQ2_c0_sig");
  TH2D* hq0vq3_den = (TH2D*)f->Get("hq0vq3_c0_sig");
  TH2D* hcosPvTpi_den = (TH2D*)f->Get("hcosPvTpi_c0_sig");
  TH2D* hcosPvcosTpi_den = (TH2D*)f->Get("hcosPvcosTpi_c0_sig");
  TH2D* hphiPvTpi_den = (TH2D*)f->Get("hphiPvTpi_c0_sig");
  TH2D* hphiPvcosTpi_den = (TH2D*)f->Get("hphiPvcosTpi_c0_sig");
  
  for (unsigned int c=1; c!=NCUTS+1; ++c) {
    TH2D* hTvTheta_pi_num = (TH2D*)f->Get(Form("hTvTheta_c%d_pi_sig",c));
    TH2D* hPvTheta_mu_num = (TH2D*)f->Get(Form("hPvTheta_c%d_mu_sig",c));
    TH2D* hWvQ2_num = (TH2D*)f->Get(Form("hWvQ2_c%d_sig",c));
    TH2D* hWexpvQ2_num = (TH2D*)f->Get(Form("hWexpvQ2_c%d_sig",c));
    TH2D* hq0vq3_num = (TH2D*)f->Get(Form("hq0vq3_c%d_sig",c));
    TH2D* hcosPvTpi_num = (TH2D*)f->Get(Form("hcosPvTpi_c%d_sig",c));
    TH2D* hcosPvcosTpi_num = (TH2D*)f->Get(Form("hcosPvcosTpi_c%d_sig",c));
    TH2D* hphiPvTpi_num = (TH2D*)f->Get(Form("hphiPvTpi_c%d_sig",c));
    TH2D* hphiPvcosTpi_num = (TH2D*)f->Get(Form("hphiPvcosTpi_c%d_sig",c));
    
    //get denominator histograms - purity
    TH2D* hTvTheta_pi_pur_den = (TH2D*)f->Get(Form("hTvTheta_c%d_pi_all",c));
    TH2D* hPvTheta_mu_pur_den = (TH2D*)f->Get(Form("hPvTheta_c%d_mu_all",c));
    TH2D* hWvQ2_pur_den = (TH2D*)f->Get(Form("hWvQ2_c%d_all",c));
    TH2D* hWexpvQ2_pur_den = (TH2D*)f->Get(Form("hWexpvQ2_c%d_all",c));
    TH2D* hq0vq3_pur_den = (TH2D*)f->Get(Form("hq0vq3_c%d_all",c));
    TH2D* hcosPvTpi_pur_den = (TH2D*)f->Get(Form("hcosPvTpi_c%d_all",c));
    TH2D* hcosPvcosTpi_pur_den = (TH2D*)f->Get(Form("hcosPvcosTpi_c%d_all",c));
    TH2D* hphiPvTpi_pur_den = (TH2D*)f->Get(Form("hphiPvTpi_c%d_all",c));
    TH2D* hphiPvcosTpi_pur_den = (TH2D*)f->Get(Form("hphiPvcosTpi_c%d_all",c));
       
    //make cumulative efficiency histograms
    hTvTheta_pi_num->Divide(hTvTheta_pi_den);
    hPvTheta_mu_num->Divide(hPvTheta_mu_den);
    hWvQ2_num->Divide(hWvQ2_den);
    hWexpvQ2_num->Divide(hWexpvQ2_den);
    hq0vq3_num->Divide(hq0vq3_den);
    hcosPvTpi_num->Divide(hcosPvTpi_den);
    hcosPvcosTpi_num->Divide(hcosPvcosTpi_den);
    hphiPvTpi_num->Divide(hphiPvTpi_den);
    hphiPvcosTpi_num->Divide(hphiPvcosTpi_den);
    
    TCanvas* c1 = new TCanvas();
    hTvTheta_pi_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hTvTheta_pi_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/TvTheta_pi_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hPvTheta_mu_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hPvTheta_mu_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/PvTheta_mu_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hWvQ2_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hWvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/WvQ2_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hWexpvQ2_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hWexpvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/WexpvQ2_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hq0vq3_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hq0vq3_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/q0vq3_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hcosPvTpi_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hcosPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/cosThetaPlanarvTpi_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hcosPvcosTpi_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hcosPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/cosThetaPlanarvcosThetapi_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hphiPvTpi_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hphiPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/PhiPlanarvTpi_c%d_eff.png",PLOTDIR.c_str(),c));
    
    hphiPvcosTpi_num->GetZaxis()->SetRangeUser(0.0,1.0);
    hphiPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/eff/2D/PhiPlanarvcosThetapi_c%d_eff.png",PLOTDIR.c_str(),c));
    
    
    //now make cumulative purity histograms
    hTvTheta_pi_num->Multiply(hTvTheta_pi_den);
    hPvTheta_mu_num->Multiply(hPvTheta_mu_den);
    hWvQ2_num->Multiply(hWvQ2_den);
    hWexpvQ2_num->Multiply(hWexpvQ2_den);
    hq0vq3_num->Multiply(hq0vq3_den);
    hcosPvTpi_num->Multiply(hcosPvTpi_den);
    hcosPvcosTpi_num->Multiply(hcosPvcosTpi_den);
    hphiPvTpi_num->Multiply(hphiPvTpi_den);
    hphiPvcosTpi_num->Multiply(hphiPvcosTpi_den);
    
    hTvTheta_pi_num->Divide(hTvTheta_pi_pur_den);
    hPvTheta_mu_num->Divide(hPvTheta_mu_pur_den);
    hWvQ2_num->Divide(hWvQ2_pur_den);
    hWexpvQ2_num->Divide(hWexpvQ2_pur_den);
    hq0vq3_num->Divide(hq0vq3_pur_den);
    hcosPvTpi_num->Divide(hcosPvTpi_pur_den);
    hcosPvcosTpi_num->Divide(hcosPvcosTpi_pur_den);
    hphiPvTpi_num->Divide(hphiPvTpi_pur_den);
    hphiPvcosTpi_num->Divide(hphiPvcosTpi_pur_den);
    
    hTvTheta_pi_num->SetTitle(Form("Cumulative Purity, Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi}) (deg)",cut_names[c].c_str()));
    hPvTheta_mu_num->SetTitle(Form("Cumulative Purity, Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu}) (deg)",cut_names[c].c_str()));
    hWvQ2_num->SetTitle(Form("Cumulative Purity, Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()));
    hWexpvQ2_num->SetTitle(Form("Cumulative Purity, Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()));
    hq0vq3_num->SetTitle(Form("Cumulative Purity, Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()));
    hcosPvTpi_num->SetTitle(Form("Cumulative Purity, Cut: %s",cut_names[c].c_str()));
    hcosPvcosTpi_num->SetTitle(Form("Cumulative Purity, Cut: %s",cut_names[c].c_str()));
    hphiPvTpi_num->SetTitle(Form("Cumulative Purity, Cut: %s",cut_names[c].c_str()));
    hphiPvcosTpi_num->SetTitle(Form("Cumulative Purity, Cut: %s",cut_names[c].c_str()));
    
    hTvTheta_pi_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/TvTheta_pi_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hPvTheta_mu_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/PvTheta_mu_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hWvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/WvQ2_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hWexpvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/WexpvQ2_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hq0vq3_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/q0vq3_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hcosPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/cosThetaPlanarvTpi_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hcosPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/cosThetaPlanarvcosThetapi_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hphiPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/PhiPlanarvTpi_c%d_pur.png",PLOTDIR.c_str(),c));
    
    hphiPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/pur/2D/PhiPlanarvcosThetapi_c%d_pur.png",PLOTDIR.c_str(),c));
    
    //now make total signal events histograms
    hTvTheta_pi_num->Multiply(hTvTheta_pi_pur_den);
    hPvTheta_mu_num->Multiply(hPvTheta_mu_pur_den);
    hWvQ2_num->Multiply(hWvQ2_pur_den);
    hWexpvQ2_num->Multiply(hWexpvQ2_pur_den);
    hq0vq3_num->Multiply(hq0vq3_pur_den);
    hcosPvTpi_num->Multiply(hcosPvTpi_pur_den);
    hcosPvcosTpi_num->Multiply(hcosPvcosTpi_pur_den);
    hphiPvTpi_num->Multiply(hphiPvTpi_pur_den);
    hphiPvcosTpi_num->Multiply(hphiPvcosTpi_pur_den);
    
    hTvTheta_pi_num->GetZaxis()->SetRangeUser(0.0,hTvTheta_pi_num->GetBinContent(hTvTheta_pi_num->GetMaximumBin()));
    hPvTheta_mu_num->GetZaxis()->SetRangeUser(0.0,hPvTheta_mu_num->GetBinContent(hPvTheta_mu_num->GetMaximumBin()));
    hWvQ2_num->GetZaxis()->SetRangeUser(0.0,hWvQ2_num->GetBinContent(hWvQ2_num->GetMaximumBin()));
    hWexpvQ2_num->GetZaxis()->SetRangeUser(0.0,hWexpvQ2_num->GetBinContent(hWexpvQ2_num->GetMaximumBin()));
    hq0vq3_num->GetZaxis()->SetRangeUser(0.0,hq0vq3_num->GetBinContent(hq0vq3_num->GetMaximumBin()));
    hcosPvTpi_num->GetZaxis()->SetRangeUser(0.0,hcosPvTpi_num->GetBinContent(hcosPvTpi_num->GetMaximumBin()));
    hcosPvcosTpi_num->GetZaxis()->SetRangeUser(0.0,hcosPvcosTpi_num->GetBinContent(hcosPvcosTpi_num->GetMaximumBin()));
    hphiPvTpi_num->GetZaxis()->SetRangeUser(0.0,hphiPvTpi_num->GetBinContent(hphiPvTpi_num->GetMaximumBin()));
    hphiPvcosTpi_num->GetZaxis()->SetRangeUser(0.0,hphiPvcosTpi_num->GetBinContent(hphiPvcosTpi_num->GetMaximumBin()));
    
    hTvTheta_pi_num->SetTitle(Form("Total Signal Events after Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi}) (deg)",cut_names[c].c_str()));
    hPvTheta_mu_num->SetTitle(Form("Total Signal Events after Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu}) (deg)",cut_names[c].c_str()));
    hWvQ2_num->SetTitle(Form("Total Signal Events after Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()));
    hWexpvQ2_num->SetTitle(Form("Total Signal Events after Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()));
    hq0vq3_num->SetTitle(Form("Total Signal Events after Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()));
    hcosPvTpi_num->SetTitle(Form("Total Signal Events after Cut: %s",cut_names[c].c_str()));
    hcosPvcosTpi_num->SetTitle(Form("Total Signal Events after Cut: %s",cut_names[c].c_str()));
    hphiPvTpi_num->SetTitle(Form("Total Signal Events after Cut: %s",cut_names[c].c_str()));
    hphiPvcosTpi_num->SetTitle(Form("Total Signal Events after Cut: %s",cut_names[c].c_str()));
    
    hTvTheta_pi_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/TvTheta_pi_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hPvTheta_mu_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/PvTheta_mu_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hWvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/WvQ2_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hWexpvQ2_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/WexpvQ2_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hq0vq3_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/q0vq3_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hcosPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/cosThetaPlanarvTpi_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hcosPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/cosThetaPlanarvcosThetapi_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hphiPvTpi_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/PhiPlanarvTpi_c%d_tot.png",PLOTDIR.c_str(),c));
    
    hphiPvcosTpi_num->Draw("COLZ");
    c1->Print(Form("%s/tot/2D/PhiPlanarvcosThetapi_c%d_tot.png",PLOTDIR.c_str(),c));
       
    delete c1;
  }
}
