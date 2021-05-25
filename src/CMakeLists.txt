
add_definitions(-DTRANSLATION_DOMAIN=\"libgravatar\")
add_library(KF5Gravatar)
add_library(KF5::Gravatar ALIAS KF5Gravatar)

target_sources(KF5Gravatar PRIVATE
    misc/gravatarcache.cpp
    misc/hash.cpp
    widgets/gravatardownloadpixmapwidget.cpp
    widgets/gravatardownloadpixmapdialog.cpp
    widgets/gravatarconfigwidget.cpp
    widgets/gravatarconfiguresettingswidget.cpp
    widgets/gravatarconfiguresettingsdialog.cpp
    job/gravatarresolvurljob.cpp
)

ecm_qt_declare_logging_category(KF5Gravatar HEADER gravatar_debug.h IDENTIFIER GRAVATAR_LOG CATEGORY_NAME org.kde.pim.gravatar
        OLD_CATEGORY_NAMES log_gravatar
        DESCRIPTION "libgravatar (libgravatar)" EXPORT GRAVATAR)

kconfig_add_kcfg_files(KF5Gravatar
    settings/gravatarsettings.kcfgc
)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KF5Gravatar PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KF5Gravatar BASE_NAME gravatar)

target_link_libraries(KF5Gravatar
  PRIVATE
    KF5::ConfigCore
    KF5::I18n
    KF5::WidgetsAddons
    KF5::ConfigGui
    KF5::PimCommon
    KF5::TextWidgets
)
target_include_directories(KF5Gravatar INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/Gravatar/;${KDE_INSTALL_INCLUDEDIR_KF5}/gravatar>")

target_include_directories(KF5Gravatar PUBLIC "$<BUILD_INTERFACE:${gravatar_SOURCE_DIR}/src;${gravatar_BINARY_DIR}/src>")


set_target_properties(KF5Gravatar PROPERTIES
    VERSION ${GRAVATAR_VERSION}
    SOVERSION ${GRAVATAR_SOVERSION}
    EXPORT_NAME Gravatar
)

install(TARGETS
    KF5Gravatar
    EXPORT KF5GravatarTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
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
  GravatarConfigWidget
  GravatarDownloadPixmapWidget
  GravatarConfigureSettingsWidget
  REQUIRED_HEADERS Gravatar_widgets_HEADERS
  PREFIX Gravatar
  RELATIVE widgets
)


ecm_generate_pri_file(BASE_NAME Gravatar
    LIB_NAME KF5Gravatar
    DEPS "" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/Gravatar
)

install(FILES
    ${Gravatar_Camelcase_widgets_HEADERS}
    ${Gravatar_Camelcase_job_HEADERS}
    ${Gravatar_Camelcase_misc_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/Gravatar
    COMPONENT Devel
)

install(FILES
    ${Gravatar_widgets_HEADERS}
    ${Gravatar_job_HEADERS}
    ${Gravatar_misc_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/gravatar_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/gravatarsettings.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/gravatar
    COMPONENT Devel
)
install(FILES
    ${PRI_FILENAME}
    DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

ecm_qt_install_logging_categories(EXPORT GRAVATAR FILE libgravatar.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
