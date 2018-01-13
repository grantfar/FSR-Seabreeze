#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "api/SeaBreezeWrapper.h"
int main() {
	char fileName[] = { "spectrum.csv" };
	
	double maxSpec = 0;
	double avgSpec = 0;
	double totalSpec = 0;
	double maxWave = 0;
	int errorCode;
	int open = seabreeze_open_spectrometer(0,&errorCode); //Opens device attached to system if there is one
	if (open == 1) { //If no device opened, program exits
		printf("--NO SPECTROMETER FOUND--\n");
		return 0;
	}
	char model[16];
	seabreeze_get_model(0,&errorCode,model, sizeof(model)); //Returns string denoting type of device
	int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode); //Acquires a spectrum and returns answer in formatted floats
	double *spectrum = (double*) malloc(pixels * sizeof(double));
	double *wavelength = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels); //Computes wavelengths for the spectrometer and fills "wavelength" array with those values up to "pixels" value
	spectrum[0]=0;
	seabreeze_set_integration_time_microsec(0, &errorCode, 10000); //Sets integration time to 10000 microseconds
	seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels); //Acquires the spectrum and returns in formatted floats and places this in the "spectrum" array up to "pixels" value

	FILE *csv = fopen(fileName,"w");
	fprintf(csv, "Wavelength, Spectrum\n");
	for(int i = 0; i < pixels; i += 5){
		for(int k = 0; k < 5 && k+i<pixels; k++){
			fprintf(csv,"%f,%f\n",wavelength[k+i],spectrum[k+i]); //Writes wavelength and spectrum at each pixel to "spectrum.csv"
			totalSpec += spectrum[k+i];
			if (maxSpec < spectrum[k+i] & k + i > 1) {
				maxSpec = spectrum[k+i];
				maxWave = wavelength[k+i];
			}
		}
	}

	printf("Output written to \"%s\"\n", fileName);
	avgSpec = totalSpec/pixels;
	printf("MAX: %f AVG: %f\nWAVELENGTH AT MAX: %f nm\n", maxSpec, avgSpec, maxWave);

	seabreeze_close_spectrometer(0, &errorCode);
	fclose(csv); //Closes spectrum file

	return 0;
}
