/* adalm_pluto.h*/

#ifndef __ADALM_PLUTO_H_
#define __ADALM_PLUTO_H_

#include "core/reference.h"
#include <iio.h>
#include <iostream>
#include <string>



// TODO: Select custom URI
// TODO: Get sensory Information 



class ADALMPluto : public Reference {
    GDCLASS (ADALMPluto, Reference);

private:
    long int Freq;
    int buffer_size;
    struct iio_context *ctx;
	struct iio_device *phy;

    struct iio_device *dev;
	struct iio_channel *rx0_i, *rx0_q;
	struct iio_buffer *rxbuf;


    /* Scan of current context
    */
    struct iio_scan_context *scan_ctx;
    struct iio_context_info **info;  //each available device will have structure of iio_context info
    Array list_of_available_devices; //the result of the scan context will be written to this variable. 
    int numberOfDevicesFound;
    struct iio_scan_context * create_scan_context();

    String selected_URI;

    String ip_address;




    void log(std::string message);

protected:
    static void _bind_methods();

public:

    void set_Freq(long int);
    void set_ip(const String&);
    void set_buffer_size(int);
    int scan_for_available_devices();
    String get_URI_byIndex(int Index);
    Array get_available_iio_devices();
    

    void setup(String URI);

    Array receive();

    ADALMPluto();
    ~ADALMPluto();

};

#endif
