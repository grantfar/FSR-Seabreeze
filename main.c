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
    seabreeze_set_integration_time_microsec(0, &errorCode, 100000);
    seabreeze_get_formatted_spectrum(0,&errorCode,spectrum,pixels);
    printf("%f\n",*spectrum+1);
    return 0;
}
