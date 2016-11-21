void CompareSignals() {
  //int kLineWidth = 3;

  TFile *f_Carrie = TFile::Open("1DHists_minervabinning_CarrieSignal.root");
  TFile *f_Brandon = TFile::Open("1DHists_minervabinning_BrandonSignal.root");
  gStyle->SetOptStat(0);

  const unsigned int NCUTS=9;

  std::string cut_names[NCUTS+1] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};

  TCanvas *c1 = new TCanvas("foo","foo",1032,700);
  std::string outfile;
  for(unsigned int i=0; i < NCUTS+1; ++i){

    TH1D* hTpi_Brandon          = (TH1D*)f_Brandon->Get(Form("hTpi_c%d_sig",        i));
    TH1D* hCosThetapi_Brandon   = (TH1D*)f_Brandon->Get(Form("hCosThetapi_c%d_sig", i));
    TH1D* hThetapi_Brandon   = (TH1D*)f_Brandon->Get(Form("hThetapi_c%d_sig", i));
    TH1D* hPmu_Brandon          = (TH1D*)f_Brandon->Get(Form("hPmu_c%d_sig",        i));
    TH1D* hCosThetamu_Brandon   = (TH1D*)f_Brandon->Get(Form("hCosThetamu_c%d_sig", i));
    TH1D* hThetamu_Brandon   = (TH1D*)f_Brandon->Get(Form("hThetamu_c%d_sig", i));
    TH1D* hWexp_Brandon            = (TH1D*)f_Brandon->Get(Form("hWexp_c%d_sig",          i));
    TH1D* hWgenie_Brandon            = (TH1D*)f_Brandon->Get(Form("hWgenie_c%d_sig",          i));
    TH1D* hQ2exp_Brandon           = (TH1D*)f_Brandon->Get(Form("hQ2exp_c%d_sig",         i));
    TH1D* hQ2genie_Brandon           = (TH1D*)f_Brandon->Get(Form("hQ2genie_c%d_sig",         i));
    TH1D* hq0_Brandon           = (TH1D*)f_Brandon->Get(Form("hq0_c%d_sig",        i));
    TH1D* hq3_Brandon           = (TH1D*)f_Brandon->Get(Form("hq3_c%d_sig",         i));

    TH1D* hTpi_Carrie          = (TH1D*)f_Carrie->Get(Form("hTpi_c%d_sig",        i));
    TH1D* hCosThetapi_Carrie   = (TH1D*)f_Carrie->Get(Form("hCosThetapi_c%d_sig", i));
    TH1D* hThetapi_Carrie   = (TH1D*)f_Carrie->Get(Form("hThetapi_c%d_sig", i));
    TH1D* hPmu_Carrie          = (TH1D*)f_Carrie->Get(Form("hPmu_c%d_sig",        i));
    TH1D* hCosThetamu_Carrie   = (TH1D*)f_Carrie->Get(Form("hCosThetamu_c%d_sig", i));
    TH1D* hThetamu_Carrie   = (TH1D*)f_Carrie->Get(Form("hThetamu_c%d_sig", i));
    TH1D* hWexp_Carrie            = (TH1D*)f_Carrie->Get(Form("hWexp_c%d_sig",          i));
    TH1D* hWgenie_Carrie            = (TH1D*)f_Carrie->Get(Form("hWgenie_c%d_sig",          i));
    TH1D* hQ2exp_Carrie           = (TH1D*)f_Carrie->Get(Form("hQ2exp_c%d_sig",         i));
    TH1D* hQ2genie_Carrie           = (TH1D*)f_Carrie->Get(Form("hQ2genie_c%d_sig",         i));
    TH1D* hq0_Carrie           = (TH1D*)f_Carrie->Get(Form("hq0_c%d_sig",        i));
    TH1D* hq3_Carrie           = (TH1D*)f_Carrie->Get(Form("hq3_c%d_sig",         i));

    hTpi_Brandon       ->SetTitle(Form("Reminaing signal events after Cut: %s;T_{#pi} (GeV)", cut_names[i].c_str()));   
    hCosThetapi_Brandon->SetTitle(Form("Reminaing signal events after Cut: %s;cos(#theta_{#pi})", cut_names[i].c_str()));
    hThetapi_Brandon->SetTitle(Form("Reminaing signal events after Cut: %s;#theta_{#pi}", cut_names[i].c_str()));
    hPmu_Brandon       ->SetTitle(Form("Reminaing signal events after Cut: %s;P_{#mu} (GeV) ", cut_names[i].c_str()));
    hCosThetamu_Brandon->SetTitle(Form("Reminaing signal events after Cut: %s;cos(#theta_{#mu}) ", cut_names[i].c_str()));
    hCosThetamu_Brandon->SetTitle(Form("Reminaing signal events after Cut: %s;#theta_{#mu} ", cut_names[i].c_str()));
    hWexp_Brandon         ->SetTitle(Form("Reminaing signal events after Cut: %s;W exp (GeV) ", cut_names[i].c_str()));
    hWgenie_Brandon         ->SetTitle(Form("Reminaing signal events after Cut: %s;W genie (GeV) ", cut_names[i].c_str()));
    hQ2exp_Brandon        ->SetTitle(Form("Reminaing signal events after Cut: %s;Q^{2} exp (GeV^{2}) ", cut_names[i].c_str()));
    hQ2genie_Brandon        ->SetTitle(Form("Reminaing signal events after Cut: %s;Q^{2} genie (GeV^{2}) ", cut_names[i].c_str()));
    hq0_Brandon        ->SetTitle(Form("Reminaing signal events after Cut: %s;q_{0} (GeV) ", cut_names[i].c_str()));
    hq3_Brandon        ->SetTitle(Form("Reminaing signal events after Cut: %s;|q_{3}| (GeV) ", cut_names[i].c_str()));


    //Tpi
    hTpi_Brandon->SetLineColor(kRed);
    {
      hTpi_Brandon->Draw("hist");
      hTpi_Carrie->Draw("histsame");
      outfile = Form("plots_compare/hTpi_c%d.pdf",        i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hTpi_Brandon->Clone(Form("hTpi_c%d_sig",i));
      h_ratio->Divide(hTpi_Brandon,hTpi_Carrie);

      double rebin_factor = 10.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hTpi_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.5);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hTpi_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }


    //CosThetapi
    hCosThetapi_Brandon->SetLineColor(kRed);
    {
      hCosThetapi_Brandon->Draw("hist");
      hCosThetapi_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hCosThetapi_c%d.pdf", i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hCosThetapi_Brandon->Clone(Form("hCosThetapi_c%d_sig",i));
      h_ratio->Divide(hCosThetapi_Brandon,hCosThetapi_Carrie);

      double rebin_factor = 2.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hTpi_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hCosThetapi_c%d.pdf",i);
      c1->Print(outfile.c_str());
    }

    //Thetapi
    hThetapi_Brandon->SetLineColor(kRed);
    {
      hCosThetapi_Brandon->Draw("hist");
      hCosThetapi_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hCosThetapi_c%d.pdf", i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hThetapi_Brandon->Clone(Form("hThetapi_c%d_sig",i));
      h_ratio->Divide(hThetapi_Brandon,hThetapi_Carrie);

      double rebin_factor = 2.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hTpi_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hThetapi_c%d.pdf",i);
      c1->Print(outfile.c_str());
    }

    //Pmu
    hPmu_Brandon->SetLineColor(kRed);
    {
      hPmu_Brandon->Draw("hist");
      hPmu_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hPmu_c%d.pdf",        i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hPmu_Brandon->Clone(Form("hPmu_c%d_sig",i));
      h_ratio->Divide(hPmu_Brandon,hPmu_Carrie);

      double rebin_factor = 2.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hPmu_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hPmu_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //CosThetamu
    hCosThetamu_Brandon->SetLineColor(kRed);
    {
      hCosThetamu_Brandon->Draw("hist");
      hCosThetamu_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hCosThetamu_c%d.pdf", i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hCosThetamu_Brandon->Clone(Form("hCosThetamu_c%d_sig",i));
      h_ratio->Divide(hCosThetamu_Brandon,hCosThetamu_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hCosThetamu_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hCosThetamu_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //Thetamu
    hThetamu_Brandon->SetLineColor(kRed);
    { 
      hThetamu_Brandon->Draw("hist");
      hThetamu_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hThetamu_c%d.pdf", i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hThetamu_Brandon->Clone(Form("hThetamu_c%d_sig",i));
      h_ratio->Divide(hThetamu_Brandon,hThetamu_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hThetamu_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hThetamu_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //Wexp
    hWexp_Brandon->SetLineColor(kRed);
    {
      hWexp_Brandon->Draw("hist");
      hWexp_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hWexp_c%d.pdf",          i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hWexp_Brandon->Clone(Form("hWexp_c%d_sig",i));
      h_ratio->Divide(hWexp_Brandon,hWexp_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hWexp_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hWexp_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //Wgenie
    hWgenie_Brandon->SetLineColor(kRed);
    {
      hWgenie_Brandon->Draw("hist");
      hWgenie_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hWgenie_c%d.pdf",          i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hWgenie_Brandon->Clone(Form("hWgenie_c%d_sig",i));
      h_ratio->Divide(hWgenie_Brandon,hWgenie_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hWgenie_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hWgenie_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //Wexp and Wgenie
    {
      hWgenie_Brandon->SetLineColor(kBlue);
      hWexp_Brandon->SetLineColor(kRed);
      hWgenie_Brandon->Draw("hist");
      hWgenie_Brandon->GetXaxis()->SetTitle("Blue:Wgenie, Red:Wexp");
      hWgenie_Brandon->SetTitle(Form("Brandon. Signal events after cut: %s", cut_names[i].c_str()));
      hWexp_Brandon->Draw("histsame");
      outfile = Form("plots_compare/hBrandon_Wexp_Wgenie_compare_c%d.pdf",          i);
      c1->Print(outfile.c_str());
    }
    {
      hWgenie_Carrie->SetLineColor(kBlue);
      hWexp_Carrie->SetLineColor(kRed);
      hWgenie_Carrie->Draw("hist");
      hWgenie_Carrie->GetXaxis()->SetTitle("Blue:Wgenie, Red:Wexp");
      hWgenie_Carrie->SetTitle(Form("Carrie. Signal events after cut: %s", cut_names[i].c_str()));
      hWexp_Carrie->Draw("histsame");
      outfile = Form("plots_compare/hCarrie_Wexp_Wgenie_compare_c%d.pdf",          i);
      c1->Print(outfile.c_str());
    }



    //Q2exp
    hQ2exp_Brandon->SetLineColor(kRed);
    {
      hQ2exp_Brandon->Draw("hist");
      hQ2exp_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hQ2exp_c%d.pdf",         i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hQ2exp_Brandon->Clone(Form("hQ2exp_c%d_sig",i));
      h_ratio->Divide(hQ2exp_Brandon,hQ2exp_Carrie);

      double rebin_factor = 2.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hQ2exp_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hQ2exp_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //Q2genie
    hQ2genie_Brandon->SetLineColor(kRed);
    {
      hQ2genie_Brandon->Draw("hist");
      hQ2genie_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hQ2genie_c%d.pdf",         i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hQ2genie_Brandon->Clone(Form("hQ2genie_c%d_sig",i));
      h_ratio->Divide(hQ2genie_Brandon,hQ2genie_Carrie);

      double rebin_factor = 2.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hQ2genie_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hQ2genie_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //q0
    hq0_Brandon->SetLineColor(kRed);
    {
      hq0_Brandon->Draw("hist");
      hq0_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hq0_c%d.pdf",         i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hq0_Brandon->Clone(Form("hq0_c%d_sig",i));
      h_ratio->Divide(hq0_Brandon,hq0_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hq0_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hq0_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

    //q3
    hq3_Brandon->SetLineColor(kRed);
    {
      hq3_Brandon->Draw("hist");
      hq3_Carrie ->Draw("histsame");
      outfile = Form("plots_compare/hq3_c%d.pdf",         i);
      c1->Print(outfile.c_str());
    }
    {
      TH1* h_ratio=(TH1*)hq3_Brandon->Clone(Form("hq3_c%d_sig",i));
      h_ratio->Divide(hq3_Brandon,hq3_Carrie);

      double rebin_factor = 1.;
      TH1* h_ratio_rebin=(TH1*)h_ratio->Clone(Form("hq3_c%d_sig",i));
      h_ratio_rebin->Rebin(rebin_factor);
      h_ratio_rebin->Scale(1./rebin_factor);
      h_ratio_rebin->SetLineColor(kBlack);
      h_ratio_rebin->SetTitle("Brandon/Carrie");
      h_ratio_rebin->SetMinimum(0.7);
      h_ratio_rebin->SetMaximum(2.0);
      h_ratio_rebin->Draw("hist");
      outfile = Form("plots_compare/ratio_hq3_c%d.pdf",i);
      c1->Print(outfile.c_str());

    }

  }
}
  

    /*
    hTpi_Carrie        ->SetTitle(Form("Reminaing signal events after Cut: %s;T_{#pi} (GeV) ", cut_names[i].c_str()));
    hCosThetapi_Carrie ->SetTitle(Form("Reminaing signal events after Cut: %s;cos(#theta_{#pi}) ", cut_names[i].c_str()));
    hPmu_Carrie        ->SetTitle(Form("Reminaing signal events after Cut: %s;P_{#mu} (GeV) ", cut_names[i].c_str()));
    hCosThetamu_Carrie ->SetTitle(Form("Reminaing signal events after Cut: %s;cos(#theta_{#mu}) ", cut_names[i].c_str()));
    hW_Carrie          ->SetTitle(Form("Reminaing signal events after Cut: %s;W (GeV) ", cut_names[i].c_str()));
    hQ2_Carrie         ->SetTitle(Form("Reminaing signal events after Cut: %s;Q^{2} (GeV^{2}) ", cut_names[i].c_str()));
    hq%d_Carrie        ->SetTitle(Form("Reminaing signal events after Cut: %s;q_{0} (GeV) ", cut_names[i].c_str()));
    hq3_Carrie         ->SetTitle(Form("Reminaing signal events after Cut: %s;|q_{3}| (GeV) ", cut_names[i].c_str()));
   */


  /*

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
*/

    //double temp_array[] = {0., 2., 4., 6., 8., 10., 12., 15, 20., 30.};
    //make array from vector
    //int rebins_size = rebins_vec.size()-1;
    //std::cout << "rebins_size = " << rebins_size << std::endl;
    //double rebins_array[rebins_size];
    //std::copy(rebins_vec.begin(), rebins_vec.end(), rebins_array);
    
    /*
    TH1 *hnew = h_uncertainty->Rebin(rebins_vec.size()-1, 
                                     Form("%s_Uncertainty_%s_rebin", 
                                            antinu_str.c_str(), 
                                            uncertainty.c_str() ), 
                                            &rebins_vec[0]);
    
    for(int i = 1; i<hnew->GetNbinsX()+1; ++i){
      hnew->SetBinContent(i, hnew->GetBinContent(i)/(hnew->GetBinWidth(i)/0.5));
    }
    hnew->Draw();
    */
