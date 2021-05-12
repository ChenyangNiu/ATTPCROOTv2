void print(std::vector <int> const &a) {
   std::cout << "The vector elements are : ";

   for(int i=0; i < a.size(); i++)
   std::cout << a.at(i) << ' ';
}

void test(int tpcRun = 200)
{
  
  TChain tpc_tree("cbmsim");
  tpc_tree.Add(TString::Format("/mnt/analysis/e12014/TPC/unpacked/run_%04d.root", tpcRun));
  
  TTreeReader reader(&tpc_tree);
  TTreeReaderValue<TClonesArray> event(reader, "AtEventH");

  //Open file and skip header
  std::ifstream events_file("findEvents_" + std::to_string(tpcRun) + ".dat");
  if(!events_file.good())
    std::cout << "Failed to open file" << std::endl;;

  //Create pad plane
  auto fAtMapPtr = new AtTpcMap();
  fAtMapPtr->GenerateATTPC();
  auto fPadPlane = fAtMapPtr->GetATTPCPlane();
  double fThreshold = 0;
  
  //Get the event
  //while(events_file.good())
  while(reader.Next())
  {

    /*
    int entry;
    events_file >> entry;
    //std::cout << "Adding entry: " << entry << endl;
    reader.SetEntry(entry);
    */
    auto eventPtr = (AtEvent*) (event->At(0));

    auto numHits = eventPtr->GetNumHits();
    for( int i = 0; i < numHits; ++i)
    {
      AtHit hit = eventPtr->GetHitArray()->at(i);
      if(hit.GetCharge() < fThreshold) continue;
    
      TVector3 position = hit.GetPosition();
      fPadPlane->Fill(position.X(), position.Y(), 1);
    }
  }
  std::cout << "Done with loop" << endl;
  //fPadPlane->Draw("COL L0");
  //fPadPlane -> Draw("COL L0");
  // fPadPlane -> SetMinimum(1.0);
  // gStyle->SetOptStat(0);
  // gStyle->SetPalette(103);
  // gPad ->Update();

  auto ppp = fAtMapPtr->GetPadRef(9908);
  print(ppp);

  std::cout << "finish" << std::endl;

}
