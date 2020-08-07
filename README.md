[![MIT License](https://img.shields.io/apm/l/atomic-design-ui.svg?)](https://github.com/tterb/atomic-design-ui/blob/master/LICENSEs)

# ADALMGodot

* Supported Godot version: *3.2*

Godot game engine provides a suitable and easy interface for visualization and Software Defined Radio (SDR) is an application where visalization can be crucial. 

This module adds the functionality of connecting to ADALM Pluto SDR through GDScript. It uses Libiio to connect to a ADALMPluto device. FFTW3 is also supported. 

# Dependencies

## libiio
libiio is a library to communicate with standard iio devices. The instructions below is from <a href="https://wiki.analog.com/resources/tools-software/linux-software/libiio">`What is libiio? [Analog Devices Wiki]`</a>


on Debian/Ubuntu (Testes on Ubuntu 18.04):
```shell
~$ sudo apt-get install libxml2 libxml2-dev bison flex libcdk5-dev cmake libaio-dev libusb-1.0-0-dev libserialport-dev libxml2-dev libavahi-client-dev doxygen graphviz
```

and then install libini:
```shell
~$git clone https://github.com/pcercuei/libini.git
~$cd libini
~/libini$ mkdir build && cd build && cmake ../ && make && sudo make install
```

To install libiio, follow:
```shell
~$ git clone https://github.com/analogdevicesinc/libiio.git
~$ cd libiio
~/libiio$ cmake ./
~/libiio$ make all
~/libiio$ sudo make install
```

# Compiling for Godot

This SCsub file written for this module compiles it as a shared library. You need source code of Godot. 

then, for Ubuntu, run:
```shell
~/GodotSource$ scons -j8 custom_modules=~/Path/to/this/repository/ platform=x11
```
after it's done, make sure the 'bin' folder in Godot's source directory is in your LD_LIBRARY_PATH environment variable. 

# Usage

After Godot is started with this module loaded, you can simply do the following to run get samples from the SDR.

```
func _ready():
	var adalm_sdr  = ADALMPluto.new()
	adalm_sdr.setup()
	var data: Array = adalm_sdr.receive()
```
