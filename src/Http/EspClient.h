#pragma once

#include "Client.h"

#ifdef ARDUINO_ARCH_ESP32

#include "GenericEspRequestImpl.h"
namespace http {
    typedef Request<GenericEspRequestImpl> GenericEspRequest;
};

#else

#include "Esp8266RequestImpl.h"
namespace http {
    typedef Request<Esp8266RequestImpl> GenericEspRequest;
};

#endif