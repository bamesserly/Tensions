#include "CommonIncludes.C"

using namespace std;
const bool W_RESTRICTIONS = true;
const bool T2K_BINNING = false;
const bool USE_CARRIE_SIGNAL = false;
const bool USE_ANISO_WEIGHT = false;

void Make2DHists()
{
  string binning_str = T2K_BINNING       ? "T2Kbinning"   : "minervabinning";
  string W_str       = W_RESTRICTIONS    ? ""             : "_NoWRestrictions";
  string signal_str  = USE_CARRIE_SIGNAL ? "_CarrieSignal" : "_BrandonSignal";
  
  string f_out_name = "2DHists_" + binning_str + W_str + signal_str + ".root";
  f_out_name = "test.root";

  cout << "Outfile: " << f_out_name.c_str() << endl;

  gStyle->SetOptStat(0);

  TFile * f_in = TFile::Open("Minerva_CC1Pi_1.root","READ");

  //Get Truth Tree branches for truth signal
  TTree* truthtree = 0;
  f_in->GetObject("CCNuPionInc",truthtree);
  bool truth_isFidVol = false;
  int mc_current = 0;
  int mc_incoming = 0;
  double mc_incomingPartVec[4];
  double mc_incomingE = 0;
  double mc_w = 0;
  int truth_N_pip = 0;
  int truth_N_pim = 0;
  double truth_genie_wgt_Theta_Delta2Npi[10];
  
  double mc_cvweight_total;
  
  double truth_pi_E[20];
  double truth_pi_px[20];
  double truth_pi_py[20];
  double truth_pi_pz[20];
  double truth_pi_theta_wrtbeam[20];
  double truth_mu_E, truth_mu_px, truth_mu_py, truth_mu_pz;
  double truth_mu_theta_wrtbeam;
  double mc_Q2;
  



  truthtree->SetBranchAddress("truth_isFidVol",&truth_isFidVol);
  truthtree->SetBranchAddress("mc_current",&mc_current);
  truthtree->SetBranchAddress("mc_incoming",&mc_incoming);
  truthtree->SetBranchAddress("mc_incomingE",&mc_incomingE);
  truthtree->SetBranchAddress("mc_incomingPartVec",mc_incomingPartVec);
  truthtree->SetBranchAddress("mc_cvweight_total", &mc_cvweight_total);
  truthtree->SetBranchAddress("mc_w",&mc_w);
  truthtree->SetBranchAddress("mc_Q2",&mc_Q2);
  
  truthtree->SetBranchAddress("truth_N_pip",&truth_N_pip);
  truthtree->SetBranchAddress("truth_N_pim",&truth_N_pim);
  truthtree->SetBranchAddress("truth_pi_E", truth_pi_E);
  truthtree->SetBranchAddress("truth_pi_px", truth_pi_px);
  truthtree->SetBranchAddress("truth_pi_py", truth_pi_py);
  truthtree->SetBranchAddress("truth_pi_pz", truth_pi_pz);
  truthtree->SetBranchAddress("truth_pi_theta_wrtbeam", truth_pi_theta_wrtbeam);
  truthtree->SetBranchAddress("truth_mu_E", &truth_mu_E);
  truthtree->SetBranchAddress("truth_mu_px", &truth_mu_px);
  truthtree->SetBranchAddress("truth_mu_py", &truth_mu_py);
  truthtree->SetBranchAddress("truth_mu_pz", &truth_mu_pz);
  truthtree->SetBranchAddress("truth_mu_theta_wrtbeam", &truth_mu_theta_wrtbeam); 
  truthtree->SetBranchAddress("truth_genie_wgt_Theta_Delta2Npi", truth_genie_wgt_Theta_Delta2Npi);





 

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
  TH2D* hTvTheta_pi_sig[NCUTS+1];  
  TH2D* hPvTheta_mu_sig[NCUTS+1];  
  TH2D* hWvQ2_sig[NCUTS+1];  
  TH2D* hWexpvQ2_sig[NCUTS+1];
  TH2D* hq0vq3_sig[NCUTS+1];
  TH2D* hcosPvTpi_sig[NCUTS+1];
  TH2D* hcosPvcosTpi_sig[NCUTS+1];
  TH2D* hphiPvTpi_sig[NCUTS+1];
  TH2D* hphiPvcosTpi_sig[NCUTS+1];
  
  TH2D* hTvTheta_pi_all[NCUTS+1];  
  TH2D* hPvTheta_mu_all[NCUTS+1];  
  TH2D* hWvQ2_all[NCUTS+1];  
  TH2D* hWexpvQ2_all[NCUTS+1];
  TH2D* hq0vq3_all[NCUTS+1];
  TH2D* hcosPvTpi_all[NCUTS+1];
  TH2D* hcosPvcosTpi_all[NCUTS+1];
  TH2D* hphiPvTpi_all[NCUTS+1];
  TH2D* hphiPvcosTpi_all[NCUTS+1];
  
  if (T2K_BINNING){
    for (unsigned int c=0; c!=NCUTS+1; ++c) {
      hTvTheta_pi_sig[c] = new TH2D(Form("hTvTheta_c%d_pi_sig",c),Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi})",cut_names[c].c_str()),200, 0.0, 5.0, 50, -1.0, 1.0);    
      hTvTheta_pi_all[c] = new TH2D(Form("hTvTheta_c%d_pi_all",c),Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi})",cut_names[c].c_str()),200, 0.0, 5.0, 50, -1.0, 1.0);    

      hPvTheta_mu_sig[c] = new TH2D(Form("hPvTheta_c%d_mu_sig",c),Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu})",cut_names[c].c_str()),50, 0.0, 10.0, 50, -1.0, 1.0);    
      hPvTheta_mu_all[c] = new TH2D(Form("hPvTheta_c%d_mu_all",c),Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu})",cut_names[c].c_str()),50, 0.0, 10.0, 50, -1.0, 1.0);    

      hWvQ2_sig[c] = new TH2D(Form("hWvQ2_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.0, 10.0);
      hWvQ2_all[c] = new TH2D(Form("hWvQ2_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.0, 10.0);

      hWexpvQ2_sig[c] = new TH2D(Form("hWexpvQ2_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 10., 50, 0.0, 10.0);
      hWexpvQ2_all[c] = new TH2D(Form("hWexpvQ2_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 10., 50, 0.0, 10.0);

      hq0vq3_sig[c] = new TH2D(Form("hq0vq3_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.0, 10.0);
      hq0vq3_all[c] = new TH2D(Form("hq0vq3_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.0, 10.0);

      //T pi vs theta adler
      hcosPvTpi_sig[c] = new TH2D(Form("hcosPvTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});T_{#pi} (GeV)",cut_names[c].c_str()), 50, -1.0, 1.0, 200, 0.0, 5.);//1A
      hcosPvTpi_all[c] = new TH2D(Form("hcosPvTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});T_{#pi} (GeV)",cut_names[c].c_str()), 50, -1.0, 1.0, 200, 0.0, 5.);//1A

      //Tpi vs phi adler
      hphiPvTpi_sig[c] = new TH2D(Form("hphiPvTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};T_{#pi} (GeV) (rad)",cut_names[c].c_str()), 50, -3.2, 3.2, 200, 0.0, 5.);//2A
      hphiPvTpi_all[c] = new TH2D(Form("hphiPvTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};T_{#pi} (GeV) (rad)",cut_names[c].c_str()), 50, -3.2, 3.2, 200, 0.0, 5.);//2A
      
      //theta pi vs theta adler
      hcosPvcosTpi_sig[c] = new TH2D(Form("hcosPvcosTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});cos(#theta_{#pi})",cut_names[c].c_str()),50, -1.0, 1.0, 50, -1.0, 1.0);//3A
      hcosPvcosTpi_all[c] = new TH2D(Form("hcosPvcosTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});cos(#theta_{#pi})",cut_names[c].c_str()),50, -1.0, 1.0, 50, -1.0, 1.0);//3A

      //theta pi vs phi adler
      hphiPvcosTpi_sig[c] = new TH2D(Form("hphiPvcosTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};cos(#theta_{#pi}) (rad)",cut_names[c].c_str()),50, -3.2, 3.2, 50, -1., 1.);//4A
      hphiPvcosTpi_all[c] = new TH2D(Form("hphiPvcosTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};cos(#theta_{#pi}) (rad)",cut_names[c].c_str()),50, -3.2, 3.2, 50, -1., 1.);//4A
    }
  }

  else {
    for (unsigned int c=0; c!=NCUTS+1; ++c) {
      hTvTheta_pi_sig[c] = new TH2D(Form("hTvTheta_c%d_pi_sig",c),Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi})",cut_names[c].c_str()),200, 0.0, 0.75, 50, -1.0, 1.0);    
      hTvTheta_pi_all[c] = new TH2D(Form("hTvTheta_c%d_pi_all",c),Form("Cumulative Efficiency, Cut: %s;T_{#pi} (GeV);cos(#theta_{#pi})",cut_names[c].c_str()),200, 0.0, 0.75, 50, -1.0, 1.0);    

      hPvTheta_mu_sig[c] = new TH2D(Form("hPvTheta_c%d_mu_sig",c),Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu})",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.8, 1.0);    
      hPvTheta_mu_all[c] = new TH2D(Form("hPvTheta_c%d_mu_all",c),Form("Cumulative Efficiency, Cut: %s;P_{#mu} (GeV);cos(#theta_{#mu})",cut_names[c].c_str()),50, 0.0, 10.0, 50, 0.8, 1.0);    

      hWvQ2_sig[c] = new TH2D(Form("hWvQ2_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 1.8, 50, 0.0, 3.0);
      hWvQ2_all[c] = new TH2D(Form("hWvQ2_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;W_{gen} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 1.8, 50, 0.0, 3.0);

      hWexpvQ2_sig[c] = new TH2D(Form("hWexpvQ2_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 1.8, 50, 0.0, 3.0);
      hWexpvQ2_all[c] = new TH2D(Form("hWexpvQ2_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;W_{exp} (GeV);Q_{gen}^{2} (GeV^{2})",cut_names[c].c_str()),50, 0.0, 1.8, 50, 0.0, 3.0);

      hq0vq3_sig[c] = new TH2D(Form("hq0vq3_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()),50, 0.0, 3.0, 50, 0.0, 3.0);
      hq0vq3_all[c] = new TH2D(Form("hq0vq3_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;q_{0,gen} (GeV);|q_{3,gen}| (GeV)",cut_names[c].c_str()),50, 0.0, 3.0, 50, 0.0, 3.0);

      //T pi vs theta adler
      hcosPvTpi_sig[c] = new TH2D(Form("hcosPvTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});T_{#pi} (GeV)",cut_names[c].c_str()), 50, -1.0, 1.0, 200, 0.0, 0.75);//1A
      hcosPvTpi_all[c] = new TH2D(Form("hcosPvTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});T_{#pi} (GeV)",cut_names[c].c_str()), 50, -1.0, 1.0, 200, 0.0, 0.75);//1A

      //Tpi vs phi adler
      hphiPvTpi_sig[c] = new TH2D(Form("hphiPvTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};T_{#pi} (GeV) (rad)",cut_names[c].c_str()), 50, -3.2, 3.2, 200, 0.0, 0.75);//2A
      hphiPvTpi_all[c] = new TH2D(Form("hphiPvTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};T_{#pi} (GeV) (rad)",cut_names[c].c_str()), 50, -3.2, 3.2, 200, 0.0, 0.75);//2A
      
      //theta pi vs theta adler
      hcosPvcosTpi_sig[c] = new TH2D(Form("hcosPvcosTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});cos(#theta_{#pi})",cut_names[c].c_str()),50, -1.0, 1.0, 50, -1.0, 1.0);//3A
      hcosPvcosTpi_all[c] = new TH2D(Form("hcosPvcosTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;cos(#theta_{Adler});cos(#theta_{#pi})",cut_names[c].c_str()),50, -1.0, 1.0, 50, -1.0, 1.0);//3A

      //theta pi vs phi adler
      hphiPvcosTpi_sig[c] = new TH2D(Form("hphiPvcosTpi_c%d_sig",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};cos(#theta_{#pi}) (rad)",cut_names[c].c_str()),50, -3.2, 3.2, 50, -1., 1.);//4A
      hphiPvcosTpi_all[c] = new TH2D(Form("hphiPvcosTpi_c%d_all",c),Form("Cumulative Efficiency, Cut: %s;#phi_{Adler};cos(#theta_{#pi}) (rad)",cut_names[c].c_str()),50, -3.2, 3.2, 50, -1., 1.);//4A
    }
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
    bool is_true_signal = false;
    
    




    if(W_RESTRICTIONS){
      is_true_signal = truth_isFidVol && mc_current==1 && mc_incoming==14 && mc_incomingE <= 10.0e3 && mc_incomingE >= 1.5e3 &&
                          truth_N_pip+truth_N_pim == 1 && mc_w < 1400.0;
    }
    else{
      is_true_signal = truth_isFidVol && mc_current==1 && mc_incoming==14 && mc_incomingE <= 10.0e3 && mc_incomingE >= 1.5e3 &&
                          truth_N_pip+truth_N_pim == 1;
    }
   
    //Experimental W
    double w_exp = sqrt(938*938 - mc_Q2 + 2.0*938*(mc_incomingE-truth_mu_E));
    w_exp *= 1.0e-3;
    
    //Adler Angles
    double muonmom = sqrt(truth_mu_E*truth_mu_E - 105.6*105.6)*0.001;
    double pionmom = sqrt(truth_pi_E[0]*truth_pi_E[0] - 139.0*139.0)*0.001;
    TVector3 Neudir(mc_incomingPartVec[0], mc_incomingPartVec[1], mc_incomingPartVec[2]);
    Neudir = Neudir.Unit();
    TVector3 MuonDir(truth_mu_px, truth_mu_py, truth_mu_pz);
    MuonDir = MuonDir.Unit();
    TVector3 PionTDir(truth_pi_px[0], truth_pi_py[0], truth_pi_pz[0]);
    PionTDir = PionTDir.Unit();
    TVector3 AngAllen = AdlerAngle(2,muonmom, pionmom, Neudir, MuonDir, PionTDir, mc_incomingE*0.001);
    double phi_planar = AngAllen[2];
    double costheta_planar = cos(AngAllen[1]);
   
    //loop through cuts 
    for (unsigned int c=0; c!=NCUTS+1; ++c) {
      if ( *cut_vals[c] ) {
        if (is_true_signal) {
          hTvTheta_pi_sig[c]->Fill((truth_pi_E[0] - 139.0)*0.001, cos(truth_pi_theta_wrtbeam[0]),  tot_weight);
          hPvTheta_mu_sig[c]->Fill(muonmom, cos(truth_mu_theta_wrtbeam), tot_weight);
          hWvQ2_sig[c]->Fill(mc_w*0.001, mc_Q2*1.0e-6, tot_weight);
          hWexpvQ2_sig[c]->Fill(w_exp, mc_Q2*1.0e-6, tot_weight);
          hq0vq3_sig[c]->Fill(sqrt(mc_Q2 + pow(mc_incomingE - truth_mu_E,2.0))*0.001, (mc_incomingE - truth_mu_E)*0.001, tot_weight);
          hcosPvTpi_sig[c]->Fill(costheta_planar, (truth_pi_E[0] - 139.0)*0.001, tot_weight);//
          hphiPvTpi_sig[c]->Fill(phi_planar, (truth_pi_E[0] - 139.0)*0.001, tot_weight);//
          hcosPvcosTpi_sig[c]->Fill(costheta_planar, cos(truth_pi_theta_wrtbeam[0]), tot_weight);//
          hphiPvcosTpi_sig[c]->Fill(phi_planar, cos(truth_pi_theta_wrtbeam[0]), tot_weight);//

          if (c==NCUTS) ncut++;
        }
          
        hTvTheta_pi_all[c]->Fill((truth_pi_E[0] - 139.0)*0.001, cos(truth_pi_theta_wrtbeam[0]),  tot_weight);
        hPvTheta_mu_all[c]->Fill(muonmom, cos(truth_mu_theta_wrtbeam), tot_weight);
        hWvQ2_all[c]->Fill(mc_w*0.001, mc_Q2*1.0e-6, tot_weight);
        hWexpvQ2_all[c]->Fill(w_exp, mc_Q2*1.0e-6, tot_weight);
        hq0vq3_all[c]->Fill(sqrt(mc_Q2 + pow(mc_incomingE - truth_mu_E,2.0))*0.001, (mc_incomingE - truth_mu_E)*0.001, tot_weight);
        hcosPvTpi_all[c]->Fill(costheta_planar, (truth_pi_E[0] - 139.0)*0.001, tot_weight);//1A
        hphiPvTpi_all[c]->Fill(phi_planar, (truth_pi_E[0] - 139.0)*0.001, tot_weight);//2A
        hcosPvcosTpi_all[c]->Fill(costheta_planar, cos(truth_pi_theta_wrtbeam[0]), tot_weight);//3A
        hphiPvcosTpi_all[c]->Fill(phi_planar, cos(truth_pi_theta_wrtbeam[0]), tot_weight);//4A
      }

      else break;
    } //end cut loop
    
    if (is_true_signal) {   
      nsignal++;
      weighted_nsignal += tot_weight;
    }
  } //end entry loop
  
  std::cout<<"Unweighted Efficiency: "<<ncut<<"/"<<nsignal<<std::endl;
  std::cout<<"Weighted EFficiency: "<<hTvTheta_pi_sig[NCUTS]->Integral(0, hTvTheta_pi_sig[NCUTS]->GetNbinsX()+1, 0, hTvTheta_pi_sig[NCUTS]->GetNbinsY()+1)<<"/"
                                    <<hTvTheta_pi_sig[0]->Integral(0, hTvTheta_pi_sig[0]->GetNbinsX()+1, 0, hTvTheta_pi_sig[0]->GetNbinsY()+1)<<std::endl;


  f_out->Write(0,TObject::kOverwrite);
  f_out->Close();
}
  /*
    string OUTFILE;
    if(W_RESTRICTIONS){
      if (T2KBINNING){
        OUTFILE = "2DHists_T2Kbinning.root";
      }
      else{
        OUTFILE = "2DHists_minervabinning.root";
      }
    }
    else{
      if (T2KBINNING){
        OUTFILE = "2DHists_T2Kbinning_NOWRESTRICTIONS.root";
      }
      else{
        OUTFILE = "2DHists_minervabinning_NOWRESTRICTIONS.root";
      }
    }
    cout << "Outfile: " << OUTFILE.c_str() << endl;

  //const unsigned int NCUTS=9;
  const unsigned int NCUTS=8;
  std::string cut_names[NCUTS+1] = {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
  //std::string cut_names[NCUTS+1]= {"none","Minos Match Quality", "Muon Charge", "Dead Time", "Reco W_{exp}", "Reco Ev", "N Hadron Tracks", "Pion dEdx", "Michel", "Exactly One Reco Pion"};
  bool* cut_vals[NCUTS+1] = {&uncut,&pass_minosMatchCut, &pass_minosChargeCut, &pass_deadTimeCut, &pass_EvCut, &pass_nVertexProngsCut, &pass_dEdxCuts, &pass_MichelCuts, &pass_pionMultiplicityCut};

  //bool* cut_vals[NCUTS+1] = {&uncut,&pass_minosMatchCut, &pass_minosChargeCut, &pass_deadTimeCut, &pass_hadronicEnergyCut, &pass_EvCut, &pass_nVertexProngsCut, &pass_dEdxCuts, &pass_MichelCuts, &pass_pionMultiplicityCut};
  */

  /*
    return CalcWexpGeV( mc_incomingE-mc_primFSLepton[3], (1.0e6)*TrueQ2GeV ); //Q2 in MeV
    double Q2_true_MeV = (1.0e6)*CalcQ2GeV(mc_incomingE, 
                                           truth_mu_px,
                                           truth_mu_py,
                                           truth_mu_pz,
                                           truth_mu_theta_wrtbeam);

    double E_hadronic_system_MeV = mc_incomingE-mc_primFSLepton[3];

    double Wexp_true_GeV = CalcWexpGeV(E_hadronic_system_MeV, Q2_true_MeV);
  */
