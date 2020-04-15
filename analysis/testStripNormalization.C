TTree *VTP;

void plot(const char *name, const char *hname, int bcm_thr, int laser_thr_min, int laser_thr_max)
{
  int nbins = 600, start = 0, stop = 600;
  VTP->Draw(
      Form("%s/(scaldat[14]*0.00033)>>%s(%d,%d,%d)", name, hname, nbins, start, stop),
      Form("scaldat[14]>%d && scaldat[12]>%d && scaldat[12]<%d", bcm_thr, laser_thr_min, laser_thr_max)
    );
  TH1F *h = (TH1F*)(gROOT->FindObject(Form("%s",hname)));
  Double_t h_int = h->Integral("");
  Double_t h_tsumwx = 0;

  for(int i=0;i<nbins;i++)
    h_tsumwx+=h->GetBinContent(i+1)*(i+(stop-start)/(2.0*nbins));
  printf("h_int=%f, h_tsumwx=%f, h_tsumwx/h_int=%f, h_mean=%f\n", h_int, h_tsumwx, h_tsumwx/h_int, h->GetMean());
}

void testStripNormalization()
{
  TFile *f0 = new TFile("../Rootfiles/eDet_1073.root");
  if(!(f0->IsOpen())) return;
  VTP = (TTree*) f0->Get("VTP");

  TCanvas *pC = new TCanvas("c1","c1",1000,800);
  pC->Divide(2,2,0.05,0.05);

  pC->cd(1); pC->cd(1)->SetLogy(1); plot("eplaneD_scalcnt[59]", "h1", 3000,100,1000);
  pC->cd(2); pC->cd(2)->SetLogy(1); plot("eplaneD_scalcnt[59]", "h2", 3000,0,50);
  pC->cd(3); pC->cd(3)->SetLogy(1); plot("eplaneA_scalcnt[59]", "h3", 3000,100,1000);
  pC->cd(4); pC->cd(4)->SetLogy(1); plot("eplaneA_scalcnt[59]", "h4", 3000,0,50);
}
