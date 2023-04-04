# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
add_executable(gravatarwidget_gui gravatarwidget_gui.cpp)
ecm_mark_as_test(gravatarwidget_gui)
target_link_libraries(gravatarwidget_gui
  Qt::Core
  Qt::Gui
  KPim6::Gravatar
  KF6::I18n
  Qt::Widgets
  KF6::CoreAddons
)

