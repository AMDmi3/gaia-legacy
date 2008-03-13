# project name
PROJECT(gaia)

# checks
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

# common definitions
SET(GAIA_VERSION "0.1.4")

ADD_DEFINITIONS(-Wall -g
		-DGAIA_VERSION="\\"${GAIA_VERSION}\\""
		-DGAIA_LAYERDIR="\\"${PROJECT_BINARY_DIR}/layers\\"")

# depends
FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(lib/ ${OPENGL_INCLUDE_DIR})

# subdirs (everything is in subdirs)
SUBDIRS(lib)
SUBDIRS(layers)
SUBDIRS(frontends)
