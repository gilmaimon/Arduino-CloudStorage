#pragma once

#include "Client.h"
#include "GenericEspRequestImpl.h"

namespace http {
    typedef Request<GenericEspRequestImpl> GenericEspRequest;
};