#include "edsdk_wrapper.hpp"

#include <EDSDK.h>
#include <EDSDKErrors.h>
#include <EDSDKTypes.h>

#include <cassert>
#include <iostream>

namespace edsdk_w {
    namespace utils {
        std::string explain_prop_value_image_quality(std::uint32_t value) {
            auto image_type = [](std::uint8_t v) {
                switch (v) {
                    case 0x0: return "Unknown";
                    case 0x1: return "Jpeg";
                    case 0x2: return "CRW";
                    case 0x4: return "RAW";
                    case 0x6: return "CR2";
                    case 0x8: return "HEIF";
                    default: return "Unknown";
                }
            };

            auto image_size = [](std::uint8_t v) {
                switch (v) {
                    case 0: return "L";
                    case 1: return "M";
                    case 2: return "S";
                    case 5: return "M1";
                    case 6: return "M2";
                    case 14: return "S1";
                    case 15: return "S2";
                    case 16: return "S3";
                    default: return "Unknown";
                }
            };

            auto image_compress = [](std::uint8_t v) {
                switch (v) {
                    case 2: return "Normal";
                    case 3: return "Fine";
                    case 4: return "Lossless";
                    case 5: return "Superfine";
                    default: return "Unknown";
                }
            };

            std::string res, space = " ";
            res += image_size((value >> 24) & 7) + space +
                   image_type((value >> 20) & 7) + space +
                   image_compress((value >> 16) & 7) + std::string(" + ") +
                   image_size((value >> 8) & 7) + space +
                   image_type((value >> 4) & 7) + space +
                   image_compress(value & 7);

            return res;
        }

        std::string explain_prop_value_white_balance(std::uint32_t value) {
            switch (value) {
                case 0: return "Auto: Ambience priority";
                case 1: return "Daylight";
                case 2: return "Cloudy";
                case 3: return "Tungsten";
                case 4: return "Fluorescent";
                case 5: return "Flash";
                case 6: return "Manual";
                case 8: return "Shade";
                case 9: return "Color temperature";
                case 10: return "Custom WB: PC-1";
                case 11: return "Custom WB: PC-2";
                case 12: return "Custom WB: PC-3";
                case 15: return "Manual - 2";
                case 16: return "Manual - 3";
                case 18: return "Manual - 4";
                case 19: return "Manual - 5";
                case 20: return "Custom WB: PC-4";
                case 21: return "Custom WB: PC-5";
                case 23: return "Auto: White priority";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_color_temperature(std::uint32_t value) {
            return std::to_string(value) + "K";
        }

        std::string explain_prop_value_color_space(std::uint32_t value) {
            switch (value) {
                case 1: return "sRGB";
                case 2: return "Adobe RGB";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_ae_mode(std::uint32_t value) {
            switch (value) {
                case 0x00: return "Program AE";
                case 0x01: return "Shutter-Speed Priority AE";
                case 0x02: return "Aperture Priority AE";
                case 0x03: return "Manual Exposure";
                case 0x04: return "Bulb";
                case 0x05: return "Auto Depth-of-Field AE";
                case 0x06: return "Depth-of-Field AE";
                case 0x07: return "Camera settings registered";
                case 0x08: return "Lock";
                case 0x09: return "Auto";
                case 0x0a: return "Night Scene Portrait";
                case 0x0b: return "Sports";
                case 0x0c: return "Portrait";
                case 0x0d: return "Landscape";
                case 0x0e: return "Close-Up";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_drive_mode(std::uint32_t value) {
            switch (value) {
                case 0x00: return "Single shooting";
                case 0x01: return "Continuous Shooting";
                case 0x02: return "Video";
                case 0x04: return "High speed continuous";
                case 0x05: return "Low speed continuous";
                case 0x06: return "Single Silent Shooting";
                case 0x07: return "Self-timer:Continuous";
                case 0x10: return "Self-timer:10 sec";
                case 0x11: return "Self-timer:2 sec";
                case 0x12: return "14fps super high speed";
                case 0x13: return "Silent single shooting";
                case 0x14: return "Silent continuous shooting";
                case 0x15: return "Silent HS continuous";
                case 0x16: return "Silent LS continuous";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_iso_speed(std::uint32_t value) {
            switch (value) {
                case 0x00: return "ISO Auto";
                case 0x28: return "ISO 6";
                case 0x30: return "ISO 12";
                case 0x38: return "ISO 25";
                case 0x40: return "ISO 50";
                case 0x48: return "ISO 100";
                case 0x4b: return "ISO 125";
                case 0x4d: return "ISO 160";
                case 0x50: return "ISO 200";
                case 0x53: return "ISO 250";
                case 0x55: return "ISO 320";
                case 0x58: return "ISO 400";
                case 0x5b: return "ISO 500";
                case 0x5d: return "ISO 640";
                case 0x60: return "ISO 800";
                case 0x63: return "ISO 1000";
                case 0x65: return "ISO 1250";
                case 0x68: return "ISO 1600";
                case 0x6b: return "ISO 2000";
                case 0x6d: return "ISO 2500";
                case 0x70: return "ISO 3200";
                case 0x73: return "ISO 4000";
                case 0x75: return "ISO 5000";
                case 0x78: return "ISO 6400";
                case 0x7b: return "ISO 8000";
                case 0x7d: return "ISO 10000";
                case 0x80: return "ISO 12800";
                case 0x83: return "ISO 16000";
                case 0x85: return "ISO 20000";
                case 0x88: return "ISO 25600";
                case 0x8b: return "ISO 32000";
                case 0x8d: return "ISO 40000";
                case 0x90: return "ISO 51200";
                case 0x93: return "ISO 64000";
                case 0x95: return "ISO 80000";
                case 0x98: return "ISO 102400";
                case 0xa0: return "ISO 204800";
                case 0xa8: return "ISO 409600";
                case 0xb0: return "ISO 819200";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_metering_mode(std::uint32_t value) {
            switch (value) {
                case 1: return "Spot metering";
                case 3: return "Evaluate metering";
                case 4: return "Partial metering";
                case 5: return "Center-weighted averaging metering";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_af_mode(std::uint32_t value) {
            switch (value) {
                case 0: return "One-Shot AF";
                case 1: return "AI Servo AF";
                case 2: return "AI Focus AF";
                case 3: return "Manual Focus";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_av(std::uint32_t value) {
            switch (value) {
                case 0x08: return "1";
                case 0x0b: return "1.1";
                case 0x0c: return "1.2";
                case 0x0d: return "1.2 (1/3)";
                case 0x10: return "1.4";
                case 0x13: return "1.6";
                case 0x14: return "1.8";
                case 0x15: return "1.8 (1/3)";
                case 0x18: return "2";
                case 0x1b: return "2.2";
                case 0x1c: return "2.5";
                case 0x1d: return "2.5 (1/3)";
                case 0x20: return "2.8";
                case 0x23: return "3.2";
                case 0x85: return "3.4";
                case 0x24: return "3.5";
                case 0x25: return "3.5 (1/3)";
                case 0x28: return "4";
                case 0x2b:
                case 0x2c: return "4.5";
                case 0x2d: return "5.0";
                case 0x30: return "5.6";
                case 0x33: return "6.3";
                case 0x34: return "6.7";
                case 0x35: return "7.1";
                case 0x38: return "8";
                case 0x3b: return "9";
                case 0x3c: return "9.5";
                case 0x3d: return "10";
                case 0x40: return "11";
                case 0x43: return "13 (1/3)";
                case 0x44: return "13";
                case 0x45: return "14";
                case 0x48: return "16";
                case 0x4b: return "18";
                case 0x4c: return "19";
                case 0x4d: return "20";
                case 0x50: return "22";
                case 0x53: return "25";
                case 0x54: return "27";
                case 0x55: return "29";
                case 0x58: return "32";
                case 0x5b: return "36";
                case 0x5c: return "38";
                case 0x5d: return "40";
                case 0x60: return "45";
                case 0x63: return "51";
                case 0x64: return "54";
                case 0x65: return "57";
                case 0x68: return "64";
                case 0x6b: return "72";
                case 0x6c: return "76";
                case 0x6d: return "80";
                case 0x70: return "91";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_tv(std::uint32_t value) {
            switch (value) {
                case 0x0c: return "Bulb";
                case 0x10: return "30\"";
                case 0x13: return "25\"";
                case 0x14: return "20\"";
                case 0x15: return "20\" (1/3)";
                case 0x18: return "15\"";
                case 0x1b: return "13\"";
                case 0x1c: return "10\"";
                case 0x1d: return "10\" (1/3)";
                case 0x20: return "8\"";
                case 0x23: return "6\" (1/3)";
                case 0x24: return "6\"";
                case 0x25: return "5\"";
                case 0x28: return "4\"";
                case 0x2b: return "3\"2";
                case 0x2c: return "3\"";
                case 0x2d: return "2\"5";
                case 0x30: return "2";
                case 0x33: return "1\"6";
                case 0x34: return "1\"5";
                case 0x35: return "1\"3";
                case 0x38: return "1";
                case 0x3b: return "0\"8";
                case 0x3c: return "0\"7";
                case 0x3d: return "0\"6";
                case 0x40: return "0\"5";
                case 0x43: return "0\"4";
                case 0x44: return "0\"3";
                case 0x45: return "0\"3 (1/3)";
                case 0x48: return "1/4";
                case 0x4b: return "1/5";
                case 0x4c: return "1/6";
                case 0x4d: return "1/6 (1/3)";
                case 0x50: return "1/8";
                case 0x53: return "1/10 (1/3)";
                case 0x54: return "1/10";
                case 0x55: return "1/13";
                case 0x58: return "1/15";
                case 0x5b: return "1/20 (1/3)";
                case 0x5c: return "1/20";
                case 0x5d: return "1/25";
                case 0x60: return "1/30";
                case 0x63: return "1/40";
                case 0x64: return "1/45";
                case 0x65: return "1/50";
                case 0x68: return "1/60";
                case 0x6b: return "1/80";
                case 0x6c: return "1/90";
                case 0x6d: return "1/100";
                case 0x70: return "1/125";
                case 0x73: return "1/160";
                case 0x74: return "1/180";
                case 0x75: return "1/200";
                case 0x78: return "1/250";
                case 0x7b: return "1/320";
                case 0x7c: return "1/350";
                case 0x7d: return "1/400";
                case 0x80: return "1/500";
                case 0x83: return "1/640";
                case 0x84: return "1/750";
                case 0x85: return "1/800";
                case 0x88: return "1/1000";
                case 0x8b: return "1/1250";
                case 0x8c: return "1/1500";
                case 0x8d: return "1/1600";
                case 0x90: return "1/2000";
                case 0x93: return "1/2500";
                case 0x94: return "1/3000";
                case 0x95: return "1/3200";
                case 0x98: return "1/4000";
                case 0x9b: return "1/5000";
                case 0x9c: return "1/6000";
                case 0x9d: return "1/6400";
                case 0xa0: return "1/8000";
                case 0xa3: return "1/10000";
                case 0xa5: return "1/12800";
                case 0xa8: return "1/16000";
                default: return "unknown";
            }
        }

        std::string explain_prop_value_exposure_compensation(std::uint32_t value) {
            switch (value) {
                case 0x28: return "+5";
                case 0x25: return "+4 2/3";
                case 0x24: return "+4 1/2";
                case 0x23: return "+4 1/3";
                case 0x20: return "+4";
                case 0x1d: return "+3 2/3";
                case 0x1c: return "+3 1/2";
                case 0x1b: return "+3 1/3";
                case 0x18: return "+3";
                case 0x15: return "+2 2/3";
                case 0x14: return "+2 1/2";
                case 0x13: return "+2 1/3";
                case 0x10: return "+2";
                case 0x0d: return "+1 2/3";
                case 0x0c: return "+1 1/2";
                case 0x0b: return "+1 1/3";
                case 0x08: return "+1";
                case 0x05: return "+2/3";
                case 0x04: return "+1/2";
                case 0x03: return "+1/3";
                case 0x00: return "0";
                case 0xfd: return "-1/3";
                case 0xfc: return "-1/2";
                case 0xfb: return "-2/3";
                case 0xf8: return "-1";
                case 0xf5: return "-1 1/3";
                case 0xf4: return "-1 1/2";
                case 0xf3: return "-1 2/3";
                case 0xf0: return "-2";
                case 0xed: return "-2 1/3";
                case 0xec: return "-2 1/2";
                case 0xeb: return "-2 2/3";
                case 0xe8: return "-3";
                case 0xe5: return "-3 1/3";
                case 0xe4: return "-3 1/2";
                case 0xe3: return "-3 2/3";
                case 0xe0: return "-4";
                case 0xdd: return "-4 1/3";
                case 0xdc: return "-4 1/2";
                case 0xdb: return "-4 2/3";
                case 0xd8: return "-5";
                default: return "unknown";
            }
        }
    }//namespace edsdk_w::utils

    EDSDK& EDSDK::get_instance() {
        static EDSDK instance{};
        return instance;
    }

    EDSDK::EDSDK() : _camera{nullptr} {
        assert(EdsInitializeSDK() == EDS_ERR_OK && "EDSDK initialization error");
        std::cout << "SDK Initialized" << std::endl; //TODO: remove console debug
    }

    EDSDK::~EDSDK() {
        assert(EdsTerminateSDK() == EDS_ERR_OK && "EDSDK termination error");
        std::cout << "SDK terminated" << std::endl; //TODO: remove console debug
    }

    std::vector<std::string> EDSDK::get_available_camera_list() {
        std::vector<std::string> res;
        EdsError err = EDS_ERR_OK;
        EdsCameraListRef cameraList = nullptr;
        EdsUInt32 count = 0;

        err = EdsGetCameraList(&cameraList);

        if (err == EDS_ERR_OK) {
            err = EdsGetChildCount(cameraList, &count);
            if (count == 0) {
                err = EDS_ERR_DEVICE_NOT_FOUND;
                return {};
            }
        }

        for (std::uint8_t i = 0; i < count; i++) {
            EdsCameraRef camera = nullptr;
            EdsDeviceInfo devInfo;
            err = EdsGetChildAtIndex(cameraList, i, &camera);
            if (err == EDS_ERR_OK) {
                err = EdsGetDeviceInfo(camera, &devInfo);
                if (err == EDS_ERR_OK) {
                    res.emplace_back(devInfo.szDeviceDescription);
                }
            }
            if (camera) {
                EdsRelease(camera);
            }
        }

        if (cameraList) {
            EdsRelease(cameraList);
        }

        return res;
    }

    bool EDSDK::set_camera(std::uint8_t index_in_list) {
        EdsError err = EDS_ERR_OK;
        EdsCameraListRef cameraList = nullptr;
        EdsCameraRef camera_ref = nullptr;

        err = EdsGetCameraList(&cameraList);

        if (err == EDS_ERR_OK) {
            err = EdsGetChildAtIndex(cameraList, index_in_list, &camera_ref);
            if (err == EDS_ERR_OK) {
                _camera = new Camera(camera_ref);
            }
        }

        if (cameraList) {
            EdsRelease(cameraList);
        }

        return err == EDS_ERR_OK;
    }

    std::optional<std::reference_wrapper<EDSDK::Camera>> EDSDK::get_camera() {
        if (_camera) {
            return *_camera;
        } else {
            return std::nullopt;
        }
    }

    std::string EDSDK::explain_prop_value(std::uint32_t prop_id, std::uint32_t value) {
        switch (prop_id) {
            case kEdsPropID_ImageQuality:
                return utils::explain_prop_value_image_quality(value);
            case kEdsPropID_WhiteBalance:
                return utils::explain_prop_value_white_balance(value);
            case kEdsPropID_ColorTemperature:
                return utils::explain_prop_value_color_temperature(value);
            case kEdsPropID_ColorSpace:
                return utils::explain_prop_value_color_space(value);
            case kEdsPropID_AEMode:
                return utils::explain_prop_value_ae_mode(value);
            case kEdsPropID_DriveMode:
                return utils::explain_prop_value_drive_mode(value);
            case kEdsPropID_ISOSpeed:
                return utils::explain_prop_value_iso_speed(value);
            case kEdsPropID_MeteringMode:
                return utils::explain_prop_value_metering_mode(value);
            case kEdsPropID_AFMode:
                return utils::explain_prop_value_af_mode(value);
            case kEdsPropID_Av:
                return utils::explain_prop_value_av(value);
            case kEdsPropID_Tv:
                return utils::explain_prop_value_tv(value);
            case kEdsPropID_ExposureCompensation:
                return utils::explain_prop_value_exposure_compensation(value);
            default:
                return "unknown property";
        }

    }

    EDSDK::Camera::Camera(EdsCameraRef camera) : _camera_ref{camera} {}

    EDSDK::Camera::~Camera()  {
        if (_camera_ref) {
            EdsRelease(_camera_ref);
        }
    }

} //namespace edsdk_w

