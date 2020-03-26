void Plot(){
	TFile *f = new TFile("striprate.root");
	int nstrip[43];
	for(int ii=0;ii<12;ii++) nstrip[ii]=ii;
	for(int ii=16;ii<28;ii++) nstrip[ii-4]=ii;
	for(int ii=36;ii<44;ii++) nstrip[ii-12]=ii;
	for(int ii=49;ii<60;ii++) nstrip[ii-17]=ii;

     Double_t Asym_D_on[128]={0.0};
     Double_t Asym_D_off[128]={0.0};


	TTree *TStrip = (TTree*) f->Get("TStrip");
	TStrip->SetBranchAddress("asym_D_on",Asym_D_on);
	TStrip->SetBranchAddress("asym_D_off",Asym_D_off);

	TH2F *hD_laseron = (TH2F*)f->Get("hD_laseron");
	TH2F *hD_laseroff = (TH2F*)f->Get("hD_laseroff");

	TH1D *hBstrip_on[43];
	TH1D *hBstrip_off[43];
/*
   for(int ii=0; ii<43; ii++){
	   hBstrip_on[ii] = hD_laseron->ProjectionY(Form("hBstrip_on_%d",nstrip[ii]),nstrip[ii],nstrip[ii]+1); 	
	   hBstrip_off[ii] = hD_laseroff->ProjectionY(Form("hBstrip_off_%d",nstrip[ii]),nstrip[ii],nstrip[ii]+1); 	
	}

    TCanvas *c1=new TCanvas("c1","c1",1500,1500);
	c1->Divide(2,4);
	for(int ii=0;ii<8;ii++){
	   c1->cd(ii+1);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,20);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
*/
/*
	   TLatex latex;
	   latex.SetNDC();
	   latex.SetTextSize(0.05);
	   //latex.SetTextAlign(12);
	   latex.DrawLatex(0.6,0.5,Form("N(laser on)=%.0lf",n1));
	   latex.DrawLatex(0.6,0.45,Form("N(laser off)=%.0lf",n2));
*///	}
/*
    TCanvas *c2=new TCanvas("c2","c2",1500,1500);
	c2->Divide(2,4);
	for(int ii=8;ii<16;ii++){
	   c2->cd(ii-7);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,20);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
	}

    TCanvas *c3=new TCanvas("c3","c3",1500,1500);
	c3->Divide(2,4);
	for(int ii=16;ii<24;ii++){
	   c3->cd(ii-15);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,30);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
	}

    TCanvas *c4=new TCanvas("c4","c4",1500,1500);
	c4->Divide(2,4);
	for(int ii=24;ii<32;ii++){
	   c4->cd(ii-23);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,20);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
	}

    TCanvas *c5=new TCanvas("c5","c5",1500,1500);
	c5->Divide(2,4);
	for(int ii=32;ii<40;ii++){
	   c5->cd(ii-31);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,20);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
	}

    TCanvas *c6=new TCanvas("c6","c6",1500,1500);
	c6->Divide(2,2);
	for(int ii=40;ii<43;ii++){
	   c6->cd(ii-39);
	   double n1 = hBstrip_on[ii]->GetEntries();
	   double n2 = hBstrip_off[ii]->GetEntries();
	   hBstrip_off[ii]->Draw();
	   hBstrip_off[ii]->GetXaxis()->SetRangeUser(0,20);
	   hBstrip_off[ii]->SetTitle(Form("plane B chan %d; cnts(8.33ms);",nstrip[ii]));

	   hBstrip_on[ii]->Draw("same");
	   hBstrip_on[ii]->Scale(n2/n1);
	   hBstrip_on[ii]->SetLineColor(2); 
	   hBstrip_on[ii]->SetLineWidth(2);
	   hBstrip_off[ii]->SetLineWidth(2);
	   
	   auto leg = new TLegend(0.6,0.6,0.75,0.75);
	   leg->AddEntry(hBstrip_on[ii],"laser on","L");
	   leg->AddEntry(hBstrip_off[ii],"laser off","L");
	   leg->SetMargin(0.5);
	   leg->Draw();
	}
*/


	TGraph *gasym_on = new TGraph(128);
	TGraph *gasym_off = new TGraph(128);

    TStrip->GetEntry(0);
	for(int ii=0;ii<128;ii++){
		gasym_on->SetPoint(ii,ii,Asym_D_on[ii]);
		gasym_off->SetPoint(ii,ii,Asym_D_off[ii]);
	}

    TCanvas *c7 = new TCanvas("c7","c7",1500,1500);
	TMultiGraph *mg =new TMultiGraph();
	gasym_on->SetMarkerColor(2);	
	gasym_on->SetMarkerSize(1.3);	
	gasym_on->SetMarkerStyle(8);	
	gasym_off->SetMarkerColor(4);	
	gasym_off->SetMarkerSize(1.3);	
	gasym_off->SetMarkerStyle(8);	
	mg->Add(gasym_on);
	mg->Add(gasym_off);
	mg->Draw("AP");

	auto leg1 = new TLegend(0.6,0.6,0.75,0.75);
	leg1->AddEntry(gasym_on,"laser on","P");
	leg1->AddEntry(gasym_off,"laser off","P");
	leg1->SetMargin(0.5);
	leg1->Draw();



/*   c1->Print("D_striprate.pdf[");
   c1->Print("D_striprate.pdf");
   c2->Print("D_striprate.pdf");
   c3->Print("D_striprate.pdf");
   c4->Print("D_striprate.pdf");
   c5->Print("D_striprate.pdf");
   c6->Print("D_striprate.pdf");
*/   c7->Print("D_striprate.pdf");
//   c7->Print("D_striprate.pdf]");

}
