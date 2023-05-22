#include "settings.h"

Settings::Settings(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "Invalid input arguments" << std::endl;
        exit(1);
    }
    this->path = argv[0];
    this->url = argv[1];
    this->name = argv[2];
}

std::string Settings::get_program_path() const { return this->path; }

std::string Settings::get_url() const { return this->url; }

std::string Settings::get_folder_name() const { return this->name; }
