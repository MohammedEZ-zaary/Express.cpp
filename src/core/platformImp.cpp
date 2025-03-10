#include "../../include/core/platformInterfaces/linuxPlatform.hpp"
#include "../../include/core/platformInterfaces/platFormFactory.hpp"
#include "../../include/core/platformInterfaces/windowsPlatform.hpp"

std::unique_ptr<PlatformInterface> createPlatform(){
    
    #ifdef _WIN32
        return std::make_unique<WindowsPlatform>();
    #elif __linux__
        return std::make_unique<LinuxPlatform>() ;
    #else
        throw std::runtime_error("Unsupported platform");
    #endif
};

