//
// Created by nikolas on 8/4/22.
//

#ifndef COMP371_PROJECT_AUDIOMANAGER_H
#define COMP371_PROJECT_AUDIOMANAGER_H

#include <complex>
#include <cstring>
#include "fftw3.h"
#include "Config.h"
#include "irrKlang.h"

class AudioManager {

    static inline fftw_complex *in, *out;
    static inline fftw_plan plan;
    static inline int N = 4096;
    static inline int sample_rate = 0;

public:

    static void setBufferSize(int size){
        N = size;
    }
    // initialize all data buffers
    static void init(int sr)
    {
        sample_rate = sr;
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

        plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        if(plan == NULL){
            std::cerr << "Bad plan \n";
            exit(-1);
        }
    }
    static void setInputData(const double* data){
        for(int i =0 ; i < N ; i++){
            in[i][0] = data[i];
        }
    }
    static void executeFFT(double* freqs,double* mags){
        fftw_execute(plan);
        for(int i = 0 ; i < N/2 ; i++){

            double real = out[i][0];
            double imag = out[i][1];

            double mag = sqrt(real * real + imag * imag);
            double freq= i * sample_rate / N;

            freqs[i] = freq;
            mags[i] = mag;
        }
    }

};


#endif //COMP371_PROJECT_AUDIOMANAGER_H
