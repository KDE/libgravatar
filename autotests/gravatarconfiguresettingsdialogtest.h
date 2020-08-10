/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARCONFIGURESETTINGSDIALOGTEST_H
#define GRAVATARCONFIGURESETTINGSDIALOGTEST_H

#include <QObject>

class GravatarConfigureSettingsDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsDialogTest(QObject *parent = nullptr);
    ~GravatarConfigureSettingsDialogTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // GRAVATARCONFIGURESETTINGSDIALOGTEST_H
