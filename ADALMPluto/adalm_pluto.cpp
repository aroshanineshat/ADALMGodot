/*adalm_pluto.cpp*/

#include "adalm_pluto.h"

void ADALMPluto::_bind_methods(){
    ClassDB::bind_method(D_METHOD("set_buffer_size", "value"), &ADALMPluto::set_buffer_size);
    //ClassDB::bind_method(D_METHOD("set_ip", "value"), &ADALMPluto::set_ip);
    ClassDB::bind_method(D_METHOD("set_Freq", "value"), &ADALMPluto::set_Freq);
    ClassDB::bind_method(D_METHOD("setup"), &ADALMPluto::setup);
    ClassDB::bind_method(D_METHOD("receive"), &ADALMPluto::receive);
    ClassDB::bind_method(D_METHOD("get_available_iio_devices"), &ADALMPluto::get_available_iio_devices);
    ClassDB::bind_method(D_METHOD("get_URI_by_Index","index"), &ADALMPluto::get_URI_byIndex);
    ClassDB::bind_method(D_METHOD("scan_available_iio_devices","index"), &ADALMPluto::scan_for_available_devices);
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

struct iio_scan_context * ADALMPluto::create_scan_context(){
    /*
    *   NULL context searches for all available backends,
    *   flag is set to 0 based on the documentation.
    */
    return iio_create_scan_context(NULL, 0);
}


/**
 *  Scans for available iio devices and updates **info  variable
 */
int ADALMPluto::scan_for_available_devices(){

    if (!this->scan_ctx){ //checking to seee if the scan context is already create. If not create it. 
        this->log("Attempting to create scan context.");
        this->scan_ctx = create_scan_context();
    }
    if (!this->scan_ctx){ //checking to see if the scan context is created. It must be created by now.
        this->log("Couldn't create the scan context.");
        return -1; //Error
    }
    this->numberOfDevicesFound = iio_scan_context_get_info_list (this->scan_ctx, &info);
    return this->numberOfDevicesFound;

}

Array  ADALMPluto::get_available_iio_devices(){
    // numberOfDevices will contain the number of available iio devices
    this->scan_for_available_devices();

    if (this->numberOfDevicesFound < 0){ //numberOfDevicesFound is returned by scan_for_available_devices function.
        return Array();
    }

    for (int i=0; i< this->numberOfDevicesFound; i++){
        String item;
        item = iio_context_info_get_description (info[i]) ; 
        item = item + " URI: " ;
        item = item + iio_context_info_get_uri(info[i]);
        list_of_available_devices.append(item);
    }

    return list_of_available_devices;

}

String ADALMPluto::get_URI_byIndex(int Index){
    if (Index <0 || Index >= this->numberOfDevicesFound){
        return String("");
    }
    String URI = iio_context_info_get_uri(info[Index]);
    return URI;
}

void ADALMPluto::setup(String URI){
    std::wstring ws = URI.c_str();
    std::string s( ws.begin(), ws.end() );
    this->ctx = iio_create_context_from_uri(s.c_str());
    this->selected_URI = URI;
    this->log("Context created...");
    this->phy = iio_context_find_device(ctx, "ad9361-phy");
    this->log("Phy created...");

    iio_channel_attr_write_longlong(
		iio_device_find_channel(this->phy, "altvoltage0", true),
		"frequency",
		this->Freq);
    
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
    this->Freq                  = 1090000000; //setting the default frequency to 1.09 GHz
    this->ip_address            = "ip:192.168.2.1"; //setting the default ip address
    this->buffer_size           = 4096;  //set the default buffer size to 4096
    this->numberOfDevicesFound  = 0; //initial value is 0.
}

ADALMPluto::~ADALMPluto(){
    iio_buffer_destroy(this->rxbuf);
    iio_context_destroy(this->ctx);
    iio_context_info_list_free(this->info);

}

void ADALMPluto::log(std::string message){
    std::cout << message << std::endl;
}