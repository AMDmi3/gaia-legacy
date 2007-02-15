# project name
project(gaia)

# cmake setup
#set(CMAKE_VERBOSE_MAKEFILE ON)		# for myself, while learning cmake

# defs
set(GAIA_VERSION "0.1.3")
add_definitions(-Wall -g -DGAIA_VERSION="\\"${GAIA_VERSION}\\"")

# subdirs (everything is in subdirs)
subdirs(lib/wwfetch)
subdirs(programs/gaia)
