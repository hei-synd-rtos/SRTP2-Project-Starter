cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C)

target_sources(app PRIVATE
        src/main.c
        src/blinky.h src/blinky.c
        src/time.h src/time.c
        src/nunchuk.h src/nunchuk.c
        src/chat.h src/chat.c
)

target_include_directories(app PRIVATE ${ZEPHYR_BASE}/include/zephyr/portability)
