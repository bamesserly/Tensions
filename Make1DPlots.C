void Make1DPlots() {

  TFile *f = TFile::Open("1DHists_minervabinning_CarrieSignal.root");
  gStyle->SetOptStat(0);

  const unsigned int NCUTS=9;
  const double EFF_MAX = 1.;
  const double EFF_MIN = 0.;
  const double PUR_MAX = 1.;
  const double PUR_MIN = 0.;

  std::string cut_names[NCUTS+1] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
  
  //get denominator histograms - efficiency
  TH1D* hTpi_den = (TH1D*)f->Get("hTpi_c0_sig");
  TH1D* hCosThetapi_den = (TH1D*)f->Get("hCosThetapi_c0_sig");
  TH1D* hPmu_den = (TH1D*)f->Get("hPmu_c0_sig");
  TH1D* hCosThetamu_den = (TH1D*)f->Get("hCosThetamu_c0_sig");
  TH1D* hW_den = (TH1D*)f->Get("hW_c0_sig");
  TH1D* hQ2_den = (TH1D*)f->Get("hQ2_c0_sig");
  TH1D* hq0_den = (TH1D*)f->Get("hq0_c0_sig");
  TH1D* hq3_den = (TH1D*)f->Get("hq3_c0_sig");




  for (unsigned int c=1; c!=NCUTS+1; ++c) {
    //Get numerator histograms -- efficiency
    TH1D* hTpi_num = (TH1D*)f->Get(Form("hTpi_c%d_sig",c));
    TH1D* hCosThetapi_num = (TH1D*)f->Get(Form("hCosThetapi_c%d_sig",c));
    TH1D* hPmu_num = (TH1D*)f->Get(Form("hPmu_c%d_sig",c));
    TH1D* hCosThetamu_num = (TH1D*)f->Get(Form("hCosThetamu_c%d_sig",c));
    TH1D* hW_num = (TH1D*)f->Get(Form("hW_c%d_sig",c));
    TH1D* hQ2_num = (TH1D*)f->Get(Form("hQ2_c%d_sig",c));
    TH1D* hq0_num = (TH1D*)f->Get(Form("hq0_c%d_sig",c));
    TH1D* hq3_num = (TH1D*)f->Get(Form("hq3_c%d_sig",c));

    hTpi_num->SetMaximum(EFF_MAX);
    hCosThetapi_num->SetMaximum(EFF_MAX);
    hPmu_num->SetMaximum(EFF_MAX); 
    hCosThetamu_num->SetMaximum(EFF_MAX);
    hW_num->SetMaximum(EFF_MAX);
    hQ2_num->SetMaximum(EFF_MAX);  
    hq0_num->SetMaximum(EFF_MAX);  
    hq3_num->SetMaximum(EFF_MAX); 

    hTpi_num->SetMinimum(EFF_MIN);
    hCosThetapi_num->SetMinimum(EFF_MIN);
    hPmu_num->SetMinimum(EFF_MIN); 
    hCosThetamu_num->SetMinimum(EFF_MIN);
    hW_num->SetMinimum(EFF_MIN);
    hQ2_num->SetMinimum(EFF_MIN);  
    hq0_num->SetMinimum(EFF_MIN);  
    hq3_num->SetMinimum(EFF_MIN); 

    //Get denominator histograms -- purity
    TH1D* hTpi_pur_den = (TH1D*)f->Get(Form("hTpi_c%d_all",c));
    TH1D* hCosThetapi_pur_den = (TH1D*)f->Get(Form("hCosThetapi_c%d_all",c));
    TH1D* hPmu_pur_den = (TH1D*)f->Get(Form("hPmu_c%d_all",c));
    TH1D* hCosThetamu_pur_den = (TH1D*)f->Get(Form("hCosThetamu_c%d_all",c));
    TH1D* hW_pur_den = (TH1D*)f->Get(Form("hW_c%d_all",c));
    TH1D* hQ2_pur_den = (TH1D*)f->Get(Form("hQ2_c%d_all",c));
    TH1D* hq0_pur_den = (TH1D*)f->Get(Form("hq0_c%d_all",c));
    TH1D* hq3_pur_den = (TH1D*)f->Get(Form("hq3_c%d_all",c));
    
    //make cumulative efficiency histograms
    hTpi_num->Divide(hTpi_den);
    hCosThetapi_num->Divide(hCosThetapi_den);
    hPmu_num->Divide(hPmu_den);
    hCosThetamu_num->Divide(hCosThetamu_den);
    hW_num->Divide(hW_den);
    hQ2_num->Divide(hQ2_den);
    hq0_num->Divide(hq0_den);
    hq3_num->Divide(hq3_den);
    
    TCanvas* c1 = new TCanvas();
    hTpi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/Tpi_c%d.pdf",c));

    hCosThetapi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/CosThetapi_c%d.pdf",c));
    
    hPmu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/Pmu_c%d.pdf",c));

    hCosThetamu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/CosThetamu_c%d.pdf",c));
    
    hW_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/W_c%d.pdf",c));

    hQ2_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/Q2_c%d.pdf",c));
    
    hq0_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/q0_c%d.pdf",c));

    hq3_num->Draw();
    c1->Print(Form("plots_1D_Carrie/eff/q3_c%d.pdf",c));

    //now make cumulative purity histograms
    hTpi_num->Multiply(hTpi_den);
    hCosThetapi_num->Multiply(hCosThetapi_den);
    hPmu_num->Multiply(hPmu_den);
    hCosThetamu_num->Multiply(hCosThetamu_den);
    hW_num->Multiply(hW_den);
    hQ2_num->Multiply(hQ2_den);
    hq0_num->Multiply(hq0_den);
    hq3_num->Multiply(hq3_den);

    hTpi_num->Divide(hTpi_pur_den);
    hCosThetapi_num->Divide(hCosThetapi_pur_den);
    hPmu_num->Divide(hPmu_pur_den);
    hCosThetamu_num->Divide(hCosThetamu_pur_den);
    hW_num->Divide(hW_pur_den);
    hQ2_num->Divide(hQ2_pur_den);
    hq0_num->Divide(hq0_pur_den);
    hq3_num->Divide(hq3_pur_den);

    hTpi_num->SetTitle       (Form("Cumulative Purity, Cut: %s;T_{#pi} (GeV)",     cut_names[c].c_str()));
    hCosThetapi_num->SetTitle(Form("Cumulative Purity, Cut: %s;cos(#theta_{#pi})", cut_names[c].c_str()));
                                                                                                        
    hPmu_num->SetTitle       (Form("Cumulative Purity, Cut: %s;P_{#mu} (GeV)",     cut_names[c].c_str()));      
    hCosThetamu_num->SetTitle(Form("Cumulative Purity, Cut: %s;cos(#theta_{#mu})", cut_names[c].c_str()));
                                                                                                        
    hW_num->SetTitle         (Form("Cumulative Purity, Cut: %s;W (GeV)",           cut_names[c].c_str()));          
    hQ2_num->SetTitle        (Form("Cumulative Purity, Cut: %s;Q^{2} (GeV^{2})",   cut_names[c].c_str()));       
                                                                                                        
    hq0_num->SetTitle        (Form("Cumulative Purity, Cut: %s;q_{0} (GeV)",       cut_names[c].c_str()));       
    hq3_num->SetTitle        (Form("Cumulative Purity, Cut: %s;|q_{3}| (GeV)",     cut_names[c].c_str()));       

    hTpi_num->SetMaximum(PUR_MAX);
    hCosThetapi_num->SetMaximum(PUR_MAX);
    hPmu_num->SetMaximum(PUR_MAX); 
    hCosThetamu_num->SetMaximum(PUR_MAX);
    hW_num->SetMaximum(PUR_MAX);
    hQ2_num->SetMaximum(PUR_MAX);  
    hq0_num->SetMaximum(PUR_MAX);  
    hq3_num->SetMaximum(PUR_MAX); 

    hTpi_num->SetMinimum(PUR_MIN);
    hCosThetapi_num->SetMinimum(PUR_MIN);
    hPmu_num->SetMinimum(PUR_MIN); 
    hCosThetamu_num->SetMinimum(PUR_MIN);
    hW_num->SetMinimum(PUR_MIN);
    hQ2_num->SetMinimum(PUR_MIN);  
    hq0_num->SetMinimum(PUR_MIN);  
    hq3_num->SetMinimum(PUR_MIN); 

    hTpi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/Tpi_c%d.pdf",c));

    hCosThetapi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/CosThetapi_c%d.pdf",c));
    
    hPmu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/Pmu_c%d.pdf",c));

    hCosThetamu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/CosThetamu_c%d.pdf",c));
    
    hW_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/W_c%d.pdf",c));

    hQ2_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/Q2_c%d.pdf",c));
    
    hq0_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/q0_c%d.pdf",c));

    hq3_num->Draw();
    c1->Print(Form("plots_1D_Carrie/pur/q3_c%d.pdf",c));

    //now make total signal events histograms
    hTpi_num->Multiply(hTpi_pur_den);
    hCosThetapi_num->Multiply(hCosThetapi_pur_den);
    hPmu_num->Multiply(hPmu_pur_den);
    hCosThetamu_num->Multiply(hCosThetamu_pur_den);
    hW_num->Multiply(hW_pur_den);
    hQ2_num->Multiply(hQ2_pur_den);
    hq0_num->Multiply(hq0_pur_den);
    hq3_num->Multiply(hq3_pur_den);

    hTpi_num->GetYaxis()->SetRangeUser(0.0,hTpi_num->GetBinContent(hTpi_num->GetMaximumBin()));
    hCosThetapi_num->GetYaxis()->SetRangeUser(0.0,hCosThetapi_num->GetBinContent(hCosThetapi_num->GetMaximumBin()));
    hPmu_num->GetYaxis()->SetRangeUser(0.0,hPmu_num->GetBinContent(hPmu_num->GetMaximumBin()));
    hCosThetamu_num->GetYaxis()->SetRangeUser(0.0,hCosThetamu_num->GetBinContent(hCosThetamu_num->GetMaximumBin()));
    hW_num->GetYaxis()->SetRangeUser( 0.0,hW_num->GetBinContent(hW_num->GetMaximumBin()));
    hQ2_num->GetYaxis()->SetRangeUser(0.0,hQ2_num->GetBinContent(hQ2_num->GetMaximumBin()));
    hq0_num->GetYaxis()->SetRangeUser(0.0,hq0_num->GetBinContent(hq0_num->GetMaximumBin()));
    hq3_num->GetYaxis()->SetRangeUser(0.0,hq3_num->GetBinContent(hq3_num->GetMaximumBin()));

    hTpi_num->SetTitle       (Form("Total Signal Events After Cut: %s;T_{#pi} (GeV)",     cut_names[c].c_str()));
    hCosThetapi_num->SetTitle(Form("Total Signal Events After Cut: %s;cos(#theta_{#pi})", cut_names[c].c_str()));
                                                                                                        
    hPmu_num->SetTitle       (Form("Total Signal Events After Cut: %s;P_{#mu} (GeV)",     cut_names[c].c_str()));      
    hCosThetamu_num->SetTitle(Form("Total Signal Events After Cut: %s;cos(#theta_{#mu})", cut_names[c].c_str()));
                                                                                                        
    hW_num->SetTitle         (Form("Total Signal Events After Cut: %s;W (GeV)",           cut_names[c].c_str()));          
    hQ2_num->SetTitle        (Form("Total Signal Events After Cut: %s;Q^{2} (GeV^{2})",   cut_names[c].c_str()));       
                                                                                                        
    hq0_num->SetTitle        (Form("Total Signal Events After Cut: %s;q_{0} (GeV)",       cut_names[c].c_str()));       
    hq3_num->SetTitle        (Form("Total Signal Events After Cut: %s;|q_{3}| (GeV)",     cut_names[c].c_str()));       

    hTpi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/Tpi_c%d.pdf",c));

    hCosThetapi_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/CosThetapi_c%d.pdf",c));
    
    hPmu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/Pmu_c%d.pdf",c));

    hCosThetamu_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/CosThetamu_c%d.pdf",c));
    
    hW_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/W_c%d.pdf",c));

    hQ2_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/Q2_c%d.pdf",c));
    
    hq0_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/q0_c%d.pdf",c));

    hq3_num->Draw();
    c1->Print(Form("plots_1D_Carrie/tot/q3_c%d.pdf",c));
    
    delete c1;
  }
}
