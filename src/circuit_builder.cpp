#include "circuit_builder.h"

const char* COMPONENT_TYPE_STRING(COMPONENT_TYPE type) {
    switch(type) {
        case CT_NONE: break;
        case CT_CLOCK: return "Clock"; break;
        case CT_PIN: return "Pin"; break;
        case CT_AND: return "AND"; break;
        case CT_OR: return "OR"; break;
        case CT_ADDER: return "Adder"; break;
    }
}