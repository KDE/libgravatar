# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(gravatarwidget_gui gravatarwidget_gui.cpp)
ecm_mark_as_test(gravatarwidget_gui)
target_link_libraries(gravatarwidget_gui
  Qt${QT_MAJOR_VERSION}::Core
  Qt${QT_MAJOR_VERSION}::Gui
  KF5::Gravatar
  KF5::I18n
  Qt${QT_MAJOR_VERSION}::Widgets
  KF5::CoreAddons
)

