/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
