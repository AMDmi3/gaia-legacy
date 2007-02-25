# depends
find_package(Qt4 REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

set(QT_USE_QTOPENGL)
find_package(OpenGL REQUIRED)

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
	message(STATUS "GPSD support not requested")
endif(WITH_GPSD)

# defs
add_definitions(${QT_DEFINITIONS})
add_definitions(-DDATADIR="\\"${CMAKE_INSTALL_PREFIX}/${DATADIR}\\"")
include_directories(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ${PNG_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GPS_INCLUDE_DIR} ../../lib/wwfetch)

# sources
file(GLOB gaia_SRCS *.cc)

set(gaia_MOC_HDRS
	ControlWidget.h
	GLWidget.h
	GeoCoordSpinbox.h
)

set(gaia_UIS
	ControlWidget.ui
)

# targets
qt4_wrap_ui(gaia_UIC_HDRS ${gaia_UIS})
qt4_wrap_cpp(gaia_MOC_SRCS ${gaia_MOC_HDRS})

add_executable(gaia ${gaia_SRCS} ${gaia_MOC_SRCS} ${gaia_UIC_HDRS})

target_link_libraries(gaia wwfetch ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${CURL_LIBRARIES} ${OPENGL_LIBRARIES} ${GPS_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if(WITH_INSTALL)
	install(PROGRAMS gaia DESTINATION ${BINDIR})
endif(WITH_INSTALL)
