#ifndef EDSDK_WRAPPER_HPP
#define EDSDK_WRAPPER_HPP

#include <string>
#include <vector>
#include "EDSDKTypes.h"

namespace edsdk_w {
    class EDSDK {
    public:
        class Camera {
        public:
            //get_prop/set_prop

        private:
            explicit Camera(EdsCameraRef camera);

            ~Camera();

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

        Camera &get_camera();

        std::string explain_prop_value(std::uint32_t prop_id, std::uint32_t value);

    private:
        EDSDK();
        ~EDSDK();

        Camera *_camera;
    };


} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
