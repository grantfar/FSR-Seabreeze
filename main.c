/*
 * This program is the main program we will use at FSR. It takes spectrum readings from the spectrometer and saves them to a file with their corresponding wavelengths.
 * After these readings are taken, it determines the average spectrum reading and max spectrum reading with its corresponding wavelength.
 */

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
	int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode); //Acquires a spectrum and returns answer in formatted floats

	double *wavelength = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels); //Computes wavelengths for the spectrometer and fills "wavelength" array with those values up to "pixels" value

	double *spectrum = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels); //Acquires the spectrum and returns in formatted floats and places this in the "spectrum" array up to "pixels" value

	FILE *csv = fopen(fileName,"w");

	fprintf(csv, "Wavelength, Spectrum\n");
	for(int i = 0; i < pixels; i += 5){
		for(int k = 0; k < 5 && k+i<pixels; k++){
			fprintf(csv,"%f,%f\n",wavelength[k+i],spectrum[k+i]); //Writes wavelength and spectrum at each pixel to "spectrum.csv"
			totalSpec += spectrum[k+i];
			if (maxSpec < spectrum[k+i] & k + i > 1) { //Sets maxSpec if spectrum reading is higher than other specrum readings, skips first two readings because of bad values
				maxSpec = spectrum[k+i];
				maxWave = wavelength[k+i];
			}
		}
	}

	printf("Output written to \"%s\"\n", fileName);

	avgSpec = totalSpec/pixels;
	printf("MAX: %f AVG: %f\nWAVELENGTH AT MAX: %f nm\n", maxSpec, avgSpec, maxWave);

	seabreeze_close_spectrometer(0, &errorCode); //Closes device
	fclose(csv); //Closes spectrum file

	return 0;
}
