#include "paralel_downloader.h"

void Paralel_downloader::download(const std::string &url,
                                  const std::string &filename) {
    que_mutex.lock();
    que.push(std::make_pair(url, filename));
    que_mutex.unlock();
    downloading_mutex.lock();
    bool dow_is_act_tmp = downloading_is_active;
    downloading_mutex.unlock();
    if (!dow_is_act_tmp) {
        th = new std::thread([&]() { que_parser(); });
    }
}

void Paralel_downloader::que_parser() {
    downloading_mutex.lock();
    downloading_is_active = 1;
    downloading_mutex.unlock();
    while (1) {
        std::string url, filename;
        bool f = 0;
        que_mutex.lock();
        if (!que.empty()) {
            url = que.front().first;
            filename = que.front().second;
            que.pop();
            f = 1;
        }
        que_mutex.unlock();
        if (f) {
            download_file(url, filename);
        } else {
            break;
        }
    }
    downloading_mutex.lock();
    downloading_is_active = 0;
    downloading_mutex.unlock();
}

void Paralel_downloader::download_file(const std::string &url,
                                       const std::string &filename) {
    CURL *curl;
    FILE *fp;
    curl = curl_easy_init();
    if (curl) {
        if ((fp = fopen(filename.c_str(), "wb"))) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }
}

void Paralel_downloader::done() { th->join(); }
