#include "edsdk_wrapper.hpp"

#include <cassert>
#include <iostream>

namespace edsdk_w {
    struct Init {
        Init() {
            assert(EdsInitializeSDK() == EDS_ERR_OK && "EDSDK initialization error");
            std::cout << "SDK Initialized" << std::endl; //TODO: remove console debug
        }

        ~Init() {
            assert(EdsTerminateSDK() == EDS_ERR_OK && "EDSDK termination error");
            std::cout << "SDK terminated" << std::endl; //TODO: remove console debug
        }
    } _;

} //namespace edsdk_w