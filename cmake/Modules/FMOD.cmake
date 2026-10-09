file(TO_CMAKE_PATH "$ENV{FMOD_LINUX_ROOT}" FMOD_ROOT)
link_directories("${FMOD_ROOT}/lib/x86_64")
include_directories("${FMOD_ROOT}/inc")