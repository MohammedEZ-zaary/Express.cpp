#include "../../include/platformInterfaces/linuxPlatform.hpp"
#include "../../include/platformInterfaces/platFormFactory.hpp"
#include "../../include/platformInterfaces/windowsPlatform.hpp"

std::unique_ptr<PlatformInterface> createPlatform(){
    
    #ifdef _WIN32
        return std::make_unique<WindowsPlatform>();
    #elif __linux__
        return std::make_unique<LinuxPlatform>() ;
    #else
        throw std::runtime_error("Unsupported platform");
    #endif
};

