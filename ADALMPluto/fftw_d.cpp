/**
 * @file fftw_d.cpp
 *
 * @brief The C++ file for computing fftw of sdr
 *
 * @author Arash Roshanineshat
 *
 */

#include "fftw_d.h"


void FFTHandler::setup(){
    std::cout << "Starting FFT ..." << std::endl;
    std::cout << "FFT size: " << this->N << std::endl;
    input_samples  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->N);
    output_samples = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->N);
    this->fftw_p = fftw_plan_dft_1d(this->N, this->input_samples, this->output_samples, FFTW_FORWARD, FFTW_ESTIMATE);
    std::cout << "FFT Started ..." << std::endl;
}


Array FFTHandler::perform_fft(Array input){
    //std::cout << "Performing FFT ..." << std::endl;
    this->convert_Array_to_fftw_complex(input);
    fftw_execute(this->fftw_p);
    Array output;
    for (int i=0; i< this->N; i++) {
        output.push_back(this->output_samples[i][0]);
        output.push_back(this->output_samples[i][1]);
    }
    return output;
}

void FFTHandler::convert_Array_to_fftw_complex(Array input){

    int index = 0;
    for (int i=0; i< 2*this->N; i++){
        int value = static_cast<float>(input.get(i));
        if (i % 2 == 0)
        {
            this->input_samples[index][0] = value;
            //std::cout << "Added real at " << index << " value: " << value <<std::endl;
        }
        else
        {
            this->input_samples[index][1] = value;
            //std::cout << "Added imag at " << index << " value: " << value <<std::endl;
            index++;
        }
    }
    //std::cout << "Converting done ..." << std::endl;
}

FFTHandler::FFTHandler(){
   
}

FFTHandler::~FFTHandler(){
    fftw_destroy_plan(this->fftw_p);  
    fftw_free(this->input_samples); 
    fftw_free(this->output_samples);
}

void FFTHandler::set_window_size(int value){
    this->N = value;
}

int FFTHandler::get_window_size(){
    return this->N;
}

void FFTHandler::_bind_methods() {
    ClassDB::bind_method(D_METHOD("perform_fft", "value"), &FFTHandler::perform_fft);
    ClassDB::bind_method(D_METHOD("set_window_size", "value"), &FFTHandler::set_window_size);
    ClassDB::bind_method(D_METHOD("get_window_size"), &FFTHandler::get_window_size);
    ClassDB::bind_method(D_METHOD("setup"), &FFTHandler::setup);
}