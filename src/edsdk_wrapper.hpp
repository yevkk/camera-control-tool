#ifndef EDSDK_WRAPPER_HPP
#define EDSDK_WRAPPER_HPP

#include <string>

namespace edsdk_w {
    class EDSDK {
    public:
        static EDSDK& get_instance();

        EDSDK(EDSDK const&) = delete;
        void operator=(EDSDK &) = delete;

        std::string explain_prop_value(std::uint32_t prop_id, std::uint32_t value);
    private:
        EDSDK();
        ~EDSDK();
    };


} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
