# depends
FIND_PACKAGE(Qt4 REQUIRED)

# defs
ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE_DIRECTORIES(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ../../lib)

# sources
FILE(GLOB qgaia_SRCS *.cc)

FILE(GLOB qgaia_MOC_HDRS [A-Z]*.h)

FILE(GLOB qgaia_UIS *.ui)

# targets
QT4_WRAP_UI(qgaia_UIC_HDRS ${qgaia_UIS})
QT4_WRAP_CPP(qgaia_MOC_SRCS ${qgaia_MOC_HDRS})
QT4_ADD_RESOURCES(qgaia_RCCS qgaia.qrc)

ADD_EXECUTABLE(qgaia ${qgaia_SRCS} ${qgaia_MOC_SRCS} ${qgaia_UIC_HDRS} ${qgaia_RCCS})

TARGET_LINK_LIBRARIES(qgaia ${QT_QTOPENGL_LIBRARY} "-Wl,--whole-archive" gaia "-Wl,--no-whole-archive")

# alternatice way of proper linking libgaia (or make libgaia dynamic)
#FILE(GLOB libgaia_SRCS ../../lib/libgaia/*.cc)
#ADD_EXECUTABLE(qgaia ${qgaia_SRCS} ${libgaia_SRCS} ${qgaia_MOC_SRCS} ${qgaia_UIC_HDRS})
#TARGET_LINK_LIBRARIES(qgaia ${QT_QTOPENGL_LIBRARY})

SET_TARGET_PROPERTIES(qgaia PROPERTIES LINK_FLAGS "-rdynamic")
