#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
#pragma once

#include <iostream>
#include <string>
class Settings {
  public:
    Settings(int argc, char const *argv[]);
    std::string get_program_path() const;
    std::string get_url() const;
    std::string get_folder_name() const;

  private:
    std::string path, url, name;
};

#endif  // SETTINGS_H_INCLUDED
