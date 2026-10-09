include(App)
include(Base)
include(Network)
include(Rendering)
include_directories(./sgCore)

file(GLOB_RECURSE SG_SOURCES *.cpp)
file(GLOB_RECURSE SG_HEADERS *.h)

add_library(sgCore STATIC ${SG_SOURCES} ${SG_HEADERS})