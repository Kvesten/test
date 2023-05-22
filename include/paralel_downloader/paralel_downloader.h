#ifndef PARALEL_DOWNLOADER_H_INCLUDED
#define PARALEL_DOWNLOADER_H_INCLUDED
#pragma once

#include <curl/curl.h>
#include <mutex>
#include <queue>
#include <stdio.h>
#include <string>
#include <thread>

class Paralel_downloader {
  public:
    void download(const std::string &url, const std::string &filename);
    void done();

  private:
    std::queue<std::pair<std::string, std::string>> que;
    void que_parser();
    void download_file(const std::string &url, const std::string &filename);
    std::mutex que_mutex;
    bool downloading_is_active = 0;
    std::mutex downloading_mutex;
    std::thread *th;
};

#endif  // PARALEL_DOWNLOADER_H_INCLUDED
