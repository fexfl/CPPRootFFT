void FFT_example(){
  // Create a canvas to plot everything on
  TCanvas *canvas = new TCanvas("canvas", "FFT example", 0, 0, 600, 600);
  canvas->Divide(2,3);

  // Define a function we want to transform
  double xmin = 0;
  double xmax = 1;
  TF1 *f = new TF1("f", "0.9 + 0.9*cos(2.*TMath::Pi()*x)+0.7*cos(3.*2.*TMath::Pi()*x)+0.9*cos(4.*2.*TMath::Pi()*x)+1.0*cos(8.*2.*TMath::Pi()*x)+0.3*cos(10.*2.*TMath::Pi()*x)+0.1*cos(11.*2.*TMath::Pi()*x)+0.1*cos(12.*2.*TMath::Pi()*x)+0.2*cos(13.*2.*TMath::Pi()*x)+0.1*cos(14.*2.*TMath::Pi()*x)+0.1*cos(15.*2.*TMath::Pi()*x)+0.2*cos(16.*2.*TMath::Pi()*x)", xmin, xmax);

  // Convert function to a discrete histogram
  int nbins = 300;
  TH1D *h = new TH1D("h", "Function Histogram", nbins, xmin, xmax);
  // Set bin values to function values
  double x;
  for (int i=0; i<nbins; i++){
    x = (double) i * (xmax - xmin)/(nbins-1) + xmin;
    h->SetBinContent(i+1, f->Eval(x));
  }

  // Draw both the function and the histogram
  canvas->cd(1);
  f->Draw();
  h->Draw("SAME");

  // Now compute the transform
  TH1 *h_t = 0;
  h_t = h->FFT(h_t, "MAG");
  h_t->SetTitle("Fourier Transformed Histogram");
  h_t->Scale(1./TMath::Sqrt(double(nbins)));

  canvas->cd(2);
  h_t->Draw();

  // Save transformed data in an array
  TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();
  double re[nbins];
  double im[nbins];

  fft->GetPointsComplex(re, im);

  // Or get single points from the frequency domain, like the DC component or the Nyquist harmonic
  double re_dc, im_dc, re_ny, im_ny;
  fft->GetPointComplex(0, re_dc, im_dc); // DC-component (Frequency 0)
  fft->GetPointComplex(nbins/2 + 1, re_ny, im_ny); // Nyquist harmonic

  // Now perform a backwards transform
  // Two options: either from array or from histogram

  // Option 1: from arrays (usually less errors than with option 2)
  TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &nbins, "C2R M K");
  fft_back->SetPointsComplex(re, im);
  fft_back->Transform();
  // Create histogram to plot output
  TH1 *h_b = 0;
  h_b = TH1::TransformHisto(fft_back, h_b, "Re");
  h_b->SetTitle("Inverse Fourier Transform with array");
  canvas->cd(3);
  h_b->Draw();

  // Option 2: from histogram
  TH1 *h_b_h = 0;
  h_b_h = h_t->FFT(h_b_h, "Re");
  h_b_h->SetTitle("Inverse Fourier Transform with histogram");
  canvas->cd(4);
  h_b_h->Draw();

  // We can also modify our signal by filtering certain frequencies from the transform before transforming back:
  int minHarmonic = 4;
  int maxHarmonic = 20;
  double re_filter = 0;
  double im_filter = 0;
  TVirtualFFT *fft_back_filtered = TVirtualFFT::FFT(1, &nbins, "C2R M K");
  fft_back_filtered->SetPointsComplex(re, im);

  for (int j = minHarmonic ; j <= maxHarmonic ; j++){
    fft_back_filtered->SetPoint(j, re_filter, im_filter);
  }
  fft_back_filtered->Transform();
  TH1 *hbf = 0;
  hbf = TH1::TransformHisto(fft_back_filtered, hbf, "Re");
  hbf->SetTitle("Backwards Transform with filtered frequencies");
  canvas->cd(5);
  hbf->Draw();

}
