# Copyright (c) 2024, Litwak913
# At GPL-3.0 License

cmake_minimum_required(VERSION 3.20)
project(arkpets-integration)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Force path to set CMAKE_INSTALL_PREFIX" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT")

set(QT_MIN_VERSION "6.4.0")
set(QT_MAJOR_VERSION 6)
set(KF_MIN_VERSION 6)

find_package(ECM "5.100" REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

find_package(Qt6 CONFIG REQUIRED COMPONENTS
    Core
    Widgets
    DBus
)

find_package(KWin REQUIRED COMPONENTS
    kwin
)

set(arkpetsintegration_SOURCES
    main.cpp
    arkpetsintegration.cpp
)

qt_add_dbus_adaptor(arkpetsintegration_SOURCES org.kde.KWin.ArkPets.xml arkpetsintegration.h ArkPets::ArkPetsIntegration)

kcoreaddons_add_plugin(ArkPetsIntegration SOURCES ${arkpetsintegration_SOURCES} INSTALL_NAMESPACE "kwin/effects/plugins/")

target_link_libraries(ArkPetsIntegration PRIVATE
    KWin::kwin
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
