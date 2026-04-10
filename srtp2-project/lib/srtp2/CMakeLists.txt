zephyr_library()
zephyr_library_sources(
        src/protocol/offsets.h
        src/protocol/frame.h src/protocol/frame.c
        src/protocol/beacon.h src/protocol/beacon.c
        src/protocol/mpdu.h src/protocol/mpdu.c
        src/service/config.h
        src/main.c
        src/service/sync.c
        src/service/sv.c
        src/service/ev.c
        src/service/msg.c
        src/network.c
)
zephyr_include_directories(include)
zephyr_library_include_directories(app PRIVATE ${ZEPHYR_BASE}/include/zephyr/portability)
