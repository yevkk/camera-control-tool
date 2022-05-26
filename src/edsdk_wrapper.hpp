#ifndef EDSDK_WRAPPER_HPP
#define EDSDK_WRAPPER_HPP

#include <string>
#include <vector>
#include <optional>
#include <functional>
#include "EDSDKTypes.h"

namespace edsdk_w {
    class EDSDK {
    public:
        class Camera {
        public:
            //get_prop/set_prop

        private:
            class SessionRAII;

            explicit Camera(EdsCameraRef camera);

            ~Camera();

            template <typename T>
            T _retrieve_property(EdsUInt32 prop_id, bool open_session = true);

            struct {
                //immutable
                std::string name;
                std::string current_storage;
                std::string body_id;
                std::string firmware_version;

                //read-only
                std::uint32_t image_quality;
                std::uint32_t ae_mode;
                std::uint32_t af_mode;
                std::string lens_name;

                //settable
                std::int32_t white_balance;
                std::uint32_t color_temperature;
                std::uint32_t color_space;
                std::uint32_t drive_mode;
                std::uint32_t metering_mode;
                std::uint32_t iso;
                std::uint32_t av;
                std::uint32_t tv;
                std::uint32_t exposure_compensation;
            } _properties;

            struct {
                //props_available_values
            } _properties_constraints;

            EdsCameraRef _camera_ref;

            friend EDSDK;
        };

    public:
        static EDSDK& get_instance();

        EDSDK(EDSDK const&) = delete;
        void operator=(EDSDK &) = delete;

        std::vector<std::string> get_available_camera_list();

        bool set_camera(std::uint8_t index_in_list);

        std::optional<std::reference_wrapper<Camera>> get_camera();

        bool reset_camera();

        static std::string explain_prop_value(std::uint32_t prop_id, std::uint32_t value);

    private:
        EDSDK();
        ~EDSDK();

        Camera *_camera;
    };

    template <>
    std::string EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id, bool open_session);

} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
