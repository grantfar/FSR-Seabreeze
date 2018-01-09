#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "api/SeaBreezeWrapper.h"
int main() {
    int errorCode;
    seabreeze_open_spectrometer(0,&errorCode); //Opens device attached to system
    char model[16];
    seabreeze_get_model(0,&errorCode,model, sizeof(model)); //Returns string denoting type of device
    int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode); //Acquires a spectrum and returns answer in formatted floats
    double *spectrum = (double*) malloc(pixels * sizeof(double));
    double *wavelength = (double*) malloc(pixels * sizeof(double));
    seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels); //Computes wavelengths for the spectrometer and fills "wavelength" array with those values up to "pixels" value
    spectrum[0]=0;
    seabreeze_set_integration_time_microsec(0, &errorCode, 10000); //Sets integration time to 10000 microseconds
    seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels); //Acquires the spectrum and returns the answer in formatted floats and places this in the "spectrum" array up to "pixels" value

    FILE *csv = fopen("spectrum.csv","w");

    for(int i = 0; i<pixels;){
      for(int k = 0; k < 5 && k+i<pixels; k++){
        fprintf(csv,"%f,%f\n",wavelength[k+i],spectrum[k+i]); //Writes wavelength and spectrum at each pixel to "spectrum.csv"
      }
      i+=5;

    }
    fclose(csv);
	printf("Output written to \"spectrum.csv\"\n");
    return 0;
}
