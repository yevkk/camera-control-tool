#include "edsdk_wrapper.hpp"

#include <EDSDK.h>
#include <EDSDKErrors.h>
#include <EDSDKTypes.h>

#include <cassert>
#include <iostream>

namespace edsdk_w {
    namespace utils {
        template<typename T>
        Queue<T>::Queue(Queue<T>&& other)  noexcept {
            _queue = std::move(other._queue);
        }

        template<typename T>
        bool Queue<T>::empty() {
            std::scoped_lock _{_mutex};
            return _queue.empty();
        }

        template<typename T>
        std::uint32_t Queue<T>::size() {
            std::scoped_lock _{_mutex};
            return _queue.size();
        }

        template<typename T>
        std::optional<T> Queue<T>::pop() {
            std::scoped_lock _{_mutex};
            if (_queue.empty()) {
                return {};
            }
            T tmp = _queue.front();
            _queue.pop();
            return tmp;
        }

        template<typename T>
        void Queue<T>::push(const T &item) {
            std::scoped_lock _{_mutex};
            _queue.push(item);
        }


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

    EDSDK::EDSDK() : _camera{nullptr}, _stop_thread{false}, _event_loop_thread{&EDSDK::_event_loop, this} {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    }

    EDSDK::~EDSDK() {
        reset_camera();
        _stop_thread = true;
        _event_loop_thread.join();
        CoUninitialize();
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

    bool EDSDK::reset_camera() {
        delete _camera;
        _camera = nullptr;

        return true;
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

    std::vector<std::string> EDSDK::explain_prop_value(std::uint32_t prop_id, const std::vector<std::uint32_t> &value) {
        std::vector<std::string> res{};

        for (const auto &element : value) {
            res.push_back(EDSDK::explain_prop_value(prop_id, element));
        }

        return res;
    }

    void EDSDK::_event_loop() {
        assert(EdsInitializeSDK() == EDS_ERR_OK && "EDSDK initialization error");
        std::cout << "SDK Initialized" << std::endl; //TODO: remove console debug

        while(!_stop_thread) {
            std::this_thread::sleep_for(std::chrono::milliseconds(112));
            EdsGetEvent();
        }

        assert(EdsTerminateSDK() == EDS_ERR_OK && "EDSDK termination error");
        std::cout << "SDK terminated" << std::endl; //TODO: remove console debug
    }

    class EDSDK::Camera::Command {
    public:
        virtual bool dispatch(EDSDK::Camera*) = 0;

        virtual ~Command() = default;
    };

    class EDSDK::Camera::CommandSetProperty : public Command {
    public:
        CommandSetProperty(EdsPropertyID prop_id, EdsUInt32 value) : Command{}, _prop_id{prop_id}, _value{value} {}

        bool dispatch(EDSDK::Camera* camera) override {
            std::uint32_t *prop_ptr;
            switch (_prop_id) {
                case kEdsPropID_WhiteBalance:
                    prop_ptr = &camera->_properties.white_balance;
                    break;
                case kEdsPropID_ColorTemperature:
                    prop_ptr = &camera->_properties.color_temperature;
                    break;
                case kEdsPropID_ColorSpace:
                    prop_ptr = &camera->_properties.color_space;
                    break;
                case kEdsPropID_DriveMode:
                    prop_ptr = &camera->_properties.drive_mode;
                    break;
                case kEdsPropID_MeteringMode:
                    prop_ptr = &camera->_properties.metering_mode;
                    break;
                case kEdsPropID_ISOSpeed:
                    prop_ptr = &camera->_properties.iso;
                    break;
                case kEdsPropID_Av:
                    prop_ptr = &camera->_properties.av;
                    break;
                case kEdsPropID_Tv:
                    prop_ptr = &camera->_properties.tv;
                    break;
                case kEdsPropID_ExposureCompensation:
                    prop_ptr = &camera->_properties.exposure_compensation;
                    break;
                default:
                    return false;
            }

            return _set_property(camera->_camera_ref,
                                 prop_ptr,
                                 camera->_properties.mutex);
        }
    private:
        bool _set_property(EdsCameraRef camera,
                           std::uint32_t *prop_ptr,
                           std::mutex &props_mutex) {
            if (_value == UINT32_MAX) return false;

            EdsError err;
            EdsDataType dataType;
            EdsUInt32 dataSize;

            err = EdsGetPropertySize(camera, _prop_id, 0, &dataType, &dataSize);
            if (err == EDS_ERR_OK) {
                err = EdsSetPropertyData(camera, _prop_id, 0, dataSize, &_value);
                if (err == EDS_ERR_OK) {
                    std::scoped_lock _{props_mutex};
                    *prop_ptr = _value;
                }
            }

            return err == EDS_ERR_OK;
        }

        EdsPropertyID _prop_id;
        EdsUInt32 _value;
    };

    class EDSDK::Camera::CommandSetState : public Command {
    public:
        explicit CommandSetState(EdsCameraStatusCommand command) : Command{}, _command{command} {}

        bool dispatch(EDSDK::Camera* camera) override {
            return EdsSendStatusCommand(camera->_camera_ref, _command, 0) == EDS_ERR_OK;
        }
    private:
        EdsCameraStatusCommand _command;
    };

    class EDSDK::Camera::CommandShutterControl : public Command {
    public:
        enum class Action {RELEASE, PRESS_FULL, PRESS_HALFWAY, PRESS_AND_RELEASE};

        explicit CommandShutterControl(Action action) : Command{}, _action{action} {}

        bool dispatch(EDSDK::Camera* camera) override {
            switch (_action) {
                case Action::RELEASE:
                    return _shutter_control(camera->_camera_ref, kEdsCameraCommand_ShutterButton_OFF);
                case Action::PRESS_FULL:
                    return _shutter_control(camera->_camera_ref, kEdsCameraCommand_ShutterButton_Completely);
                case Action::PRESS_HALFWAY:
                    return _shutter_control(camera->_camera_ref, kEdsCameraCommand_ShutterButton_Halfway);
                case Action::PRESS_AND_RELEASE:
                    return _shutter_control(camera->_camera_ref, kEdsCameraCommand_ShutterButton_Completely) &&
                           _shutter_control(camera->_camera_ref, kEdsCameraCommand_ShutterButton_OFF);
            }
            return false;
        }
    private:
        inline static bool _shutter_control(EdsCameraRef camera, EdsInt32 param) {
            return EdsSendCommand(camera,
                                  kEdsCameraCommand_PressShutterButton,
                                  param) == EDS_ERR_OK;
        }

        Action _action;
    };

    class EDSDK::Camera::CommandSessionControl : public Command {
    public:
        enum class Action {OPEN, CLOSE};

        explicit CommandSessionControl(Action action) : Command{}, _action{action} {}

        bool dispatch(EDSDK::Camera* camera) override {
            switch (_action) {
                case Action::OPEN:
                    return _open_session(camera);
                case Action::CLOSE:
                    return _close_session(camera);
            }
            return false;
        }
    private:
        static bool _open_session(EDSDK::Camera* camera) {
            if (camera->_explicit_session_opened) {
                return false;
            }

            camera->_explicit_session_opened = (EdsOpenSession(camera->_camera_ref) == EDS_ERR_OK);
            return camera->_explicit_session_opened;
        }

        static bool _close_session(EDSDK::Camera* camera) {
            if (!camera->_explicit_session_opened) {
                return false;
            }

            camera->_explicit_session_opened = (EdsCloseSession(camera->_camera_ref) != EDS_ERR_OK);
            return !camera->_explicit_session_opened;
        }

        Action _action;
    };

    EDSDK::Camera::Camera(EdsCameraRef camera) : _camera_ref{camera}, _explicit_session_opened{false}, _command_queue{} {
        //loading initial properties values from camera
        EdsOpenSession(_camera_ref);

        std::scoped_lock properties_lock_guard{_properties.mutex};
        _properties.name = _retrieve_property<std::string>(kEdsPropID_ProductName);
        _properties.current_storage = _retrieve_property<std::string>(kEdsPropID_CurrentStorage);
        _properties.body_id = _retrieve_property<std::string>(kEdsPropID_BodyIDEx);
        _properties.firmware_version = _retrieve_property<std::string>(kEdsPropID_FirmwareVersion);

        _properties.image_quality = _retrieve_property<std::uint32_t>(kEdsPropID_ImageQuality);
        _properties.ae_mode = _retrieve_property<std::uint32_t>(kEdsPropID_AEMode);
        _properties.af_mode = _retrieve_property<std::uint32_t>(kEdsPropID_AFMode);
        _properties.lens_name = _retrieve_property<std::string>(kEdsPropID_LensName);

        _properties.white_balance = _retrieve_property<std::int32_t>(kEdsPropID_WhiteBalance);
        _properties.color_temperature = _retrieve_property<std::uint32_t>(kEdsPropID_ColorTemperature);
        _properties.color_space = _retrieve_property<std::uint32_t>(kEdsPropID_ColorSpace);
        _properties.drive_mode = _retrieve_property<std::uint32_t>(kEdsPropID_DriveMode);
        _properties.metering_mode = _retrieve_property<std::uint32_t>(kEdsPropID_MeteringMode);
        _properties.iso = _retrieve_property<std::uint32_t>(kEdsPropID_ISOSpeed);
        _properties.av = _retrieve_property<std::uint32_t>(kEdsPropID_Av);
        _properties.tv = _retrieve_property<std::uint32_t>(kEdsPropID_Tv);
        _properties.exposure_compensation = _retrieve_property<std::uint32_t>(kEdsPropID_ExposureCompensation);

        std::scoped_lock properties_constraints_lock_guard{_properties_constraints.mutex};
        _properties_constraints.white_balance = _retrieve_property_constraints(kEdsPropID_WhiteBalance);
        _properties_constraints.color_temperature = _retrieve_property_constraints(kEdsPropID_ColorTemperature);
        _properties_constraints.color_space = _retrieve_property_constraints(kEdsPropID_ColorSpace);
        _properties_constraints.drive_mode = _retrieve_property_constraints(kEdsPropID_DriveMode);
        _properties_constraints.metering_mode = _retrieve_property_constraints(kEdsPropID_MeteringMode);
        _properties_constraints.iso = _retrieve_property_constraints(kEdsPropID_ISOSpeed);
        _properties_constraints.av = _retrieve_property_constraints(kEdsPropID_Av);
        _properties_constraints.tv = _retrieve_property_constraints(kEdsPropID_Tv);
        _properties_constraints.exposure_compensation = _retrieve_property_constraints(kEdsPropID_ExposureCompensation);

        EdsCloseSession(_camera_ref);

        //starting dispatcher thread
        _stop_thread = false;
        _dispatcher_thread = std::thread{&EDSDK::Camera::_command_dispatcher, this};
    }

    EDSDK::Camera::~Camera()  {
        _stop_thread = true;
        _dispatcher_thread.join();

        if (_camera_ref) {
            EdsRelease(_camera_ref);
        }
    }

    void EDSDK::Camera:: shutter_button() {
        _command_queue.push(new EDSDK::Camera::CommandShutterControl(EDSDK::Camera::CommandShutterControl::Action::PRESS_AND_RELEASE));
    }

    void EDSDK::Camera::shutter_button_press() {
        _command_queue.push(new EDSDK::Camera::CommandShutterControl(EDSDK::Camera::CommandShutterControl::Action::PRESS_FULL));
    }

    void EDSDK::Camera::shutter_button_press_halfway() {
        _command_queue.push(new EDSDK::Camera::CommandShutterControl(EDSDK::Camera::CommandShutterControl::Action::PRESS_HALFWAY));
    }

    void EDSDK::Camera::shutter_button_release() {
        _command_queue.push(new EDSDK::Camera::CommandShutterControl(EDSDK::Camera::CommandShutterControl::Action::RELEASE));
    }

    bool EDSDK::Camera::update_shutdown_timer() {
        return EdsSendCommand(_camera_ref,
                              kEdsCameraCommand_ExtendShutDownTimer,
                              0) == EDS_ERR_OK;
    }

    void EDSDK::Camera::open_session() {
        _command_queue.push(new EDSDK::Camera::CommandSessionControl{EDSDK::Camera::CommandSessionControl::Action::OPEN});
    }

    void EDSDK::Camera::close_session() {
        _command_queue.push(new EDSDK::Camera::CommandSessionControl{EDSDK::Camera::CommandSessionControl::Action::CLOSE});
    }

    void EDSDK::Camera::lock_ui() {
        _command_queue.push(new EDSDK::Camera::CommandSetState{kEdsCameraStatusCommand_UILock});
    }

    void EDSDK::Camera::unlock_ui() {
        _command_queue.push(new EDSDK::Camera::CommandSetState{kEdsCameraStatusCommand_UIUnLock});
    }

    std::string EDSDK::Camera::get_name() {
        std::scoped_lock _{_properties.mutex};
        return _properties.name;
    }

    std::string EDSDK::Camera::get_current_storage() {
        std::scoped_lock _{_properties.mutex};
        return _properties.current_storage;
    }

    std::string EDSDK::Camera::get_body_id() {
        std::scoped_lock _{_properties.mutex};
        return _properties.body_id;
    }

    std::string EDSDK::Camera::get_firmware_version() {
        std::scoped_lock _{_properties.mutex};
        return _properties.firmware_version;
    }

    std::string EDSDK::Camera::get_image_quality() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ImageQuality, _properties.image_quality);
    }

    std::string EDSDK::Camera::get_ae_mode() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_AEMode, _properties.ae_mode);
    }

    std::string EDSDK::Camera::get_af_mode() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_AFMode, _properties.af_mode);
    }

    std::string EDSDK::Camera::get_lens_name() {
        std::scoped_lock _{_properties.mutex};
        return _properties.lens_name;
    }

    std::string EDSDK::Camera::get_white_balance() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_WhiteBalance, _properties.white_balance);
    }

    std::string EDSDK::Camera::get_color_temperature() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ColorTemperature, _properties.color_temperature);
    }

    std::string EDSDK::Camera::get_color_space() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ColorSpace, _properties.color_space);
    }

    std::string EDSDK::Camera::get_drive_mode() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_DriveMode, _properties.drive_mode);
    }

    std::string EDSDK::Camera::get_metering_mode() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_MeteringMode, _properties.metering_mode);
    }

    std::string EDSDK::Camera::get_iso() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ISOSpeed, _properties.iso);
    }

    std::string EDSDK::Camera::get_av() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_Av, _properties.av);
    }

    std::string EDSDK::Camera::get_tv() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_Tv, _properties.tv);
    }

    std::string EDSDK::Camera::get_exposure_compensation() {
        std::scoped_lock _{_properties.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ExposureCompensation, _properties.exposure_compensation);
    }

    std::vector<std::string> EDSDK::Camera::get_white_balance_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_WhiteBalance, _properties_constraints.white_balance);
    }

    std::vector<std::string> EDSDK::Camera::get_color_temperature_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ColorTemperature, _properties_constraints.color_temperature);
    }

    std::vector<std::string> EDSDK::Camera::get_color_space_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ColorSpace, _properties_constraints.color_space);
    }

    std::vector<std::string> EDSDK::Camera::get_drive_mode_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_DriveMode, _properties_constraints.drive_mode);
    }

    std::vector<std::string> EDSDK::Camera::get_metering_mode_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_MeteringMode, _properties_constraints.metering_mode);
    }

    std::vector<std::string> EDSDK::Camera::get_iso_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ISOSpeed, _properties_constraints.iso);
    }

    std::vector<std::string> EDSDK::Camera::get_av_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_Av, _properties_constraints.av);
    }

    std::vector<std::string> EDSDK::Camera::get_tv_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_Tv, _properties_constraints.tv);
    }

    std::vector<std::string> EDSDK::Camera::get_exposure_compensation_constraints() {
        std::scoped_lock _{_properties_constraints.mutex};
        return EDSDK::explain_prop_value(kEdsPropID_ExposureCompensation, _properties_constraints.exposure_compensation);
    }


    void EDSDK::Camera::set_white_balance(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.white_balance.size()) ?
                              UINT32_MAX :
                              _properties_constraints.white_balance[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_WhiteBalance, value});
    }

    void EDSDK::Camera::set_color_temperature(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.color_temperature.size()) ?
                              UINT32_MAX :
                              _properties_constraints.color_temperature[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_ColorTemperature, value});
    }

    void EDSDK::Camera::set_color_space(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.color_space.size()) ?
                              UINT32_MAX :
                              _properties_constraints.color_space[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_ColorSpace, value});
    }

    void EDSDK::Camera::set_drive_mode(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.drive_mode.size()) ?
                              UINT32_MAX :
                              _properties_constraints.drive_mode[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_DriveMode, value});
    }

    void EDSDK::Camera::set_metering_mode(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.metering_mode.size()) ?
                              UINT32_MAX :
                              _properties_constraints.metering_mode[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_MeteringMode, value});
    }

    void EDSDK::Camera::set_iso(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.iso.size()) ?
                              UINT32_MAX :
                              _properties_constraints.iso[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_ISOSpeed, value});
    }

    void EDSDK::Camera::set_av(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.av.size()) ?
                              UINT32_MAX :
                              _properties_constraints.av[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_Av, value});
    }

    void EDSDK::Camera::set_tv(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.tv.size()) ?
                              UINT32_MAX :
                              _properties_constraints.tv[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_Tv, value});
    }

    void EDSDK::Camera::set_exposure_compensation(std::uint32_t index_in_constraints) {
        std::scoped_lock _{_properties_constraints.mutex};
        std::uint32_t value = (index_in_constraints >= _properties_constraints.exposure_compensation.size()) ?
                              UINT32_MAX :
                              _properties_constraints.exposure_compensation[index_in_constraints];

        _command_queue.push(new EDSDK::Camera::CommandSetProperty{kEdsPropID_ExposureCompensation, value});
    }

    template <typename T>
    T EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id) {
        T value;
        EdsError err = EDS_ERR_OK;
        EdsDataType data_type;
        EdsUInt32 data_size;

        err = EdsGetPropertySize(_camera_ref, prop_id, 0, &data_type, &data_size);
        if (err == EDS_ERR_OK) {
            err = EdsGetPropertyData(_camera_ref, prop_id, 0, data_size, &value);
        }

        return err == EDS_ERR_OK ? value : T{};
    }

    template <>
    std::string EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id) {
        char value[EDS_MAX_NAME];
        EdsError err = EDS_ERR_OK;
        EdsDataType data_type;
        EdsUInt32 data_size;

        err = EdsGetPropertySize(_camera_ref, prop_id, 0, &data_type, &data_size);
        if (err == EDS_ERR_OK) {
            err = EdsGetPropertyData(_camera_ref, prop_id, 0, data_size, &value);
        }

        return err == EDS_ERR_OK ? std::string(value) : "";
    }

    std::vector<std::uint32_t> EDSDK::Camera::_retrieve_property_constraints(EdsUInt32 prop_id) {
        EdsError err = EDS_ERR_OK;
        EdsPropertyDesc desc;
        std::vector<std::uint32_t> res{};

        err = EdsGetPropertyDesc(_camera_ref, prop_id, &desc);
        if (err == EDS_ERR_OK) {
            for (std::uint32_t i = 0; i < desc.numElements; i++) {
                res.push_back(desc.propDesc[i]);
            }
        }

        return res;
    }

    void EDSDK::Camera::_command_dispatcher() {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (EdsOpenSession(_camera_ref) == EDS_ERR_OK) {
            _explicit_session_opened = true;
        }

        //setting event callbacks
        EdsSetPropertyEventHandler(_camera_ref,
                                   kEdsPropertyEvent_PropertyChanged,
                                   EDSDK::Camera::_property_changed_callback,
                                   this);

        while(!_stop_thread) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            EdsGetEvent();
            if (!_command_queue.empty()) {
                auto tmp = _command_queue.pop().value();
                tmp->dispatch(this);
                delete tmp;
            }
        }

        if (EdsCloseSession(_camera_ref) == EDS_ERR_OK) {
            _explicit_session_opened = false;
        }

        CoUninitialize();
    }

    EdsError EDSCALLBACK EDSDK::Camera::_property_changed_callback(EdsPropertyEvent event,
                                                    EdsPropertyID prop_id,
                                                    EdsUInt32 param,
                                                    EdsVoid *ctx) {
        auto camera = static_cast<EDSDK::Camera*>(ctx);
        std::scoped_lock _{camera->_properties.mutex};
        switch (prop_id) {
            case kEdsPropID_WhiteBalance:
                camera->_properties.white_balance = camera->_retrieve_property<std::uint32_t>(prop_id);
//                std::cout << "WB: " << explain_prop_value(prop_id,  camera->_properties.white_balance) << "\n";
                break;
            case kEdsPropID_ColorTemperature:
                camera->_properties.color_temperature = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_ColorSpace:
                camera->_properties.color_space = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_DriveMode:
                camera->_properties.drive_mode = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_MeteringMode:
                camera->_properties.metering_mode = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_ISOSpeed:
                camera->_properties.iso = camera->_retrieve_property<std::uint32_t>(prop_id);
//                std::cout << "ISO: " << explain_prop_value(prop_id, camera->_properties.iso) << "\n";
                break;
            case kEdsPropID_Av:
                camera->_properties.av = camera->_retrieve_property<std::uint32_t>(prop_id);
//                std::cout << "AV: " << explain_prop_value(prop_id, camera->_properties.av) << "\n";
                break;
            case kEdsPropID_Tv:
                camera->_properties.tv = camera->_retrieve_property<std::uint32_t>(prop_id);
//                std::cout << "TV: " << explain_prop_value(prop_id, camera->_properties.tv) << "\n";
                break;
            case kEdsPropID_ExposureCompensation:
                camera->_properties.exposure_compensation = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_ImageQuality:
                camera->_properties.image_quality = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_AEMode:
                camera->_properties.ae_mode = camera->_retrieve_property<std::uint32_t>(prop_id);
//                std::cout << "AE: " << explain_prop_value(prop_id, camera->_properties.ae_mode) << "\n";
                break;
            case kEdsPropID_AFMode:
                camera->_properties.af_mode = camera->_retrieve_property<std::uint32_t>(prop_id);
                break;
            case kEdsPropID_LensStatus:
                camera->_properties.lens_name = camera->_retrieve_property<std::string>(kEdsPropID_LensName);
//                std::cout << "Lens: " << camera->_properties.lens_name << "\n";
                break;
            default:
                return EDS_ERR_INVALID_PARAMETER;
        }
        return EDS_ERR_OK;
    }

} //namespace edsdk_w

