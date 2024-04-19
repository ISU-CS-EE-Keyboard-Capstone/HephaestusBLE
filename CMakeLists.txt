# CMakeList.txt : CMake project for HephaestusBLE, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (HephaestusBLE "HephaestusBLE.cpp" "HephaestusBLE.h" "libBle.h" "SingletonSupport.h" "libBle.cpp" "SingletonSupport.cpp" "pch.h" "main.cpp")
#add_library (HephaestusBLElib "HephaestusBLE.cpp" "HephaestusBLE.h" "libBle.h" "SingletonSupport.h" "libBle.cpp" "SingletonSupport.cpp" "pch.h" "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HephaestusBLE PROPERTY CXX_STANDARD 20)
endif()
if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET HephaestusBLElib PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
