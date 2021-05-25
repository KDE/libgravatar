add_executable(gravatarwidget_gui gravatarwidget_gui.cpp)
ecm_mark_as_test(gravatarwidget_gui)
target_link_libraries(gravatarwidget_gui
  Qt::Core
  Qt::Gui
  KF5::Gravatar
  KF5::I18n
  Qt::Widgets
  KF5::CoreAddons
)

