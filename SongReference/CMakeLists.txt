# CMakeList.txt : CMake project for SongReference, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SongReference "SongReference.cpp" "SongReference.h" "song.h" "song.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SongReference PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
