void Plot(){
	TFile *f = new TFile("striprate.root");
	int nstrip[43];
	for(int ii=0;ii<12;ii++) nstrip[ii]=ii;
	for(int ii=16;ii<28;ii++) nstrip[ii-4]=ii;
	for(int ii=36;ii<44;ii++) nstrip[ii-12]=ii;
	for(int ii=49;ii<60;ii++) nstrip[ii-17]=ii;

    Double_t totalQ_on=0, totalQ_off=0;

	TTree *TStrip = (TTree*) f->Get("TStrip");
	TStrip->SetBranchAddress("totalQ_on",&totalQ_on);
	TStrip->SetBranchAddress("totalQ_off",&totalQ_off);

	TStrip->GetEntry(0);
	Double_t nscale = 1.0*totalQ_off/(1.0*totalQ_on);
	cout<<nscale<<endl;

	TH2F *hD_laseron = (TH2F*)f->Get("hD_laseron");
	TH2F *hD_laseroff = (TH2F*)f->Get("hD_laseroff");

	TH1D *hDstrip_on[43];
	TH1D *hDstrip_off[43];

   for(int ii=0; ii<43; ii++){
	   hDstrip_on[ii] = hD_laseron->ProjectionY(Form("hDstrip_on_%d",nstrip[ii]),nstrip[ii],nstrip[ii]+1); 	
	   hDstrip_off[ii] = hD_laseroff->ProjectionY(Form("hDstrip_off_%d",nstrip[ii]),nstrip[ii],nstrip[ii]+1); 	
	}

    TCanvas *c1=new TCanvas("c1","c1",1500,1500);
	c1->Divide(2,4);
	for(int ii=0;ii<8;ii++){
	   c1->cd(ii+1);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,400);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();


	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}

    TCanvas *c2=new TCanvas("c2","c2",1500,1500);
	c2->Divide(2,4);
	for(int ii=8;ii<16;ii++){
	   c2->cd(ii-7);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,400);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();

	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}

    TCanvas *c3=new TCanvas("c3","c3",1500,1500);
	c3->Divide(2,4);
	for(int ii=16;ii<24;ii++){
	   c3->cd(ii-15);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,400);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();

	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}

    TCanvas *c4=new TCanvas("c4","c4",1500,1500);
	c4->Divide(2,4);
	for(int ii=24;ii<32;ii++){
	   c4->cd(ii-23);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,300);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();

	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}

    TCanvas *c5=new TCanvas("c5","c5",1500,1500);
	c5->Divide(2,4);
	for(int ii=32;ii<40;ii++){
	   c5->cd(ii-31);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,300);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();

	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}

    TCanvas *c6=new TCanvas("c6","c6",1500,1500);
	c6->Divide(2,2);
	for(int ii=40;ii<43;ii++){
	   c6->cd(ii-39);
	   double n1 = hDstrip_on[ii]->GetEntries();
	   double n2 = hDstrip_off[ii]->GetEntries();
	   hDstrip_off[ii]->Draw();
	   hDstrip_off[ii]->GetXaxis()->SetRangeUser(0,400);
	   hDstrip_off[ii]->SetTitle(Form("plane D chan %d; cnts(8.33ms);",nstrip[ii]));

	   hDstrip_on[ii]->Draw("same");
	   hDstrip_on[ii]->Scale(nscale);
	   hDstrip_on[ii]->SetLineColor(2); 
	   hDstrip_on[ii]->SetLineWidth(2);
	   hDstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hDstrip_on[ii],"laser on","L");
	   leg->AddEntry(hDstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();

	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.02);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1*nscale));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
	}


   c1->Print("D_striprate.pdf[");
   c1->Print("D_striprate.pdf");
   c2->Print("D_striprate.pdf");
   c3->Print("D_striprate.pdf");
   c4->Print("D_striprate.pdf");
   c5->Print("D_striprate.pdf");
   c6->Print("D_striprate.pdf");
   c6->Print("D_striprate.pdf]");

}
