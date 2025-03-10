#ifndef PLATFORM_FACTORY_HPP
#define PLATFORM_FACTORY_HPP

#include "platformInterface.hpp"
#include <memory>

std::unique_ptr<PlatformInterface> createPlatform();

#endif
