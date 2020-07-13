/* adalm_pluto.h*/

#ifndef __ADALM_PLUTO_H_
#define __ADALM_PLUTO_H_

#include "core/reference.h"
#include <iio.h>
#include <iostream>
#include <string>


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

    String ip_address;

    void log(std::string);

protected:
    static void _bind_methods();

public:

    void set_Freq(long int);
    void set_ip(const String&);
    void set_buffer_size(int);

    void setup();

    Array receive();

    ADALMPluto();
    ~ADALMPluto();

};

#endif