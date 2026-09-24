set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(src/styles)
include_directories(src/ribbon)
include_directories(include)

## EXCLUDE RIBBONDSGN!

file(GLOB_RECURSE ALL_RIBBON_SOURCES "*.cpp")
file(GLOB_RECURSE ALL_RIBBON_HEADERS "*.h")

set(RIBBON_SOURCES "")
set(RIBBON_HEADERS "")

foreach(SRC ${ALL_RIBBON_SOURCES})
    if(SRC MATCHES "_win\\.cpp$")
    else()
        list(APPEND RIBBON_SOURCES ${SRC})
    endif()
endforeach()

foreach(HDR ${ALL_RIBBON_HEADERS})
    if(HDR MATCHES "_win\\.h$")
    else()
        list(APPEND RIBBON_HEADERS ${HDR})
    endif()
endforeach()

add_library(QTitanRibbon STATIC ${RIBBON_SOURCES} ${RIBBON_HEADERS})
target_link_libraries(QTitanRibbon PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui)
