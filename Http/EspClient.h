#pragma once

#include "Client.h"
#include "Esp8266RequestImpl.h"

namespace http {
    typedef Request<Esp8266RequestImpl> Esp8266Request;
};