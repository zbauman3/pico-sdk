cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

execute_process(COMMAND jq --raw-output ".entry" ${CMAKE_CURRENT_LIST_DIR}/config.json
  OUTPUT_VARIABLE CONF_ENTRY
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND jq --raw-output ".name" ${CMAKE_CURRENT_LIST_DIR}/config.json
  OUTPUT_VARIABLE CONF_NAME
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND jq --raw-output ".pio // empty" ${CMAKE_CURRENT_LIST_DIR}/config.json
  OUTPUT_VARIABLE CONF_PIO_NAME
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

project("${CONF_NAME}_project" C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable("${CONF_NAME}")

if(NOT (CONF_PIO_NAME STREQUAL ""))
  pico_generate_pio_header("${CONF_NAME}" "${CMAKE_CURRENT_LIST_DIR}/src/${CONF_PIO_NAME}")
endif()

target_sources("${CONF_NAME}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src/${CONF_ENTRY}")

pico_enable_stdio_usb("${CONF_NAME}" 1)
pico_enable_stdio_uart("${CONF_NAME}" 0)

target_link_libraries("${CONF_NAME}" PRIVATE pico_stdlib hardware_pio)

pico_add_extra_outputs("${CONF_NAME}")