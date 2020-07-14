/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "adalm_pluto.h"
#include "fftw_d.h"

void register_ADALMPluto_types() {
    ClassDB::register_class<ADALMPluto>();  
    ClassDB::register_class<FFTHandler>();

}

void unregister_ADALMPluto_types() {
   // Nothing to do here in this example.
}