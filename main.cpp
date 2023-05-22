#include "include/paralel_downloader/paralel_downloader.h"
#include "include/settings/settings.h"
#include "include/very_easy_curl/very_easy_curl.h"
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sys/stat.h>

// #define DEBUG

std::set<std::string>  // Пытаюсь вытащить картинки из html.... Не работает как
                       // надо!
extract_img(const std::string &html_str) {
    static const std::regex hl_regex("<img src=\"(.*?)\"",
                                     std::regex_constants::icase);

    return {std::sregex_token_iterator(html_str.begin(), html_str.end(),
                                       hl_regex, 1),
            std::sregex_token_iterator{}};
}

int main(const int argc, const char *argv[]) {
#ifdef DEBUG
    std::cout << "Start" << std::endl;
#endif  // DEBUG

    Settings settings(argc, argv);
    Paralel_downloader pd;

    std::string str_response = url_to_str(settings.get_url());

    mkdir(settings.get_folder_name().c_str(), ACCESSPERMS);

    const auto hlinks = extract_img(str_response);

    std::map<std::string, std::string> img_map;

    // Если есть картинки то качаем и заменяем пути в html
    if (!hlinks.empty()) {
        std::string img_path = settings.get_folder_name() + "/img";
        mkdir(img_path.c_str(), ACCESSPERMS);

        for (auto &i : hlinks) {
            std::string filename(i, i.find_last_of("/"), i.length());
            img_map[i] = "/img" + filename;
        }

        for (auto &i : img_map) {
            // Если ссылка относительная (/img/1.png)
            std::string url = i.first.find("://") == std::string::npos
                                  ? settings.get_url() + i.first
                                  : i.first;
            pd.download(url, settings.get_folder_name() + i.second);
#ifdef DEBUG
            std::cout << str_response.find(i.first) << std::endl;
#endif  // DEBUG
            str_response =
                std::regex_replace(str_response, std::regex(i.first), i.second);
#ifdef DEBUG
            std::cout << i.first << std::endl;
            std::cout << i.second << std::endl;
#endif  // DEBUG
        }
    }

    std::string index_html_path = settings.get_folder_name() + "/index.html";
    std::ofstream html_out(index_html_path.c_str());
    if (html_out.is_open()) {
        html_out << str_response;
        html_out.close();
    } else {
        std::cerr << "Failed create file index.html" << std::endl;
        return (1);
    }

    pd.done();

#ifdef DEBUG
    std::cout << "Good end" << std::endl;
#endif  // DEBUG
    return 0;
}
