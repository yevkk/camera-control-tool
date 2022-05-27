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
            std::string get_name();
            std::string get_current_storage();
            std::string get_body_id();
            std::string get_firmware_version();

            std::string get_image_quality();
            std::string get_ae_mode();
            std::string get_af_mode();
            std::string get_lens_name();

            std::string get_white_balance();
            std::string get_color_temperature();
            std::string get_color_space();
            std::string get_drive_mode();
            std::string get_metering_mode();
            std::string get_iso();
            std::string get_av();
            std::string get_tv();
            std::string get_exposure_compensation();

            std::vector<std::string> get_white_balance_constraints();
            std::vector<std::string> get_color_temperature_constraints();
            std::vector<std::string> get_color_space_constraints();
            std::vector<std::string> get_drive_mode_constraints();
            std::vector<std::string> get_metering_mode_constraints();
            std::vector<std::string> get_iso_constraints();
            std::vector<std::string> get_av_constraints();
            std::vector<std::string> get_tv_constraints();
            std::vector<std::string> get_exposure_compensation_constraints();

        private:
            class SessionRAII;

            explicit Camera(EdsCameraRef camera);

            ~Camera();

            template <typename T>
            T _retrieve_property(EdsUInt32 prop_id, bool open_session = true);

            std::vector<std::uint32_t> _retrieve_property_constraints(EdsUInt32 prop_id, bool open_session = true);

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
                std::vector<std::uint32_t> white_balance;
                std::vector<std::uint32_t> color_temperature;
                std::vector<std::uint32_t> color_space;
                std::vector<std::uint32_t> drive_mode;
                std::vector<std::uint32_t> metering_mode;
                std::vector<std::uint32_t> iso;
                std::vector<std::uint32_t> av;
                std::vector<std::uint32_t> tv;
                std::vector<std::uint32_t> exposure_compensation;
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

        static std::vector<std::string> explain_prop_value(std::uint32_t prop_id, const std::vector<std::uint32_t> &value);

    private:
        EDSDK();
        ~EDSDK();

        Camera *_camera;
    };

    template <>
    std::string EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id, bool open_session);

} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
