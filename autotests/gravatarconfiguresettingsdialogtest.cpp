/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfiguresettingsdialogtest.h"
#include "../src/widgets/gravatarconfiguresettingsdialog.h"
#include "../src/widgets/gravatarconfiguresettingswidget.h"
#include <QTest>
#include <QVBoxLayout>

GravatarConfigureSettingsDialogTest::GravatarConfigureSettingsDialogTest(QObject *parent)
    : QObject(parent)
{
}

GravatarConfigureSettingsDialogTest::~GravatarConfigureSettingsDialogTest()
{
}

void GravatarConfigureSettingsDialogTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarConfigureSettingsDialog dlg;
    auto *configurewidget = dlg.findChild<Gravatar::GravatarConfigureSettingsWidget *>(QStringLiteral("gravatarconfiguresettings"));
    QVERIFY(configurewidget);

    auto *topLayout = dlg.findChild<QVBoxLayout *>(QStringLiteral("toplayout"));
    QVERIFY(topLayout);
}

QTEST_MAIN(GravatarConfigureSettingsDialogTest)
