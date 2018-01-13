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
	int open = seabreeze_open_spectrometer(0,&errorCode); //Opens device attached to system
	if (open == 0) {
		for (;;) {
			getColor();
			sleep(1);
		}
		seabreeze_close_spectrometer(0, &errorCode);
	}
	else
		printf("--NO SPECTROMETER FOUND--\n");
	return 0;
}

int getColor() {
	double maxSpec = 0;
	double maxWave = 0;
	char model[16];
	seabreeze_get_model(0,&errorCode,model, sizeof(model)); //Returns string denoting type of device
	int pixels = seabreeze_get_formatted_spectrum_length(0, &errorCode); //Acquires a spectrum and returns answer in formatted floats
	double *spectrum = (double*) malloc(pixels * sizeof(double));
	double *wavelength = (double*) malloc(pixels * sizeof(double));
	seabreeze_get_wavelengths(0,&errorCode,wavelength,pixels); //Computes wavelengths for the spectrometer and fills "wavelength" array with those values up to "pixels" value
	spectrum[0]=0;
	seabreeze_set_integration_time_microsec(0, &errorCode, 10000); //Sets integration time to 10000 microseconds
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
	for (int i = 0; i < 8; i++) {
		if (maxWave < waveStart[i] && maxWave >= waveStart[i + 1] && maxSpec > 5000 && LED == 0) {
			printf("%s\n", colors[i]);
			LED = 1;
		}
	}
	if (LED == 0)
		printf("NO LED\n");
}
