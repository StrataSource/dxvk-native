#include "util_env.h"

 #include <array>
#include <filesystem>

#if defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace dxvk::env {

  std::string getExePath() {
#if defined(__linux__)
    std::array<char, PATH_MAX> exePath = {};

    size_t count = readlink("/proc/self/exe", exePath.data(), exePath.size());

    return std::string(exePath.begin(), exePath.begin() + count);
#elif defined(__APPLE__)
    std::array<char, PATH_MAX> exePath = {};

    uint32_t count = PATH_MAX;
    _NSGetExecutablePath(exePath.data(), &count);

    return std::string(exePath.begin(), exePath.begin() + count);
#endif
  }
  
  
  void setThreadName(const std::string& name) {
    std::array<char, 16> posixName = {};
    strlcpy(posixName.data(), name.c_str(), 16);
    #ifdef __APPLE__
      ::pthread_setname_np(posixName.data());
    #else
      ::pthread_setname_np(pthread_self(), posixName.data());
    #endif
  }


  bool createDirectory(const std::string& path) {
    return std::filesystem::create_directories(path);
  }

}