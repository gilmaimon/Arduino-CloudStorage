#pragma once

namespace cloud_storage_utils {

    template <class Ty> struct ResultWrapper {
        const bool isOk;
        const Ty value;

        ResultWrapper(bool isOk, Ty value) : isOk(isOk), value(value) {}

        operator Ty() {
            return value;
        }
    };
}