#include "FFT_Functions.hh"
#include <iostream>

// This is an example main function that would use the FFT_Functions header file

// These functions perform the Fourier transformations from different starting points
// The starting arrays/histograms are generated in these functions
void FFT_fff(); // Creates a modulated cos TF1 function and transforms it
void FFT_ffh_modcos(); // Creates a modulated cos Histogram and transforms it (this is equivalent to the function above)
void FFT_ffh_gauss(); // Creates a gauss-like TH1D histogram and transforms it

// This is the main function, uncomment or comment the function that you want to run
void FFT_main(){
  //FFT_fff();
  //FFT_ffh_gauss();
  FFT_ffh_modcos();
}

void FFT_fff(){
  // Test a function as an input for fourier transform:
  int n = 1000;
  TF1 *fmodcos = new TF1("fmodcos", "(10*cos(2*pi*5*x) + 5*cos(2*pi*40*x)) * e^(-pi*(x)^2)", -2*TMath::Pi(), 2*TMath::Pi());

  // Define two arrays to store the data in
  double real[n];
  double imag[n];
  
  // Perform the FFT with function defined in header
  fourier_from_function(fmodcos, real, imag);

  // Now perform a back transform
  fourier_back_from_array(real, imag, n);
}

void FFT_ffh_modcos(){
  // Take the modulated cosine function from above and create a histogram from it
  int n = 1000;
  double xmin = -2*TMath::Pi();
  double xmax = 2*TMath::Pi();
  TF1 *fun = new TF1("fmodcos", "(10*cos(2*pi*5*x) + 5*cos(2*pi*40*x)) * e^(-pi*(x)^2)",xmin ,xmax );

  TH1D *histo = new TH1D("histo", "Histogram from function", n, xmin, xmax);

  // Fill the histogram bins with the function y-values
  double x;
  for (int i=0; i<n; i++){
    // Define x value to evaluate the function at
    x = (double) i * (xmax - xmin)/(n-1) + xmin ;

    // Fill histogram with value at current x
    histo->SetBinContent(i+1, fun->Eval(x));
  }

  // Create arrays to store the transformed data in
  double real[n];
  double imag[n];

  // Perform the transform through the function defined in the header
  fourier_from_histo(histo, real, imag);

  // print magnitude values
  for (int i=0; i<n; i++){
    std::cout << "Bin: " << i << " Magnitude: " << TMath::Sqrt( real[i]*real[i] + imag[i]*imag[i]) << std::endl;
  }

  // Now transform back
  fourier_back_from_array(real, imag, n);
}

void FFT_ffh_gauss(){
  // Test a histogram as an input for fourier transform:
  int n = 10000;

  TH1::SetDefaultSumw2();
  gROOT->SetStyle("Plain");
  
  TH1D *histo = new TH1D("histo", "A Gaussian type histogram", n, -100, 100);

  TRandom3 *R = new TRandom3();
  R->SetSeed(0);
  for(int i=0; i<n; i++){
    histo->Fill(R->Gaus(0, 5)); 
  }

  // Create arrays to save the data in
  double real[n];
  double imag[n];

  fourier_from_histo(histo, real, imag);
  
}
