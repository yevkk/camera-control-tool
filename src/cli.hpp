#ifndef CLI_HPP
#define CLI_HPP

#include "edsdk_wrapper.hpp"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <cstring>
#include <algorithm>

namespace edsdk_w::cli {
    class CLI {
    private:
        enum class Command {
            PROP, UI, SESSION, CAMERA, EXIT, HELP
        };

        static const std::map<std::string, const Command> _commands_map;

        static std::vector<std::string> parse_args(const std::string &args_string) {
            std::vector<std::string> args;

            std::istringstream isstream(args_string);
            std::string arg_word;
            while (isstream >> arg_word) {
                args.push_back(arg_word);
            }

            return args;
        };

    public:
        CLI() = delete;

        static void run(std::istream &is = std::cin, bool repeat_commands = false) {
            EDSDK& eds = EDSDK::get_instance();

            while (true) {
                std::cout << "#";
                std::string line;
                std::getline(is, line);
                if (repeat_commands) {
                    std::cout << line << "\n";
                }
                auto args = parse_args(line);

                if (args.empty() || !_commands_map.contains(args[0])) {
                    std::cout << "error: wrong command, enter `help` to get commands list\n";
                    continue;
                }

                auto cmd = _commands_map.find(args[0])->second;

                if ((cmd == Command::PROP ||
                    cmd == Command::UI ||
                    cmd == Command::SESSION) && !(eds.get_camera().has_value())) {
                    std::cout << "error: camera is not set\n";
                    continue;
                }

                bool result;
                switch (cmd) {
                    case Command::PROP: {
                        // TODO: implement
                        break;
                    }
                    case Command::UI: {
                        if (args.size() != 2) {
                            std::cout << "error: wrong number of arguments\n";
                        } else {
                            if (args[1] == "lock") {
                                result = eds.get_camera().value().get().lock_ui();
                                std::cout << (result ? "ui locked successfully\n" : "failed to lock ui\n");
                            } else if (args[1] == "unlock") {
                                result = eds.get_camera().value().get().unlock_ui();
                                std::cout << (result ? "ui unlocked successfully\n" : "failed to unlock ui\n");
                            } else {
                                std::cout << "error: unknown argument\n";
                            }
                        }
                        break;
                    }
                    case Command::SESSION: {
                        if (args.size() != 2) {
                            std::cout << "error: wrong number of arguments\n";
                        } else {
                            if (args[1] == "open") {
                                result = eds.get_camera().value().get().open_session();
                                std::cout << (result ? "session opened successfully\n" : "failed to open session\n");
                            } else if (args[1] == "close") {
                                result = eds.get_camera().value().get().close_session();
                                std::cout << (result ? "session closed successfully\n" : "failed to close session\n");
                            } else {
                                std::cout << "error: unknown argument\n";
                            }
                        }
                        break;
                    }
                    case Command::CAMERA: {
                        // TODO: implement
                        break;
                    }
                    case Command::HELP: {
                        std::cout << "\t- prop show <property> - print available values of <property> with corresponding indices\n\n";
                        std::cout << "\t- prop get <property> - print current value of <property>\n\n";
                        std::cout << "\t- prop set <property> <num> - set a new value to <property>, <num> represents the index of value in the list of available values\n\n";
                        std::cout << "\t- ui lock - lock ui on current camera\n\n";
                        std::cout << "\t- ui unlock - unlock ui on current camera\n\n";
                        std::cout << "\t- session open - explicitly open session with current camera\n\n";
                        std::cout << "\t- session close - explicitly close session with current camera\n\n";
                        std::cout << "\t- camera list - print a list of cameras connected to the computer with corresponding indices\n\n";
                        std::cout << "\t- camera set <num> - set a current camera, <num> represents the index of camera in the list of connected cameras\n\n";
                        std::cout << "\t- camera reset - reset current camera\n\n";
                        std::cout << "\t- exit - close application\n\n\n";
                        std::cout << "\t <property> argument for prop commands can be designated the following values: \n";
                        std::cout << "\t get: camera | serial | firmware | storage | ae | quality | lens \n";
                        std::cout << "\t get | set | show: wb | temperature | color_space | drive_mode | metering_mode | af_mode | av | tv | iso | exp_compensation \n";
                        break;
                    }
                    case Command::EXIT: {
                        return;
                    }
                    default: {
                    }
                }
            }
        }
    };


    const std::map<std::string, const CLI::Command> CLI::_commands_map = {
            {"prop",    CLI::Command::PROP},
            {"ui",      CLI::Command::UI},
            {"session", CLI::Command::SESSION},
            {"camera",  CLI::Command::CAMERA},
            {"exit",    CLI::Command::EXIT},
            {"help",    CLI::Command::HELP}
    };
} // namespace edsdk_w::cli

#ifdef WRAPPER_CLI_MAIN
int main() {
    edsdk_w::cli::CLI::run();
    return 0;
}
#endif

#endif //CLI_HPP