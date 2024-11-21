if (CONFIG_TCS34725)
    # zephyr_include_directories()
    zephyr_library_sources(tcs34725.c)
endif()
