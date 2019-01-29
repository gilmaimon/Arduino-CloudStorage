#pragma once

namespace cloud_storage_utils {

    template <class Ty> struct ResultWrapper {
        const bool isOk;
        const Ty value;

        ResultWrapper(bool isOk, Ty value) : isOk(isOk), value(value) {}
        ResultWrapper(bool isOk) : isOk(isOk), value() {}

        operator Ty() {
            return value;
        }
    };

    
    template <class Ty> struct PopResultWrapper {
        const bool isOk;
        const Ty value;
        const bool hasNext;

        PopResultWrapper(bool isOk, Ty value, bool hasNext) : isOk(isOk), value(value), hasNext(hasNext)  {}
        PopResultWrapper(bool isOk) : PopResultWrapper(isOk, {}, false) {}

        operator Ty() {
            return value;
        }
    };
}