# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "6.2.80")

project(gravatar VERSION ${PIM_VERSION})

set(KF_MIN_VERSION "6.6.0")
set(QT_REQUIRED_VERSION "6.7.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h tests/*.cpp tests/*.h autotests/*.h autotests/*.cpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(ECMFeatureSummary)
include(ECMAddTests)
include(ECMCheckOutboundLicense)
file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h tests/*.cpp tests/*.h autotests/*.h autotests/*.cpp)
ecm_check_outbound_license(LICENSES GPL-2.0-only  FILES ${ALL_SOURCE_FILES})

include(ECMAddQch)

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")


set(LIBGRAVATAR_LIB_VERSION ${PIM_VERSION})
set(PIMCOMMON_LIB_VERSION "6.2.40")
find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets Network)
find_package(KF6I18n ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6Config ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6WidgetsAddons ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6ConfigWidgets ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} CONFIG REQUIRED)
find_package(KPim6PimCommon ${PIMCOMMON_LIB_VERSION} CONFIG REQUIRED)

ecm_setup_version(PROJECT VARIABLE_PREFIX GRAVATAR
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/gravatar_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KPim6GravatarConfigVersion.cmake"
    SOVERSION 6
)


########### Targets ###########


add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
ecm_set_disabled_deprecation_versions(QT 6.8.0 KF 6.8.0)


option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()
if(BUILD_TESTING)
   add_definitions(-DBUILD_TESTING)
endif()


add_subdirectory(src)

########### CMake Config Files ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KPim6Gravatar")
if(BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KPim6Gravatar_QCH
        FILE KPim6GravatarQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
)
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KPim6GravatarQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KPimGravatarConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GravatarConfig.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GravatarConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KPim6GravatarConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KPim6GravatarTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KPim6GravatarTargets.cmake NAMESPACE KPim6::)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/gravatar_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/Gravatar COMPONENT Devel
)

if(BUILD_TESTING)
    find_package(Qt6 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Test)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif()

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
ecm_feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
