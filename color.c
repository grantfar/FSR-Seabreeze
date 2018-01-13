/* 
 * This program takes input from the spectrometer, then based on its spectrum readings,
 * it determines the color of an LED it is pointed at.
 * As is, the program just runs until you kill the window it's in. Not important to make it pretty,
 * but it'd be hella cool if we did.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "api/SeaBreezeWrapper.h"
#include <unistd.h>

int getColor();

char *colors[] ={
	"BLACK",
	"RED",
	"ORANGE",
	"YELLOW",
	"GREEN",
	"BLUE",
	"WHITE"
};
double waveStart[11] ={
	99999, 780, 622, 597, 577, 492, 455, 390, 0
};

int errorCode;

int main() {
	int open = seabreeze_open_spectrometer(0,&errorCode); //Opens device attached to system if there is one
	if (open == 0) {
		for (;;) {
			getColor();
			sleep(1); //Waits one second before getting color again
		}
		seabreeze_close_spectrometer(0, &errorCode); //CLoses device
	}
	else //Exits program if no device is opened
		printf("--NO SPECTROMETER FOUND--\n");
	return 0;
}

int getColor() {

	double maxSpec = 0;
	double maxWave = 0;
	char model[16];

	int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode); //Acquires a spectrum and returns answer in formatted floats

	double *wavelength = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels); //Computes wavelengths for the spectrometer and fills "wavelength" array with those values up to "pixels" value

	double *spectrum = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels); //Acquires the spectrum and returns in formatted floats and places this in the "spectrum" array up to "pixels" value

	for(int i = 0; i<pixels; i += 5){
		for(int k = 0; k < 5 && k+i<pixels; k++){
			if (maxSpec < spectrum[k+i] & k + i > 1) {
				maxSpec = spectrum[k+i];
				maxWave = wavelength[k+i];
			}
		}
	}
	int LED = 0;
	for (int i = 0; i < 8; i++) { //Compares wavelength of max spectrum reading to determine color based on colors array
		if (maxWave < waveStart[i] && maxWave >= waveStart[i + 1] && maxSpec > 5000 && LED == 0) {
			printf("%s\n", colors[i]);
			LED = 1;
		}
	}
	if (LED == 0) //If light source is not bright enough
		printf("NO LED\n");
}
