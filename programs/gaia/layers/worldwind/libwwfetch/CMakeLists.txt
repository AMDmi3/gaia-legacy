# depends
find_package(CURL REQUIRED)

# defs
include_directories(${CURL_INCLUDE_DIR})

# sources
file(GLOB wwfetch_SRCS *.c)

# targets
add_library(wwfetch ${wwfetch_SRCS})
