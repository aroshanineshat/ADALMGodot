/*adalm_pluto.cpp*/

#include "adalm_pluto.h"

void ADALMPluto::_bind_methods(){
    ClassDB::bind_method(D_METHOD("set_buffer_size", "value"), &ADALMPluto::set_buffer_size);
    //ClassDB::bind_method(D_METHOD("set_ip", "value"), &ADALMPluto::set_ip);
    ClassDB::bind_method(D_METHOD("set_Freq", "value"), &ADALMPluto::set_Freq);
    ClassDB::bind_method(D_METHOD("setup"), &ADALMPluto::setup);
    ClassDB::bind_method(D_METHOD("receive"), &ADALMPluto::receive);
}

void ADALMPluto::set_buffer_size(int value){
    this->buffer_size   = value;
    this->log("Buffer size updated ...");
}

void ADALMPluto::set_ip (const String& value){
    this->ip_address = value;
    this->log ("IP updated ...");
}

void ADALMPluto::set_Freq(long int value){
    this->Freq = value;
    this->log ("Frequency set ...");
}

void ADALMPluto::setup(){
    std::wstring ws = this->ip_address.c_str();
    std::string s( ws.begin(), ws.end() );
    this->ctx = iio_create_context_from_uri(s.c_str());
    this->log("Context created...");
    this->phy = iio_context_find_device(ctx, "ad9361-phy");
    this->log("Phy created...");

    iio_channel_attr_write_longlong(
		iio_device_find_channel(this->phy, "altvoltage0", true),
		"frequency",
		this->Freq); /* RX LO frequency 1.09GHz */
    
    iio_channel_attr_write_longlong(
		iio_device_find_channel(phy, "voltage0", false),
		"sampling_frequency",
		1140000); /* RX baseband rate 5 MSPS */

    dev = iio_context_find_device(ctx, "cf-ad9361-lpc");
 
	rx0_i = iio_device_find_channel(dev, "voltage0", 0);
	rx0_q = iio_device_find_channel(dev, "voltage1", 0);
 
	iio_channel_enable(rx0_i);
	iio_channel_enable(rx0_q);
 
	rxbuf = iio_device_create_buffer(dev, this->buffer_size, false);


}

Array ADALMPluto::receive(){
    	void *p_dat, *p_end, *t_dat;
		ptrdiff_t p_inc;
 
		iio_buffer_refill(rxbuf);
 
		p_inc = iio_buffer_step(rxbuf);
		p_end = iio_buffer_end(rxbuf);

        Array data;
 
		for (p_dat = iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc, t_dat += p_inc) {
			const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
			const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)

			data.push_back( i );
            data.push_back( q );
 
		}		
    return data;
}

ADALMPluto::ADALMPluto(){
    this->Freq          = 1090000000; //setting the default frequency to 1.09 GHz
    this->ip_address    = "ip:192.168.2.1"; //setting the default ip address
    this->buffer_size   = 4096;  //set the default buffer size to 4096
}

ADALMPluto::~ADALMPluto(){
    iio_buffer_destroy(this->rxbuf);
    iio_context_destroy(this->ctx);
}

void ADALMPluto::log(std::string message){
    std::cout << message << std::endl;
}