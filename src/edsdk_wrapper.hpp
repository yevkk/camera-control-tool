#ifndef EDSDK_WRAPPER_HPP
#define EDSDK_WRAPPER_HPP

#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <queue>
#include <atomic>
#include <mutex>
#include <thread>
#include "EDSDKTypes.h"

namespace edsdk_w {
    namespace utils {
        template<typename T>
        class Queue {
        public:
            Queue() = default;
            Queue(const Queue<T> &) = delete ;
            Queue& operator=(const Queue<T> &) = delete ;

            Queue(Queue<T>&& other) noexcept;

            [[nodiscard]] bool empty();

            [[nodiscard]] std::uint32_t size();

            std::optional<T> pop();

            void push(const T &item);

        private:
            std::queue<T> _queue;
            std::mutex _mutex;
        };
    } //namespace utils

    class EDSDK {
    public:
        class Camera {
        private:
            class Command;
            class CommandSetProperty;
            class CommandSetState;
            class CommandShutterControl;
            class CommandSessionControl;
        public:
            void shutter_button();
            void shutter_button_press();
            void shutter_button_press_halfway();
            void shutter_button_release();

            bool update_shutdown_timer();

            void open_session();
            void close_session();

            void lock_ui();
            void unlock_ui();

            [[nodiscard]] std::string get_name();
            [[nodiscard]] std::string get_current_storage();
            [[nodiscard]] std::string get_body_id();
            [[nodiscard]] std::string get_firmware_version();

            [[nodiscard]] std::string get_image_quality();
            [[nodiscard]] std::string get_ae_mode();
            [[nodiscard]] std::string get_af_mode();
            [[nodiscard]] std::string get_lens_name();

            [[nodiscard]] std::string get_white_balance();
            [[nodiscard]] std::string get_color_temperature();
            [[nodiscard]] std::string get_color_space();
            [[nodiscard]] std::string get_drive_mode();
            [[nodiscard]] std::string get_metering_mode();
            [[nodiscard]] std::string get_iso();
            [[nodiscard]] std::string get_av();
            [[nodiscard]] std::string get_tv();
            [[nodiscard]] std::string get_exposure_compensation();

            [[nodiscard]] std::vector<std::string> get_white_balance_constraints();
            [[nodiscard]] std::vector<std::string> get_color_temperature_constraints();
            [[nodiscard]] std::vector<std::string> get_color_space_constraints();
            [[nodiscard]] std::vector<std::string> get_drive_mode_constraints();
            [[nodiscard]] std::vector<std::string> get_metering_mode_constraints();
            [[nodiscard]] std::vector<std::string> get_iso_constraints();
            [[nodiscard]] std::vector<std::string> get_av_constraints();
            [[nodiscard]] std::vector<std::string> get_tv_constraints();
            [[nodiscard]] std::vector<std::string> get_exposure_compensation_constraints();

            void set_white_balance(std::uint32_t index_in_constraints);
            void set_color_temperature(std::uint32_t index_in_constraints);
            void set_color_space(std::uint32_t index_in_constraints);
            void set_drive_mode(std::uint32_t index_in_constraints);
            void set_metering_mode(std::uint32_t index_in_constraints);
            void set_iso(std::uint32_t index_in_constraints);
            void set_av(std::uint32_t index_in_constraints);
            void set_tv(std::uint32_t index_in_constraints);
            void set_exposure_compensation(std::uint32_t index_in_constraints);

        private:
             explicit Camera(EdsCameraRef camera);

            ~Camera();

            template <typename T>
            T _retrieve_property(EdsUInt32 prop_id);

            std::vector<std::uint32_t> _retrieve_property_constraints(EdsUInt32 prop_id);

            void _command_dispatcher();

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

                std::mutex mutex;
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

                std::mutex mutex;
            } _properties_constraints;

            EdsCameraRef _camera_ref;
            std::atomic_bool _explicit_session_opened;
            utils::Queue<Command*> _command_queue;

            std::atomic_bool _stop_thread;
            std::thread _dispatcher_thread;

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
