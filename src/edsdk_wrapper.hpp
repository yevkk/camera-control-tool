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
            T _retrieve_property(EdsUInt32 prop_id);

            struct {
                //props
            } properties;

            struct {
                //props_available_values
            } properties_constraints;

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
    std::string EDSDK::Camera::_retrieve_property(EdsUInt32 prop_id);

} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
