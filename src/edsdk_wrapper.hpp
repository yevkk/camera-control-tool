#ifndef EDSDK_WRAPPER_HPP
#define EDSDK_WRAPPER_HPP

namespace edsdk_w {
    class EDSDK {
    public:
        static EDSDK& get_instance();

        EDSDK(EDSDK const&) = delete;
        void operator=(EDSDK &) = delete;
    private:
        EDSDK();
        ~EDSDK();
    };


} //namespace edsdk_w

#endif //EDSDK_WRAPPER_HPP
