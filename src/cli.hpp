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
            PROP, UI, SESSION, CAMERA, SHUTTER, EXIT, HELP
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

        static void print_values(const std::vector<std::string> &v) {
            if (v.empty()) {
                std::cout << "No available values\n";
            } else {
                for (int i = 0 ; i < v.size(); i++) {
                    std::cout << i + 1 << " - " << v[i] << std::endl;
                }
            }
        }

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

                eds.events();
                bool result;
                switch (cmd) {
                    case Command::PROP: {
                        if (args.size() == 3 && args[1] == "get") {
                            auto prop_arg = args[2];
                            if (prop_arg == "camera_name") {
                                std::cout << "Camera: " << eds.get_camera().value().get().get_name() << std::endl;
                            } else if (prop_arg == "serial") {
                                std::cout << "Serial: " << eds.get_camera().value().get().get_body_id() << std::endl;
                            } else if (prop_arg == "firmware") {
                                std::cout << "Firmware version: " << eds.get_camera().value().get().get_firmware_version() << std::endl;
                            } else if (prop_arg == "storage") {
                                std::cout << "Storage card type: " << eds.get_camera().value().get().get_current_storage() << std::endl;
                            } else if (prop_arg == "ae") {
                                std::cout << "AE: " << eds.get_camera().value().get().get_ae_mode() << std::endl;
                            } else if (prop_arg == "quality") {
                                std::cout << "Image quality: " << eds.get_camera().value().get().get_image_quality() << std::endl;
                            } else if (prop_arg == "lens") {
                                std::cout << "Lens: " << eds.get_camera().value().get().get_lens_name() << std::endl;
                            } else if (prop_arg == "wb") {
                                std::cout << "WB: " << eds.get_camera().value().get().get_white_balance() << std::endl;
                            } else if (prop_arg == "temperature") {
                                std::cout << "Color temperature: " << eds.get_camera().value().get().get_color_temperature() << std::endl;
                            } else if (prop_arg == "color_space") {
                                std::cout << "Color Space: " << eds.get_camera().value().get().get_color_space() << std::endl;
                            } else if (prop_arg == "drive_mode") {
                                std::cout << "Drive Mode: " << eds.get_camera().value().get().get_drive_mode() << std::endl;
                            } else if (prop_arg == "metering_mode") {
                                std::cout << "Metering Mode: " << eds.get_camera().value().get().get_metering_mode() << std::endl;
                            } else if (prop_arg == "af_mode") {
                                std::cout << "AF Mode: " << eds.get_camera().value().get().get_af_mode() << std::endl;
                            } else if (prop_arg == "av") {
                                std::cout << "Av: " << eds.get_camera().value().get().get_av() << std::endl;
                            } else if (prop_arg == "tv") {
                                std::cout << "Tv: " << eds.get_camera().value().get().get_tv() << std::endl;
                            } else if (prop_arg == "iso") {
                                std::cout << "ISO: " << eds.get_camera().value().get().get_iso() << std::endl;
                            } else if (prop_arg == "exp_compensation") {
                                std::cout << "Exposure Compensation: " << eds.get_camera().value().get().get_exposure_compensation() << std::endl;
                            } else {
                                std::cout << "error: unknown argument\n";
                            }
                        } else if (args.size() == 3 && args[1] == "show") {
                            std::vector<std::string> value_list;
                            auto prop_arg = args[2];

                            if (prop_arg == "wb") {
                                std::cout << "WB: ";
                                value_list = eds.get_camera().value().get().get_white_balance_constraints();
                            } else if (prop_arg == "temperature") {
                                std::cout << "Color temperature:\n";
                                value_list = eds.get_camera().value().get().get_color_temperature_constraints();
                            } else if (prop_arg == "color_space") {
                                std::cout << "Color Space:\n";
                                value_list = eds.get_camera().value().get().get_color_space_constraints();
                            } else if (prop_arg == "drive_mode") {
                                std::cout << "Drive Mode:\n";
                                value_list = eds.get_camera().value().get().get_drive_mode_constraints();
                            } else if (prop_arg == "metering_mode") {
                                std::cout << "Metering Mode:\n";
                                value_list = eds.get_camera().value().get().get_metering_mode_constraints();
                            } else if (prop_arg == "av") {
                                std::cout << "Av:\n";
                                value_list = eds.get_camera().value().get().get_av_constraints();
                            } else if (prop_arg == "tv") {
                                std::cout << "Tv:\n";
                                value_list = eds.get_camera().value().get().get_tv_constraints();
                            } else if (prop_arg == "iso") {
                                std::cout << "ISO:\n";
                                value_list = eds.get_camera().value().get().get_iso_constraints();
                            } else if (prop_arg == "exp_compensation") {
                                std::cout << "Exposure Compensation:\n";
                                value_list = eds.get_camera().value().get().get_exposure_compensation_constraints();
                            } else {
                                std::cout << "error: unknown argument\n";
                                break;
                            }

                            print_values(value_list);
                        } else if (args.size() == 4 && args[1] == "set") {
                            std::vector<std::string> value_list;
                            auto prop_arg = args[2];
                            auto value_index = std::stoi(args[3]) - 1;

                            if (prop_arg == "wb") {
                                result = eds.get_camera().value().get().set_white_balance(value_index);
                            } else if (prop_arg == "temperature") {
                                result = eds.get_camera().value().get().set_color_temperature(value_index);
                            } else if (prop_arg == "color_space") {
                                result = eds.get_camera().value().get().set_color_space(value_index);
                            } else if (prop_arg == "drive_mode") {
                                result = eds.get_camera().value().get().set_drive_mode(value_index);
                            } else if (prop_arg == "metering_mode") {
                                result = eds.get_camera().value().get().set_metering_mode(value_index);
                            } else if (prop_arg == "av") {
                                result = eds.get_camera().value().get().set_av(value_index);
                            } else if (prop_arg == "tv") {
                                result = eds.get_camera().value().get().set_tv(value_index);
                            } else if (prop_arg == "iso") {
                                result = eds.get_camera().value().get().set_iso(value_index);
                            } else if (prop_arg == "exp_compensation") {
                                result = eds.get_camera().value().get().set_exposure_compensation(value_index);
                            } else {
                                std::cout << "error: unknown argument\n";
                                break;
                            }
                            std::cout << (result ? "value set successfully\n" : "failed to set value\n");
                        } else {
                            std::cout << "error: wrong number of arguments\n";
                        }
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
                    case Command::SHUTTER: {
                        if (args.size() == 1) {
                            result = eds.get_camera().value().get().shutter_button();
                        } else if (args.size() != 2) {
                            std::cout << "error: wrong number of arguments\n";
                            break;
                        } else if (args[1] == "full") {
                            result = eds.get_camera().value().get().shutter_button_press();
                        } else if (args[1] == "half") {
                            result = eds.get_camera().value().get().shutter_button_press_halfway();
                        } else if (args[1] == "release") {
                            result = eds.get_camera().value().get().shutter_button_release();
                        }
                        std::cout << (result ? "success\n" : "failed\n");
                        break;
                    }
                    case Command::CAMERA: {
                        if (args.size() == 2 && args[1] == "list") {
                            auto camera_list = eds.get_available_camera_list();
                            if (camera_list.empty()) {
                                std::cout << "No connected cameras\n";
                            } else {
                                for (int i = 0 ; i < camera_list.size(); i++) {
                                    std::cout << i + 1 << " - " << camera_list[i] << std::endl;
                                }
                            }
                        } else if (args.size() == 2 && args[1] == "reset") {
                            result = eds.reset_camera();
                            std::cout << (result ? "camera reset successfully\n" : "failed to reset camera\n");
                        } else if (args.size() == 3 && args[1] == "set") {
                            result = eds.set_camera(std::stoi(args[2]) - 1);
                            std::cout << (result ? "camera set successfully\n" : "failed to set camera\n");
                        } else {
                            std::cout << "error: wrong number of arguments\n";
                        }
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
                        std::cout << "\t- shutter - full press and release of shutter button\n\n";
                        std::cout << "\t- shutter full - full press of shutter button\n\n";
                        std::cout << "\t- shutter half - halfway press of shutter button\n\n";
                        std::cout << "\t- shutter release - release of shutter button\n\n";
                        std::cout << "\t- camera list - print a list of cameras connected to the computer with corresponding indices\n\n";
                        std::cout << "\t- camera set <num> - set a current camera, <num> represents the index of camera in the list of connected cameras\n\n";
                        std::cout << "\t- camera reset - reset current camera\n\n";
                        std::cout << "\t- exit - close application\n\n\n";
                        std::cout << "\t <property> argument for prop commands can be designated the following values: \n";
                        std::cout << "\t get: camera_name | serial | firmware | storage | ae | af_mode | quality | lens \n";
                        std::cout << "\t get | set | show: wb | temperature | color_space | drive_mode | metering_mode | av | tv | iso | exp_compensation \n";
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
            {"shutter", CLI::Command::SHUTTER},
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