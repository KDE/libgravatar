/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARCONFIGURESETTINGSWIDGETTEST_H
#define GRAVATARCONFIGURESETTINGSWIDGETTEST_H

#include <QObject>

class GravatarConfigureSettingsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsWidgetTest(QObject *parent = nullptr);
    ~GravatarConfigureSettingsWidgetTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

#endif // GRAVATARCONFIGURESETTINGSWIDGETTEST_H
