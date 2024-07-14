cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/config.json MY_JSON_STRING)
string(JSON CONF_ENTRY GET ${MY_JSON_STRING} entry)
string(JSON CONF_NAME GET ${MY_JSON_STRING} name)

project("${CONF_NAME}_project" C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable("${CONF_NAME}" "src/${CONF_ENTRY}")

pico_enable_stdio_usb("${CONF_NAME}" 1)
pico_enable_stdio_uart("${CONF_NAME}" 1)

pico_add_extra_outputs("${CONF_NAME}")

target_link_libraries("${CONF_NAME}" pico_stdlib)