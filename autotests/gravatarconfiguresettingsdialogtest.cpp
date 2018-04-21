/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "gravatarconfiguresettingsdialogtest.h"
#include "../src/widgets/gravatarconfiguresettingsdialog.h"
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <KPluralHandlingSpinBox>
#include <QTest>
#include <KSeparator>

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
    QCheckBox *useDefaultImage = dlg.findChild<QCheckBox *>(QStringLiteral("usedefaultimage"));
    QVERIFY(useDefaultImage);

    QPushButton *clearGravatarCache = dlg.findChild<QPushButton *>(QStringLiteral("cleargravatarcachebutton"));
    QVERIFY(clearGravatarCache);

    QLabel *lab = dlg.findChild<QLabel *>(QStringLiteral("gravatarcachesizelabel"));
    QVERIFY(lab);

    KPluralHandlingSpinBox *gravatarCacheSize = dlg.findChild<KPluralHandlingSpinBox *>(QStringLiteral("gravatarcachesize"));
    QVERIFY(gravatarCacheSize);


    QLabel *label = dlg.findChild<QLabel *>(QStringLiteral("labelwarning"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    KSeparator *separator = dlg.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);

    KSeparator *separator2 = dlg.findChild<KSeparator *>(QStringLiteral("separator2"));
    QVERIFY(separator2);
}

QTEST_MAIN(GravatarConfigureSettingsDialogTest)
