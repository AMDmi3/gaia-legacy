# depends
FIND_PACKAGE(Qt4 REQUIRED)

# defs
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ../../libgaia)

# sources
FILE(GLOB qgaia_SRCS *.cc)

SET(qgaia_MOC_HDRS
	ControlWidget.h
	GLWidget.h
)

SET(qgaia_UIS
	ControlWidget.ui
)

# targets
QT4_WRAP_UI(qgaia_UIC_HDRS ${qgaia_UIS})
QT4_WRAP_CPP(qgaia_MOC_SRCS ${qgaia_MOC_HDRS})

ADD_EXECUTABLE(qgaia ${qgaia_SRCS} ${qgaia_MOC_SRCS} ${qgaia_UIC_HDRS})

TARGET_LINK_LIBRARIES(qgaia gaia ${QT_QTOPENGL_LIBRARY})
