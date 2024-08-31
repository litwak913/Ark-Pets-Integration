cmake_minimum_required(VERSION 3.20)
project(arkpets-integration)

set(KF6_MIN_VERSION "6.0.0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

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

qt_add_dbus_adaptor(arkpetsintegration_SOURCES org.kde.KWin.ArkPets.xml arkpetsintegration.hpp KWin::ArkPetsIntegration)

kcoreaddons_add_plugin(ArkPetsIntegration SOURCES ${arkpetsintegration_SOURCES} INSTALL_NAMESPACE "kwin/plugins")

target_link_libraries(ArkPetsIntegration PRIVATE
    KWin::kwin
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
