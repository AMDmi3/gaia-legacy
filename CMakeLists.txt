# project name
project(gaia)

# cmake setup
set(CMAKE_VERBOSE_MAKEFILE ON)		# for myself, while learning cmake
#set(CMAKE_USE_RELATIVE_PATH ON)	# how to make ../../lib/wwfetch remain relative?

# defs
set(GAIA_VERSION "0.1.3")
add_definitions(-Wall -g)

# subdirs (everything is in subdirs)
subdirs(lib/wwfetch)
subdirs(programs/gaia)
