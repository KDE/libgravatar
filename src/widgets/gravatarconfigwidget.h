/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef GRAVATARCONFIGWIDGET_H
#define GRAVATARCONFIGWIDGET_H

#include <QWidget>
#include "gravatar_export.h"
namespace Gravatar {
class GravatarConfigWidgetPrivate;

/** Gravatar configuration widget. */
class GRAVATAR_EXPORT GravatarConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GravatarConfigWidget(QWidget *parent = nullptr);
    ~GravatarConfigWidget();

    void save();
    void doLoadFromGlobalSettings();
    void doResetToDefaultsOther();

Q_SIGNALS:
    void configChanged(bool);

private:
    void slotGravatarEnableChanged(bool state);
    void slotConfigureSettings();
    void updateWidgetState(bool state);
    GravatarConfigWidgetPrivate *const d;
};
}

#endif // GRAVATARCONFIGWIDGET_H
