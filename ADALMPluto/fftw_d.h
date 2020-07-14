#ifndef __FFTW_D
#define __FFTW_D

#include <fftw3.h>
#include <iostream>
#include "core/reference.h"


class FFTHandler : public Reference {
    GDCLASS(FFTHandler, Reference);


protected:
    static void _bind_methods();
        
private:
    int N;
    fftw_plan    fftw_p;
    fftw_complex *input_samples, *output_samples;
    void convert_Array_to_fftw_complex(Array input);


public:
    void set_window_size(int value);
    Array perform_fft(Array Input);
    void setup();
    int get_window_size();
    FFTHandler();
    ~FFTHandler();
};


#endif