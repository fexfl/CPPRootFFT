#include <iostream>

// This header includes several functions that perform a Fast Fourier Transfrom from an array, a histogram, or a function, as well as the reverse transformations

// Performing a FFT from a TF1 Function as the starting point:
void fourier_from_function( TF1 *fun , double* real, double* imag){
  double xmin, xmax;
  // Set xmin and xmax to the Range values defined in the function
  fun->GetRange(xmin, xmax);

  // Create a canvas for plotting everything
  TCanvas *canvas = new TCanvas("canvas", "Canvas Title", 0, 0, 600, 600);
  canvas->Divide(1,2);

  // Number of bins for histogram
  int n = 1000;
  // Create histogram from function to transform later
  TH1D *histo = new TH1D("histo", "Histogram from function", n, xmin, xmax);

  double x;
  for (int i=0; i<n; i++){
    // Define x value to evaluate the function at
    x = (double) i * (xmax - xmin)/(n-1) + xmin ;

    // Fill histogram with value at current x
    histo->SetBinContent(i+1, fun->Eval(x));
  }

  // Draw this histogram
  canvas->cd(1);
  histo->Draw();
  // Now define a histogram to save the transformed data in
  TH1 *histo_f = 0;

  // Do the fourier transform
  // For the options, "Re" returns the real part of the transform. For other options, see TVirtualFFT class reference (try "MAG").
  histo_f = histo->FFT(histo_f, "Re");
  histo_f->SetTitle("Fourier transfomed histogram");
  // Data is now saved in histo_f
  // x-Axis is now the bin number and not the frequency, divide by function range to obtain frequency
  // y-Axis has to be rescaled with a factor of 1/Sqrt(n)

  // Plot the Histogram:
  canvas->cd(2);
  histo_f->Draw();
  
  // Want to save data in arrays?
  // Get the FFT Object that was just created:
  TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();

  // Save data in arrays
  fft->GetPointsComplex(real, imag);
}

// Performing a FFT from a histogram with data as the starting point
void fourier_from_histo( TH1D *histo , double* real, double* imag){
  // First create canvas for plotting
  TCanvas *canvas_histo = new TCanvas("canvas_histo", "Canvas Title", 0, 0, 600, 600);
  canvas_histo->Divide(1,2);

  // Plot the original histogram
  canvas_histo->cd(1);
  histo->Draw();
  // Define a histogram to save the transformed data in
  TH1 *histo_f = 0;
  // Do the fourier transform
  histo_f = histo->FFT(histo_f, "MAG");
  // Data is now saved in histo_f
  // x-Axis is now the bin number and not the frequency, divide by function range to obtain frequency
  // y-Axis has to be rescaled with a factor of 1/Sqrt(n)

  // Draw the Histogram:
  canvas_histo->cd(2);
  histo_f->Draw();

  // Want to save data in arrays?
  // Get the FFT Object that was just created:
  TVirtualFFT *fft = TVirtualFFT::GetCurrentTransform();

  // Save data in arrays
  fft->GetPointsComplex(real, imag);
}

// Performing a FFT from an array of real data points as a starting point
void fourier_from_array(double* datapoints, int& n){
  std::cout << "Datapoints: " << n << std::endl;

  // Create a Fourier transform object
  TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &n, "R2C ES K");
  // Fill the object with datapoints
  fft_own->SetPoints(datapoints);
  // Execute the FFT
  fft_own->Transform();

  // Plot the output in a histogram:
  TH1 *histo_f = 0;
  histo_f = TH1::TransformHisto(fft_own, histo_f, "MAG");
  histo_f->SetTitle("Fourier tranformed data");

  TCanvas *canvas_array = new TCanvas("canvas_array", "Canvas Title", 0, 0, 600, 600);
  histo_f->Draw();

  // Want to save the output data in arrays again?
  // Overwrite the datapoints array:
  fft_own->GetPoints(datapoints);
}

void fourier_back_from_array(double* fourierreal, double* fourierimag, int& n){
  std::cout << "Datapoints: " << n << std::endl;
  
  // Create a TVirtualFFT object for the back transform
  TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &n, "C2R M K");
  fft_back->SetPointsComplex(fourierreal, fourierimag);
  fft_back->Transform();

  // Create canvas and Histogram for plotting
  TCanvas *canvas_back = new TCanvas("canvas_back", "Reverse Fourier Transform", 0, 0, 600, 600);
  TH1 *histo_back = 0;
  histo_back = TH1::TransformHisto(fft_back, histo_back, "Re");
  histo_back->Draw();
}

void fourier_back_from_histo(TH1* histo_transformed){
  // Make sure that histo_transformed was transformed with the option "RE" and not "MAG", or else the back transformation will not be correct
  // Define histogram to save the back transformation in
  TH1 *histo_back = 0;
  histo_back = histo_transformed->FFT(histo_back, "Re");

  TCanvas *canvas_back_histo = new TCanvas("canvas_back_histo", "Canvas Title", 0, 0, 600, 600);
  histo_back->SetTitle("Histogram after reverse fourier transform");
  histo_back->Draw();
}
