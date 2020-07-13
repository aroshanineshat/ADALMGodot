/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "adalm_pluto.h"

void register_ADALMPluto_types() {
    ClassDB::register_class<ADALMPluto>();
}

void unregister_ADALMPluto_types() {
   // Nothing to do here in this example.
}
