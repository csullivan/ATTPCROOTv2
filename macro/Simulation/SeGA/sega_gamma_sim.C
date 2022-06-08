void sega_gamma_sim(Int_t nEvents = 10, TString mcEngine = "TGeant4")
{

  TString dir = getenv("VMCWORKDIR");

  // Output file name
  TString outFile ="segasim.root";

  // Parameter file name
  TString parFile="segapar.root";

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  //gSystem->Load("libAtGen.so");

  AtVertexPropagator* vertex_prop = new AtVertexPropagator();


  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName(mcEngine);              // Transport engine
  run->SetOutputFile(outFile);          // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  // -----   Create geometry   ----------------------------------------------

  FairModule* cave= new AtCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  //FairModule* magnet = new AtMagnet("Magnet");
  //run->AddModule(magnet);

  /*FairModule* pipe = new AtPipe("Pipe");
  run->AddModule(pipe);*/


  FairDetector* SeGA = new AtSeGA("SeGA", kTRUE);
  SeGA->SetGeometryFileName("SeGA.root");
  //ATTPC->SetModifyGeometry(kTRUE);
  run->AddModule(SeGA);

 // ------------------------------------------------------------------------


  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();





  // -----   Create GammaDummyGenerator
     Double_t pdgId = 22;       // 22 for gamma emission, 2212 for proton emission
     Double_t theta1 = 0.;      // polar angle distribution: lower edge (50)
     Double_t theta2 = 180.;    // polar angle distribution: upper edge (51)
     Double_t momentum = 0.001; // GeV/c
     Int_t multiplicity = 1;
     ATTPCGammaDummyGenerator* gammasGen = new ATTPCGammaDummyGenerator(pdgId, multiplicity);
     gammasGen->SetThetaRange(theta1, theta2);
     gammasGen->SetCosTheta();
     gammasGen->SetPRange(momentum, momentum);
     //gammasGen->SetDecayChainPoint(0.000513,0.1);
     //gammasGen->SetDecayChainPoint(0.000854,0.5);
     //gammasGen->SetDecayChainPoint(0.001561,0.1);
     //gammasGen->SetDecayChainPoint(0.002002,0.1);
     //gammasGen->SetDecayChainPoint(0.003750,0.2);
     gammasGen->SetPhiRange(0., 360.); //(2.5,4)
     gammasGen->SetBoxXYZ(-0.1, 0.1, -0.1, 0.1, 9.9, 10.1);
     gammasGen->SetLorentzBoost(0.0); // for instance beta=0.8197505718204776 for 700 A MeV
     // add the gamma generator
     primGen->AddGenerator(gammasGen);


     run->SetGenerator(primGen);

// ------------------------------------------------------------------------

  //---Store the visualiztion info of the tracks, this make the output file very large!!
  //--- Use it only to display but not for production!
  run->SetStoreTraj(kTRUE);



  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------

  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------

  // -----   Start run   ----------------------------------------------------
   run->Run(nEvents);

  //You can export your ROOT geometry ot a separate file
  run->CreateGeometryFile("geofile_sega_full.root");
  // ------------------------------------------------------------------------

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
       << "s" << endl << endl;
  // ------------------------------------------------------------------------
}
