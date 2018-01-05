#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <api/SeaBreezeWrapper.h>
int main() {
    int errorCode;
    seabreeze_open_spectrometer(0,&errorCode);
    char model[16];
    seabreeze_get_model(0,&errorCode,model, sizeof(model));
    int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode);
    double *spectrum = (double*) malloc(pixels * sizeof(double));
    double *wavelength = (double*) malloc(pixels * sizeof(double));
    seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels);
    spectrum[0]=0;
    seabreeze_set_integration_time_microsec(0, &errorCode, 10000);
    seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels);

    FILE *csv;
    csv = fopen("spectrum.csv","w");


    for(int i = 0; i<pixels;){
      for(int k = 0; k < 5 && k+i<pixels; k++){
        fprintf(csv,"%f,%f\n",wavelength[k+i],spectrum[k+i]);
      }
      i+=5;

    }
    fclose(csv);
    return 0;
}
