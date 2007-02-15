# depends
find_package(CURL REQUIRED)

# defs
add_definitions(-DGAIA_VERSION="\\"${GAIA_VERSION}\\"")
include_directories(${CURL_INCLUDE_DIR})

# sources
file(GLOB wwfetch_SRCS *.c)

# targets
add_library(wwfetch ${wwfetch_SRCS})
