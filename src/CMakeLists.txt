# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_definitions(-DTRANSLATION_DOMAIN=\"libgravatar\")
add_library(KPim${KF_MAJOR_VERSION}Gravatar)
add_library(KPim${KF_MAJOR_VERSION}::Gravatar ALIAS KPim${KF_MAJOR_VERSION}Gravatar)


target_sources(KPim${KF_MAJOR_VERSION}Gravatar PRIVATE
    misc/gravatarcache.cpp
    misc/hash.cpp
    widgets/gravatardownloadpixmapwidget.cpp
    widgets/gravatardownloadpixmapdialog.cpp
    widgets/gravatarconfiguresettingswidget.cpp
    widgets/gravatarconfiguresettingsdialog.cpp
    job/gravatarresolvurljob.cpp
    job/gravatarresolvurljob.h
    misc/hash.h
    misc/gravatarcache.h
    gravatar_private_export.h
    widgets/gravatarconfiguresettingswidget.h
    widgets/gravatardownloadpixmapdialog.h
    widgets/gravatardownloadpixmapwidget.h
    widgets/gravatarconfiguresettingsdialog.h

)

ecm_qt_declare_logging_category(KPim${KF_MAJOR_VERSION}Gravatar HEADER gravatar_debug.h IDENTIFIER GRAVATAR_LOG CATEGORY_NAME org.kde.pim.gravatar
        OLD_CATEGORY_NAMES log_gravatar
        DESCRIPTION "libgravatar (libgravatar)" EXPORT GRAVATAR)

kconfig_add_kcfg_files(KPim${KF_MAJOR_VERSION}Gravatar
    settings/gravatarsettings.kcfgc
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim${KF_MAJOR_VERSION}Gravatar PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim${KF_MAJOR_VERSION}Gravatar BASE_NAME gravatar)

target_link_libraries(KPim${KF_MAJOR_VERSION}Gravatar
  PRIVATE
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::WidgetsAddons
    KF${KF_MAJOR_VERSION}::ConfigGui
    KPim${KF_MAJOR_VERSION}::PimCommon
    KF${KF_MAJOR_VERSION}::TextWidgets
)
target_include_directories(KPim${KF_MAJOR_VERSION}Gravatar INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Gravatar/>")

target_include_directories(KPim${KF_MAJOR_VERSION}Gravatar PUBLIC "$<BUILD_INTERFACE:${gravatar_SOURCE_DIR}/src;${gravatar_BINARY_DIR}/src>")


set_target_properties(KPim${KF_MAJOR_VERSION}Gravatar PROPERTIES
    VERSION ${GRAVATAR_VERSION}
    SOVERSION ${GRAVATAR_SOVERSION}
    EXPORT_NAME Gravatar
)

install(TARGETS
    KPim${KF_MAJOR_VERSION}Gravatar
    EXPORT KPim${KF_MAJOR_VERSION}GravatarTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

ecm_generate_headers(Gravatar_Camelcase_job_HEADERS
  HEADER_NAMES
  GravatarResolvUrlJob
  REQUIRED_HEADERS Gravatar_job_HEADERS
  PREFIX Gravatar
  RELATIVE job
)

ecm_generate_headers(Gravatar_Camelcase_misc_HEADERS
  HEADER_NAMES
  GravatarCache
  REQUIRED_HEADERS Gravatar_misc_HEADERS
  PREFIX Gravatar
  RELATIVE misc
)

ecm_generate_headers(Gravatar_Camelcase_widgets_HEADERS
  HEADER_NAMES
  GravatarConfigureSettingsDialog
  GravatarDownloadPixmapWidget
  GravatarConfigureSettingsWidget
  REQUIRED_HEADERS Gravatar_widgets_HEADERS
  PREFIX Gravatar
  RELATIVE widgets
)


ecm_generate_pri_file(BASE_NAME Gravatar
    LIB_NAME KPim${KF_MAJOR_VERSION}Gravatar
    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Gravatar/Gravatar
)

install(FILES
    ${Gravatar_Camelcase_widgets_HEADERS}
    ${Gravatar_Camelcase_job_HEADERS}
    ${Gravatar_Camelcase_misc_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Gravatar/Gravatar
    COMPONENT Devel
)

install(FILES
    ${Gravatar_widgets_HEADERS}
    ${Gravatar_job_HEADERS}
    ${Gravatar_misc_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/gravatar_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/gravatarsettings.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim${KF_MAJOR_VERSION}/Gravatar/gravatar
    COMPONENT Devel
)

install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

ecm_qt_install_logging_categories(EXPORT GRAVATAR FILE libgravatar.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})


if (BUILD_QCH)
    ecm_add_qch(
        KPim${KF_MAJOR_VERSION}Gravatar_QCH
        NAME KPim${KF_MAJOR_VERSION}Gravatar
        BASE_NAME KPim${KF_MAJOR_VERSION}Gravatar
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${Gravatar_Camelcase_widgets_HEADERS}
            ${Gravatar_Camelcase_job_HEADERS}
            ${Gravatar_Camelcase_misc_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt${QT_MAJOR_VERSION}Core_QCH
            Qt${QT_MAJOR_VERSION}Gui_QCH
            Qt${QT_MAJOR_VERSION}Widgets_QCH
            Qt5Networks_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            GRAVATAR_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
