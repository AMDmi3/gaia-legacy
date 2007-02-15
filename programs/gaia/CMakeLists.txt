project(gaia)

# checks
cmake_minimum_required(VERSION 2.4.0)

# depends
find_package(Qt3 REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

# options
option(WITH_GPSD	"Enable GPSD support"	OFF)

# custom library search
if(WITH_GPSD)
	find_path(GPS_INCLUDE_DIR NAMES gps.h)
	find_library(GPS_LIBRARY NAMES gps)
	if(NOT GPS_INCLUDE_DIR OR NOT GPS_LIBRARY)
		MESSAGE(FATAL_ERROR "Could not find GPS library")
	endif(NOT GPS_INCLUDE_DIR OR NOT GPS_LIBRARY)
	message(STATUS "Found GPS: ${GPS_LIBRARY}")
	add_definitions(-DWITH_GPSD)
else(WITH_GPSD)
	MESSAGE(STATUS "GPSD support not requested")
endif(WITH_GPSD)

# defs
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDE_DIR} ${PNG_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GPS_INCLUDE_DIR} ../../lib/wwfetch)

# sources
file(GLOB gaia_SRCS *.cc)

set(gaia_MOC_HDRS
	ControlWidget.h
	GLWidget.h
	MainWidget.h
	DoubleSpinBox.h
)

# targets
qt_wrap_cpp(gaia gaia_MOC_SRCS ${gaia_MOC_HDRS})

add_executable(gaia ${gaia_SRCS} ${gaia_MOC_SRCS})

target_link_libraries(gaia wwfetch ${QT_LIBRARIES} ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${CURL_LIBRARIES} ${OPENGL_LIBRARIES} ${GPS_LIBRARY})
