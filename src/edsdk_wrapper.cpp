#include "edsdk_wrapper.hpp"

#include <EDSDK.h>
#include <EDSDKErrors.h>
#include <EDSDKTypes.h>

#include <cassert>
#include <iostream>

namespace edsdk_w {
    EDSDK& EDSDK::get_instance() {
        static EDSDK instance{};
        return instance;
    }

    EDSDK::EDSDK() {
        assert(EdsInitializeSDK() == EDS_ERR_OK && "EDSDK initialization error");
        std::cout << "SDK Initialized" << std::endl; //TODO: remove console debug
    }

    EDSDK::~EDSDK() {
        assert(EdsTerminateSDK() == EDS_ERR_OK && "EDSDK termination error");
        std::cout << "SDK terminated" << std::endl; //TODO: remove console debug
    }

} //namespace edsdk_w
