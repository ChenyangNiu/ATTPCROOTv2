




// missing_pad.cpp --- 
// 
// Description: 
// Author: Chenyang Niu
// Email: niu@frib.msu.edu; niu@pku.edu.cn 
// Created: Tue Apr 13 23:37:22 2021 (-0400)
// Last-Updated: Tue Apr 27 00:52:19 2021 (-0400)
//           By: Chenyang Niu
//     Update #: 231

//#inclde "../../../AtMap/AtTpcMap.h"

using namespace std;

void print(std::vector <int> const &a) {
  std::cout << "The vector elements are : ";

  for(int i=0; i < a.size(); i++)
    std::cout << a.at(i) << ' ';
}

int missing_pad(int run_num=206)
{
  char inrootname[128];
  sprintf(inrootname, "./plane_%04d.root", run_num);
  TFile *inroot = new TFile(inrootname);
  
  if (inroot->IsZombie()) {
    cout << "Error opening file" << endl;
    return -1; }

  TH2Poly *h2p = (TH2Poly*) inroot->Get("ATTPC_Plane");
  //h2p->Draw("COL L0");
  
  //cut = (TCutG*) cut_file->FindObjectAny("CUTG"); //"CUTG"
  int Nbin = h2p->GetNumberOfBins();
  cout << Nbin << endl;


  // read mapping file
  ifstream intxt("../../../scripts/e12014_pad_map_size.csv");
  if(! intxt.is_open()) 
    cout << "./** text file open failed !!!!!" << endl;
  double temp_num;  //temporary number
  string temp_str; //temporary string
  getline(intxt, temp_str);
  int map_pad_temp[10240][6]={0};
  for(int i=0; i<10240; ++i) //10240
    {
      if (intxt.eof())
	cout << "Error, end of file" << endl;
      for(int j=0; j<5; ++j)
	{
	  intxt >> map_pad_temp[i][j];
	  intxt.ignore();
	}
      intxt >> map_pad_temp[i][5];
    }

  int map_pad[10240][6]={0};
  for(int i=0; i<10240; ++i) //10240
    {
      int pad_ID = map_pad_temp[i][4];
      for(int j=0; j<6; ++j)
	map_pad[pad_ID][j] = map_pad_temp[i][j];
    }
    

  // // test map_pad
  // for(int i=0; i<10; ++i)
  //   {
  //     for(int j=0; j<6; ++j)
  // 	cout << map_pad[i][j] << "\t";
  //     cout << endl;
  //   }
  

  //cout << h2p->GetBinContent(100) << endl;
  // cout << h2p->GetXaxis()->GetNbins() << endl;
  // cout << h2p->GetYaxis()->GetNbins() << endl;

  // for(int i=0; i<100; ++i)
  //   for(int j=0; j<100; ++j)
  //     {
  // 	int N = h2p->GetBinContent(i,j);
  // 	if(N>0)
  // 	  cout << i << "\t"
  // 	       << j << "\t"
  // 	       << h2p->GetBinContent(i,j) << endl;
  //     }

  
  TH1D *h1[10];
  h1[0] = new TH1D("CoBo ID", "CoBo ID", 12,0, 12);
  h1[1] = new TH1D("AsAd ID", "AsAd ID", 5, 0, 5);
  h1[2] = new TH1D("AGET ID", "AGET ID", 5, 0, 5);
  h1[3] = new TH1D("AGET ch", "AGET ch", 65, 0, 65);
  h1[4] = new TH1D("Pad ID", "Pad ID", 110, 0, 11000);

  h1[5] = new TH1D("CoBo ID ", "CoBo ID", 11,0, 11);
  h1[6] = new TH1D("AsAd ID ", "AsAd ID", 5, 0, 5);
  h1[7] = new TH1D("AGET ID ", "AGET ID", 5, 0, 5);
  h1[8] = new TH1D("AGET ch ", "AGET ch", 65, 0, 65);
  h1[9] = new TH1D("Pad ID ", "Pad ID", 110, 0, 11000);

  
  //noise pads
  ofstream outtxt1;
  outtxt1.open("./noise_pads.txt");
  
  int Nmax = h2p->GetMaximum();
  cout << "max = " << Nmax << endl;
  int Nnoise = 0;
  for(int i=0; i<Nbin; ++i)
    {
      int N = h2p->GetBinContent(i+1);
      if(N > Nmax*0.9)
  	{
  	  //cout << i << "\t" << N << endl;
  	  outtxt1 << "\t" << i << "\t";
	  for(int j=0; j<5; ++j)
	    {
	      outtxt1 << map_pad[i][j] << "\t";
	      h1[j]->Fill(map_pad[i][j]);

	    }
	  outtxt1 << endl;
  	  ++Nnoise;
  	}
    }
  outtxt1.close();
  cout << endl << "noise pads num: " << Nnoise << endl;


  // missing pads  
  ofstream outtxt2;
  outtxt2.open("./missing_pads.txt");
  int Nmissing = 0;
  for(int i=0; i<Nbin; ++i)
    {
      int N = h2p->GetBinContent(i+1);
      if(N == 0)
  	{
  	  //cout << i << "\t" << N << endl;
  	  outtxt2 << "\t" << i << "\t";
	  for(int j=0; j<5; ++j)
	    {
	      outtxt2 << map_pad[i][j] << "\t";
	      h1[j+5]->Fill(map_pad[i][j]);
	    }
	  outtxt2 << endl;
  	  ++Nmissing;
  	}
    }
  outtxt2.close();
  cout << endl << "missing pads num: " << Nmissing << endl;


  // TCanvas *c1 = new TCanvas("c1","c1", 1900, 1000);
  // c1->Divide(5,2);
  // for(int j=0; j<5; ++j)
  //   {
  //     c1->cd(j+1);
  //     h1[j]->Draw();
  //     c1->cd(j+6);
  //     h1[j+5]->Draw();
  //   }
  // c1->Update();
  // c1->Print("check_pads.pdf");

  
  TCanvas *c2 = new TCanvas("noise","noise");
  c2->Divide(2,2);
  TCanvas *c3 = new TCanvas("missing","missing");
  c3->Divide(2,2);
  for(int j=0; j<4; ++j)
    {
      c2->cd(j+1);
      h1[j]->Draw();
      c3->cd(j+1);
      h1[j+5]->Draw();
    }

  gStyle->SetTitleSize(0.07, "t");
  gStyle->SetLabelSize(0.07, "XY");
  gStyle->SetFrameLineWidth(2);
  for(int j=0; j<10; ++j)
    h1[j]->SetLineWidth(2);

  
  c2->Update();
  c3->Update();

  c2->Print("noise.pdf");
  //c3->Print("missing.pdf");
  
  

  
  //auto ele = h2p->GetPadRef(9908);


  
  return 0; 
}

