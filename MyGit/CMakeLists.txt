# CMakeList.txt : CMake project for MyGit, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (MyGit "core/MyGit.cpp" "core/MyGit.h" "utils/Crypto.h" "utils/FileHandler.h" "utils/FileHandler.cpp" "utils/Crypto.cpp" "commands/Add.h" "commands/Add.cpp" "utils/zLibWrapper.h" "utils/ZLibWrapper.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MyGit PROPERTY CXX_STANDARD 20)
endif()

# Link OpenSSL and zlib to the MyGit executable
target_link_libraries(MyGit PRIVATE OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB)

# TODO: Add tests and install targets if needed.