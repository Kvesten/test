cmake_minimum_required(VERSION 3.0)

project(http_parser)

find_package(CURL)

include_directories(include/settings)
include_directories(include/very_easy_curl)
include_directories(include/paralel_downloader)
include_directories(${CURL_INCLUDE_DIRS})

add_subdirectory(include/settings)
add_subdirectory(include/very_easy_curl)
add_subdirectory(include/paralel_downloader)

set(SOURCE_CPP main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_CPP})

target_link_libraries(${PROJECT_NAME}
                        settings
                        very_easy_curl
                        ${CURL_LIBRARIES}
                        paralel_downloader)
