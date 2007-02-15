project(gaia)

# checks
cmake_minimum_required(VERSION 2.4.0)

# depends
find_package(Qt3 REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CURL REQUIRED)

# defs
add_definitions(${QT_DEFINITIONS})
link_directories(${QT_LIB_DIR} ${PNG_LIB_DIR} ${JPEG_LIB_DIR} ${OPENGL_LIB_DIR} ${CURL_LIB_DIR})
include_directories(${QT_INCLUDE_DIR} ${PNG_INCLUDE_DIR} ${JPEG_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ../../lib/wwfetch)

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

target_link_libraries(gaia wwfetch ${QT_LIBRARIES} ${PNG_LIBRARIES} ${JPEG_LIBRARIES} ${CURL_LIBRARIES} ${OPENGL_LIBRARIES})
