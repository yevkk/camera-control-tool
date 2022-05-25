#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

#define DEFAULT_FILENAME "logs.txt"

namespace utils {
    class Logger {
    public:
        virtual void log(const std::string &message) = 0;

    protected:
        static void _log(std::ostream &os, const std::string &message) {
            os << std::put_time(_current_time(), TIME_FORMAT);
            os << message << "\n";
        }

    private:
        static const char TIME_FORMAT[];

        static std::tm* _current_time() {
            using namespace std::chrono;
            auto tmp_time= system_clock::to_time_t(system_clock::now());
            return std::localtime(&tmp_time);
        }
    };

    const char Logger::TIME_FORMAT[] = "[%d/%m/%y-%H:%M:%S] ";

    class FileLogger : public Logger {
    public:
        explicit FileLogger(std::string filename = DEFAULT_FILENAME) : Logger{}, _filename{std::move(filename)} {};

        void log(const std::string &message) override {
            std::ofstream ofs(_filename, std::ios::out | std::ios::app);
            Logger::_log(ofs, message);
        }
    private:
        std::string _filename;
    };

    class OStreamLogger : public Logger {
    public:
        explicit OStreamLogger(std::ostream &os) : Logger{}, _os{os} {};

        void log(const std::string &message) override {
            Logger::_log(_os, message);
        }
    private:
        std::ostream &_os;
    };
}

#endif //LOGGER_HPP
