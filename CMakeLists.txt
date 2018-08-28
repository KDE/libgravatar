cmake_minimum_required(VERSION 3.0)
set(PIM_VERSION "5.9.40")

project(gravatar VERSION ${PIM_VERSION})

set(KF5_VERSION "5.49.0")

find_package(ECM ${KF5_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(LIBRARY_NAMELINK)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(ECMAddTests)
include(ECMCoverageOption)


set(QT_REQUIRED_VERSION "5.9.0")
set(LIBGRAVATAR_LIB_VERSION ${PIM_VERSION})
set(PIMCOMMON_LIB_VERSION "5.9.40")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Test Network)
find_package(KF5I18n ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5Config ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5WidgetsAddons ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5TextWidgets ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5KIO ${KF5_VERSION} CONFIG REQUIRED)
find_package(KF5PimCommon ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)

ecm_setup_version(PROJECT VARIABLE_PREFIX GRAVATAR
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/gravatar_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5GravatarConfigVersion.cmake"
    SOVERSION 5
    )


########### Targets ###########

add_definitions("-DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII")
add_definitions(-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Gravatar")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5GravatarConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GravatarConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GravatarConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5GravatarConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT KF5GravatarTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5GravatarTargets.cmake NAMESPACE KF5::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/gravatar_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
    )
if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif()

add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

install( FILES libgravatar.renamecategories libgravatar.categories DESTINATION ${KDE_INSTALL_CONFDIR} )
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
