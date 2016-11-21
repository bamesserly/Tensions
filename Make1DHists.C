#include "CommonIncludes.C"

using namespace std;
const bool W_RESTRICTIONS = true;
const bool T2K_BINNING = false;
const bool USE_CARRIE_SIGNAL = true;
const bool USE_ANISO_WEIGHT = false;

void Make1DHists()
{
  string binning_str = T2K_BINNING       ? "T2Kbinning"   : "minervabinning";
  string W_str       = W_RESTRICTIONS    ? ""             : "_NoWRestrictions";
  string signal_str  = USE_CARRIE_SIGNAL ? "_CarrieSignal" : "_BrandonSignal";
  
  string f_out_name = "1DHists_" + binning_str + W_str + signal_str + ".root";
  //f_out_name = "test.root";

  cout << "Outfile: " << f_out_name.c_str() << endl;
  
  gStyle->SetOptStat(0);

  TFile * f_in = TFile::Open("Minerva_CC1Pi_1.root","READ");

  //Get Truth Tree branches for truth signal
  TTree* truthtree = 0;
  f_in->GetObject("CCNuPionInc",truthtree);
  bool truth_isFidVol = false;
  int mc_current = 0;
  int mc_incoming = 0;

  double mc_incomingE = 0;
  double mc_w = 0;
  int truth_N_pip = 0;
  int truth_N_pim = 0;
  double truth_genie_wgt_Theta_Delta2Npi[10];

  double mc_cvweight_total;

  double truth_pi_E[20];



  double truth_pi_theta_wrtbeam[20];
  double truth_mu_E;
  double truth_mu_theta_wrtbeam;
  double mc_Q2;

  double truth_mu_px, truth_mu_py, truth_mu_pz;
  double mc_primFSLepton[4];

  truthtree->SetBranchAddress("truth_isFidVol",&truth_isFidVol);
  truthtree->SetBranchAddress("mc_current",&mc_current);
  truthtree->SetBranchAddress("mc_incoming",&mc_incoming);
  truthtree->SetBranchAddress("mc_incomingE",&mc_incomingE);

  truthtree->SetBranchAddress("mc_cvweight_total", &mc_cvweight_total);
  truthtree->SetBranchAddress("mc_w",&mc_w);
  truthtree->SetBranchAddress("mc_Q2",&mc_Q2);

  truthtree->SetBranchAddress("truth_N_pip",&truth_N_pip);
  truthtree->SetBranchAddress("truth_N_pim",&truth_N_pim);
  truthtree->SetBranchAddress("truth_pi_E", truth_pi_E);



  truthtree->SetBranchAddress("truth_pi_theta_wrtbeam", truth_pi_theta_wrtbeam);
  truthtree->SetBranchAddress("truth_mu_E", &truth_mu_E);



  truthtree->SetBranchAddress("truth_mu_theta_wrtbeam", &truth_mu_theta_wrtbeam); 
  truthtree->SetBranchAddress("truth_genie_wgt_Theta_Delta2Npi", truth_genie_wgt_Theta_Delta2Npi);

  truthtree->SetBranchAddress("truth_mu_px", &truth_mu_px);
  truthtree->SetBranchAddress("truth_mu_py", &truth_mu_py);
  truthtree->SetBranchAddress("truth_mu_pz", &truth_mu_pz);

  truthtree->SetBranchAddress("mc_primFSLepton", mc_primFSLepton);

  //Get Reco Tree
  TTree* recotree = 0;
  f_in->GetObject("reco",recotree);
  double Ev_GeV = -999.;
  recotree->SetBranchAddress("Ev_GeV",&Ev_GeV);
  double total_weight = 1.0;
  double POTScale, norm_correction;
  recotree->SetBranchAddress("total_weight",&total_weight);
  recotree->SetBranchAddress("POTScale", &POTScale);
  recotree->SetBranchAddress("norm_correction", &norm_correction);

  //Get Cuttree
  TTree* cuttree = 0;
  f_in->GetObject("Cuts",cuttree);
  bool pass_AllCuts = false;
  bool pass_minosMatchCut = false;
  bool pass_minosChargeCut = false;
  bool pass_deadTimeCut  = false; 
  bool pass_hadronicEnergyCut = false; 
  bool pass_EvCut = false;
  bool pass_nVertexProngsCut = false;
  bool pass_hadron_dEdxQualityCut = false;
  bool pass_hadron_dEdxPIDCut = false;
  bool pass_hadron_MichelQualityCut = false;
    bool pass_hadron_MichelPIDCut = false;
    bool pass_hadronCuts = false;
  bool pass_pionMultiplicityCut = false;
  cuttree->SetBranchAddress("pass_AllCuts",&pass_AllCuts);
  cuttree->SetBranchAddress("pass_minosMatchCut",&pass_minosMatchCut);
  cuttree->SetBranchAddress("pass_minosChargeCut",&pass_minosChargeCut);
  cuttree->SetBranchAddress("pass_deadTimeCut", &pass_deadTimeCut);
  cuttree->SetBranchAddress("pass_hadronicEnergyCut", &pass_hadronicEnergyCut);
  cuttree->SetBranchAddress("pass_EvCut", &pass_EvCut);
  cuttree->SetBranchAddress("pass_nVertexProngsCut", &pass_nVertexProngsCut);
  cuttree->SetBranchAddress("pass_hadronCuts",&pass_hadronCuts);
  cuttree->SetBranchAddress("pass_hadron_dEdxQualityCut",&pass_hadron_dEdxQualityCut);
  cuttree->SetBranchAddress("pass_hadron_dEdxPIDCut",&pass_hadron_dEdxPIDCut);
  cuttree->SetBranchAddress("pass_hadron_MichelQualityCut",&pass_hadron_MichelQualityCut);
  cuttree->SetBranchAddress("pass_hadron_MichelPIDCut",&pass_hadron_MichelPIDCut);
  cuttree->SetBranchAddress("pass_pionMultiplicityCut", &pass_pionMultiplicityCut);

  //Output file
  TFile* f_out = new TFile(f_out_name.c_str(), "RECREATE");

  bool uncut = true;
  bool pass_dEdxCuts = false;
  bool pass_MichelCuts = false;

  unsigned int NCUTS;
  std::string INFILE;
  std::string PLOTDIR; 
  const std::string* cut_names;
  std::vector<bool*> cut_vals;

  if(W_RESTRICTIONS){
    NCUTS = 9;

    const std::string cut_names_W[]= {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
    cut_names = cut_names_W;
    
    cut_vals.push_back(&uncut);
    cut_vals.push_back(&pass_minosMatchCut);
    cut_vals.push_back(&pass_minosChargeCut);
    cut_vals.push_back(&pass_deadTimeCut);
    cut_vals.push_back(&pass_hadronicEnergyCut);
    cut_vals.push_back(&pass_EvCut);
    cut_vals.push_back(&pass_nVertexProngsCut);
    cut_vals.push_back(&pass_dEdxCuts);
    cut_vals.push_back(&pass_MichelCuts);
    cut_vals.push_back(&pass_pionMultiplicityCut);

  }
  else{
    NCUTS = 8;

    const std::string cut_names_noW[] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
    cut_names = cut_names_noW;

    cut_vals.push_back(&uncut);
    cut_vals.push_back(&pass_minosMatchCut);
    cut_vals.push_back(&pass_minosChargeCut);
    cut_vals.push_back(&pass_deadTimeCut);
    cut_vals.push_back(&pass_EvCut);
    cut_vals.push_back(&pass_nVertexProngsCut);
    cut_vals.push_back(&pass_dEdxCuts);
    cut_vals.push_back(&pass_MichelCuts);
    cut_vals.push_back(&pass_pionMultiplicityCut);
  }

  //Output histograms
  TH1D* hTpi_sig[NCUTS+1];
  TH1D* hCosThetapi_sig[NCUTS+1];
  TH1D* hThetapi_sig[NCUTS+1];
  TH1D* hPmu_sig[NCUTS+1];
  TH1D* hCosThetamu_sig[NCUTS+1];
  TH1D* hThetamu_sig[NCUTS+1];
  TH1D* hWexp_sig[NCUTS+1];
  TH1D* hWgenie_sig[NCUTS+1];
  TH1D* hQ2exp_sig[NCUTS+1];
  TH1D* hQ2genie_sig[NCUTS+1];
  TH1D* hq0_sig[NCUTS+1];
  TH1D* hq3_sig[NCUTS+1];

  TH1D* hTpi_all[NCUTS+1];
  TH1D* hCosThetapi_all[NCUTS+1];
  TH1D* hThetapi_all[NCUTS+1];
  TH1D* hPmu_all[NCUTS+1];
  TH1D* hCosThetamu_all[NCUTS+1];
  TH1D* hThetamu_all[NCUTS+1];
  TH1D* hWexp_all[NCUTS+1];
  TH1D* hWgenie_all[NCUTS+1];
  TH1D* hQ2exp_all[NCUTS+1];
  TH1D* hQ2genie_all[NCUTS+1];
  TH1D* hq0_all[NCUTS+1];
  TH1D* hq3_all[NCUTS+1];


  //TODO in principle, use if (T2Kbinning) for second scheme
  for (unsigned int c=0; c!=NCUTS+1; ++c) {
    //hTpi_num[c]        = new TH1D(Form("hTpi_c%d_num",c),";T_{#pi} (MeV)", 50, 0.0, 5000.0);
    hTpi_sig[c]        = new TH1D(Form("hTpi_c%d_sig",c),        Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV)",     cut_names[c].c_str()), 150, 0.0, 0.75);
    hTpi_all[c]        = new TH1D(Form("hTpi_c%d_all",c),        Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV)",     cut_names[c].c_str()), 150, 0.0, 0.75);

    hCosThetapi_sig[c] = new TH1D(Form("hCosThetapi_c%d_sig",c), Form("Cumulative Efficiency, Cut: %s;cos(#theta_{#pi})", cut_names[c].c_str()), 50, -1., 1.);
    hCosThetapi_all[c] = new TH1D(Form("hCosThetapi_c%d_all",c), Form("Cumulative Efficiency, Cut: %s;cos(#theta_{#pi})", cut_names[c].c_str()), 50, -1., 1.);

    hThetapi_sig[c]    = new TH1D(Form("hThetapi_c%d_sig",c),    Form("Cumulative Efficiency, Cut: %s;#theta_{#pi}",      cut_names[c].c_str()), 50, -0.1, 3.14);
    hThetapi_all[c]    = new TH1D(Form("hThetapi_c%d_all",c),    Form("Cumulative Efficiency, Cut: %s;#theta_{#pi}",      cut_names[c].c_str()), 50, -0.1, 3.14);

    hPmu_sig[c]        = new TH1D(Form("hPmu_c%d_sig",c),        Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV)",     cut_names[c].c_str()), 50, 0.0, 10.0);
    hPmu_all[c]        = new TH1D(Form("hPmu_c%d_all",c),        Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV)",     cut_names[c].c_str()), 50, 0.0, 10.0);

    hCosThetamu_sig[c] = new TH1D(Form("hCosThetamu_c%d_sig",c), Form("Cumulative Efficiency, Cut: %s;cos(#theta_{#mu})", cut_names[c].c_str()), 50, 0.8, 1.);
    hCosThetamu_all[c] = new TH1D(Form("hCosThetamu_c%d_all",c), Form("Cumulative Efficiency, Cut: %s;cos(#theta_{#mu})", cut_names[c].c_str()), 50, 0.8, 1.);

    hThetamu_sig[c]    = new TH1D(Form("hThetamu_c%d_sig",c),    Form("Cumulative Efficiency, Cut: %s;#theta_{#mu}",      cut_names[c].c_str()), 50, 0, 0.65);
    hThetamu_all[c]    = new TH1D(Form("hThetamu_c%d_all",c),    Form("Cumulative Efficiency, Cut: %s;#theta_{#mu}",      cut_names[c].c_str()), 50, 0, 0.65);

    hWexp_sig[c]       = new TH1D(Form("hWexp_c%d_sig",c),       Form("Cumulative Efficiency, Cut: %s;W_(exp) (GeV)",        cut_names[c].c_str()), 50, 0., 1.8);
    hWexp_all[c]       = new TH1D(Form("hWexp_c%d_all",c),       Form("Cumulative Efficiency, Cut: %s;W_(exp) (GeV)",        cut_names[c].c_str()), 50, 0., 1.8);

    hWgenie_sig[c]     = new TH1D(Form("hWgenie_c%d_sig",c),     Form("Cumulative Efficiency, Cut: %s;W_(genie) (GeV)",   cut_names[c].c_str()), 50, 0., 1.8);
    hWgenie_all[c]     = new TH1D(Form("hWgenie_c%d_all",c),     Form("Cumulative Efficiency, Cut: %s;W_(genie) (GeV)",   cut_names[c].c_str()), 50, 0., 1.8);

    hQ2exp_sig[c]         = new TH1D(Form("hQ2exp_c%d_sig",c),         Form("Cumulative Efficiency, Cut: %s;Q^{2} exp (GeV^{2})",   cut_names[c].c_str()), 50, 0.0, 3.0);
    hQ2exp_all[c]         = new TH1D(Form("hQ2exp_c%d_all",c),         Form("Cumulative Efficiency, Cut: %s;Q^{2} exp (GeV^{2})",   cut_names[c].c_str()), 50, 0.0, 3.0);

    hQ2genie_sig[c]         = new TH1D(Form("hQ2genie_c%d_sig",c),         Form("Cumulative Efficiency, Cut: %s;Q^{2} genie (GeV^{2})",   cut_names[c].c_str()), 50, 0.0, 3.0);
    hQ2genie_all[c]         = new TH1D(Form("hQ2genie_c%d_all",c),         Form("Cumulative Efficiency, Cut: %s;Q^{2} genie (GeV^{2})",   cut_names[c].c_str()), 50, 0.0, 3.0);

    hq0_sig[c]         = new TH1D(Form("hq0_c%d_sig",c),         Form("Cumulative Efficiency, Cut: %s;q_{0} (GeV)",       cut_names[c].c_str()),  50, 0.0, 3.0);
    hq0_all[c]         = new TH1D(Form("hq0_c%d_all",c),         Form("Cumulative Efficiency, Cut: %s;q_{0} (GeV)",       cut_names[c].c_str()),  50, 0.0, 3.0);

    hq3_sig[c]         = new TH1D(Form("hq3_c%d_sig",c),         Form("Cumulative Efficiency, Cut: %s;|q_{3}| (GeV)",     cut_names[c].c_str()),  50, 0.0, 3.0);
    hq3_all[c]         = new TH1D(Form("hq3_c%d_all",c),         Form("Cumulative Efficiency, Cut: %s;|q_{3}| (GeV)",     cut_names[c].c_str()),  50, 0.0, 3.0);
  }

  //Loop entries
  double nentries = truthtree->GetEntries();
  cout << "nentries = " << nentries << endl;
  unsigned long int nsignal = 0;
  unsigned long int ncut = 0;
  double weighted_nsignal = 0;
  for(int i=0; i<nentries; ++i){
    truthtree->GetEntry(i);
    cuttree->GetEntry(i);
    recotree->GetEntry(i);

    if(i%50000==0){
      std::cout << i << endl;
    }

    //set combined cut values
    pass_dEdxCuts = pass_hadron_dEdxQualityCut && pass_hadron_dEdxPIDCut;
    pass_MichelCuts = pass_hadron_MichelQualityCut && pass_hadron_MichelPIDCut; 

    //determine weights
    double aniso_weight = 0.5*(1.0 + truth_genie_wgt_Theta_Delta2Npi[4]);
    //double tot_weight = mc_cvweight_total*aniso_weight*POTScale*norm_correction;
    double tot_weight = total_weight;

    //Signal Definition
    bool is_true_signal = false; //truth_isFidVol && mc_current==1 && mc_incoming==14 && mc_incomingE <= 10.0e3 && mc_incomingE >= 1.5e3 && truth_N_pip+truth_N_pim == 1 && analysis_W < 1400;

    double W_genie_GeV = mc_w*0.001;
    double E_hadronic_system_MeV = mc_incomingE-mc_primFSLepton[3];
    double Q2_true_MeV = CalcQ2GeV(mc_incomingE, truth_mu_px, truth_mu_py, truth_mu_pz, truth_mu_theta_wrtbeam);
    double W_true_exp_GeV = CalcWexpGeV(E_hadronic_system_MeV, Q2_true_MeV);
    double analysis_W = USE_CARRIE_SIGNAL ? W_true_exp_GeV : W_genie_GeV;

    //std::cout << W_true_exp_GeV << ", " << W_genie_GeV << std::endl;
    //std::cout << "  " << Q2_true_MeV << ", " << (mc_Q2)*1.0e-6 << std::endl;

    if(W_RESTRICTIONS){
      is_true_signal = truth_isFidVol && mc_current==1 && mc_incoming==14 && mc_incomingE <= 10.0e3 && mc_incomingE >= 1.5e3 &&
                          truth_N_pip+truth_N_pim == 1 && analysis_W < 1.4000;
    }
    else{
      is_true_signal = truth_isFidVol && mc_current==1 && mc_incoming==14 && mc_incomingE <= 10.0e3 && mc_incomingE >= 1.5e3 &&
                          truth_N_pip+truth_N_pim == 1;
    }


















    //loop through cuts 
    for (unsigned int c=0; c!=NCUTS+1; ++c) {
      if ( *cut_vals[c] ) {
        if (is_true_signal) {
          //std::cout << W_true_exp_GeV <<  std::endl;

          hTpi_sig[c]->Fill((truth_pi_E[0] - 139.0)*.001,  tot_weight);
          hCosThetapi_sig[c]->Fill(cos(truth_pi_theta_wrtbeam[0]),  tot_weight);
          hThetapi_sig[c]->Fill(truth_pi_theta_wrtbeam[0],  tot_weight);
          hPmu_sig[c]->Fill(sqrt(truth_mu_E*truth_mu_E - 105.6*105.6)*0.001, tot_weight);
          hCosThetamu_sig[c]->Fill(cos(truth_mu_theta_wrtbeam), tot_weight);
          hThetamu_sig[c]->Fill(truth_mu_theta_wrtbeam, tot_weight);
          hWexp_sig[c]->Fill(W_true_exp_GeV, tot_weight);
          hWgenie_sig[c]->Fill(W_genie_GeV, tot_weight);
          hQ2exp_sig[c]->Fill(Q2_true_MeV, tot_weight);
          hQ2genie_sig[c]->Fill(mc_Q2*1.0e-6, tot_weight);
          hq0_sig[c]->Fill((mc_incomingE - truth_mu_E)*0.001, tot_weight);
          hq3_sig[c]->Fill(sqrt(mc_Q2 + pow(mc_incomingE - truth_mu_E,2.0))*0.001, tot_weight);


          if (c==NCUTS) ncut++;
        }

        hTpi_all[c]->Fill((truth_pi_E[0] - 139.0)*.001,  tot_weight);
        hCosThetapi_all[c]->Fill(cos(truth_pi_theta_wrtbeam[0]),  tot_weight);
        hThetapi_all[c]->Fill(truth_pi_theta_wrtbeam[0],  tot_weight);
        hPmu_all[c]->Fill(sqrt(truth_mu_E*truth_mu_E - 105.6*105.6)*0.001, tot_weight);
        hCosThetamu_all[c]->Fill(cos(truth_mu_theta_wrtbeam), tot_weight);
        hThetamu_all[c]->Fill(truth_mu_theta_wrtbeam, tot_weight);
        hWgenie_all[c]->Fill(W_genie_GeV, tot_weight);
        hWexp_all[c]->Fill(W_true_exp_GeV, tot_weight);
        hQ2genie_all[c]->Fill(mc_Q2*1.0e-6, tot_weight);
        hQ2exp_all[c]->Fill(Q2_true_MeV, tot_weight);
        hq0_all[c]->Fill((mc_incomingE - truth_mu_E)*0.001, tot_weight);
        hq3_all[c]->Fill(sqrt(mc_Q2 + pow(mc_incomingE - truth_mu_E,2.0))*0.001, tot_weight);

      }

      else break;
    } //end cut loop

    if (is_true_signal) {   
      nsignal++;
      weighted_nsignal += tot_weight;
    }
  } //end entry loop

  std::cout<<"Unweighted Efficiency: "<<ncut<<"/"<<nsignal<<std::endl;
  std::cout<<"Weighted EFficiency: "<<hTpi_sig[NCUTS]->Integral(0, hTpi_sig[NCUTS]->GetNbinsX()+1)<<"/"
                                    <<hTpi_sig[0]->Integral(0, hTpi_sig[0]->GetNbinsX()+1)<<std::endl;


  f_out->Write(0,TObject::kOverwrite);
  f_out->Close();
}

//std::cout<<"Unweighted Efficiency: "<<ncut<<"/"<<nsignal<<std::endl;
//std::cout<<"Weighted EFficiency: "<<hTvTheta_pi_num[NCUTS]->Integral(0, hTvTheta_pi_num[NCUTS]->GetNbinsX()+1, 0, hTvTheta_pi_num[NCUTS]->GetNbinsY()+1)<<"/"
//  <<hTvTheta_pi_num[0]->Integral(0, hTvTheta_pi_num[0]->GetNbinsX()+1, 0, hTvTheta_pi_num[0]->GetNbinsY()+1)<<std::endl;

//hTvTheta_pi_num->Write();
//hTvTheta_pi_den->Write();

//Other branches
/*
   double Ev_GeV = -999., Emu_GeV = -999., MuonPt_GeV = -999., MuonPz_GeV = -999.;
   double ThetaMu_Rad = -999., Ehad_GeV = -999., Q2_GeV = -999., Wexp_GeV = -999.;
   vector<double>* KE_HadronPion_GeV = 0;
   vector<double>* Theta_HadronPion_Rad = 0; 
   vector<double>* OpeningAngle_Deg = 0;
   double n_passed_pions = -999.;
   double total_weight = 1.0;

   recotree->SetBranchAddress("Ev_GeV",&Ev_GeV);
   recotree->SetBranchAddress("Emu_GeV",&Emu_GeV);
   recotree->SetBranchAddress("MuonPt_GeV",&MuonPt_GeV);
   recotree->SetBranchAddress("MuonPz_GeV",&MuonPz_GeV);
   recotree->SetBranchAddress("ThetaMu_Rad",&ThetaMu_Rad);
   recotree->SetBranchAddress("Ehad_GeV",&Ehad_GeV);
   recotree->SetBranchAddress("Q2_GeV",&Q2_GeV);
   recotree->SetBranchAddress("Wexp_GeV",&Wexp_GeV);
   recotree->SetBranchAddress("KE_HadronPion_GeV",&KE_HadronPion_GeV);
   recotree->SetBranchAddress("Theta_HadronPion_Rad",&Theta_HadronPion_Rad);
   recotree->SetBranchAddress("OpeningAngle_Deg",&OpeningAngle_Deg);
   recotree->SetBranchAddress("n_passed_pions",&n_passed_pions);
   recotree->SetBranchAddress("total_weight",&total_weight);

  //std::string cut_names[NCUTS+1] = {"none","minosMatchQuality", "muonCharge", "deadTime", "RecoW", "RecoEv", "nVertexTracks", "dEdx and Michel", "ExactlyOneRecoPion"};
  //std::string cut_names[NCUTS+1] = {"none","minosMatchQuality", "muonCharge", "deadTime", "RecoW", "RecoEv", "nVertexTracks", "dEdx", "Michel", "ExactlyOneRecoPion"};
  std::string cut_names[NCUTS+1] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
  //bool* cut_vals[NCUTS+1] = {&uncut,&pass_minosMatchCut, &pass_minosChargeCut, &pass_deadTimeCut, &pass_hadronicEnergyCut, &pass_EvCut, &pass_nVertexProngsCut, &pass_hadronCuts, &pass_pionMultiplicityCut};
  //
  

  bool* cut_vals[NCUTS+1] = {&uncut,&pass_minosMatchCut, &pass_minosChargeCut, 
                             &pass_deadTimeCut, &pass_hadronicEnergyCut, 
                             &pass_EvCut, &pass_nVertexProngsCut, &pass_PID, 
                             &pass_michel, &pass_pionMultiplicityCut};
*/

  //gROOT->ProcessLine(".x CommonIncludes.C");
  //
  //ROOT::Cintex::Cintex::Enable();
  //TH1::AddDirectory(false);
  //bool pass_PID = false;
  //bool pass_michel = false;
