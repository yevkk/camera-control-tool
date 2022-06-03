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
            bool shutter_button();
            bool shutter_button_press();
            bool shutter_button_press_halfway();
            bool shutter_button_release(bool close_session = true);

            bool update_shutdown_timer();

            bool open_session();
            bool close_session();

            [[nodiscard]] std::string get_name() const;
            [[nodiscard]] std::string get_current_storage() const;
            [[nodiscard]] std::string get_body_id() const;
            [[nodiscard]] std::string get_firmware_version() const;

            [[nodiscard]] std::string get_image_quality() const;
            [[nodiscard]] std::string get_ae_mode() const;
            [[nodiscard]] std::string get_af_mode() const;
            [[nodiscard]] std::string get_lens_name() const;

            [[nodiscard]] std::string get_white_balance() const;
            [[nodiscard]] std::string get_color_temperature() const;
            [[nodiscard]] std::string get_color_space() const;
            [[nodiscard]] std::string get_drive_mode() const;
            [[nodiscard]] std::string get_metering_mode() const;
            [[nodiscard]] std::string get_iso() const;
            [[nodiscard]] std::string get_av() const;
            [[nodiscard]] std::string get_tv() const;
            [[nodiscard]] std::string get_exposure_compensation() const;

            [[nodiscard]] std::vector<std::string> get_white_balance_constraints() const;
            [[nodiscard]] std::vector<std::string> get_color_temperature_constraints() const;
            [[nodiscard]] std::vector<std::string> get_color_space_constraints() const;
            [[nodiscard]] std::vector<std::string> get_drive_mode_constraints() const;
            [[nodiscard]] std::vector<std::string> get_metering_mode_constraints() const;
            [[nodiscard]] std::vector<std::string> get_iso_constraints() const;
            [[nodiscard]] std::vector<std::string> get_av_constraints() const;
            [[nodiscard]] std::vector<std::string> get_tv_constraints() const;
            [[nodiscard]] std::vector<std::string> get_exposure_compensation_constraints() const;

            bool set_white_balance(std::uint32_t index_in_constraints);
            bool set_color_temperature(std::uint32_t index_in_constraints);
            bool set_color_space(std::uint32_t index_in_constraints);
            bool set_drive_mode(std::uint32_t index_in_constraints);
            bool set_metering_mode(std::uint32_t index_in_constraints);
            bool set_iso(std::uint32_t index_in_constraints);
            bool set_av(std::uint32_t index_in_constraints);
            bool set_tv(std::uint32_t index_in_constraints);
            bool set_exposure_compensation(std::uint32_t index_in_constraints);

        private:
            explicit Camera(EdsCameraRef camera);

            ~Camera();

            inline bool _shutter_button_command(EdsInt32 param);

            template <typename T>
            T _retrieve_property(EdsUInt32 prop_id);

            std::vector<std::uint32_t> _retrieve_property_constraints(EdsUInt32 prop_id);

            bool _set_property(EdsUInt32 prop_id,
                               std::uint32_t *prop_ptr,
                               const std::vector<std::uint32_t> &constraints,
                               std::uint32_t value_index);

            static EdsError EDSCALLBACK _property_changed_callback(EdsPropertyEvent event,
                                                                   EdsPropertyID prop_id,
                                                                   EdsUInt32 param,
                                                                   EdsVoid *ctx);

            static EdsError EDSCALLBACK _property_desc_changed_callback(EdsPropertyEvent event,
                                                                   EdsPropertyID prop_id,
                                                                   EdsUInt32 param,
                                                                   EdsVoid *ctx);

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
                std::uint32_t white_balance;
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
            bool _explicit_session_opened;

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
    std::string EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id);

} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
