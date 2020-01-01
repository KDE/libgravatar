/*
   Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>

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

#include "gravatarconfiguresettingswidgettest.h"
#include "../src/widgets/gravatarconfiguresettingswidget.h"
#include <KPluralHandlingSpinBox>
#include <KSeparator>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(GravatarConfigureSettingsWidgetTest)

GravatarConfigureSettingsWidgetTest::GravatarConfigureSettingsWidgetTest(QObject *parent)
    : QObject(parent)
{

}

void GravatarConfigureSettingsWidgetTest::shouldHaveDefaultValues()
{
    Gravatar::GravatarConfigureSettingsWidget w;

    QVBoxLayout *topLayout = w.findChild<QVBoxLayout* >(QStringLiteral("mainlayout"));
    QVERIFY(topLayout);
    QCOMPARE(topLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QCheckBox *useDefaultImage = w.findChild<QCheckBox *>(QStringLiteral("usedefaultimage"));
    QVERIFY(useDefaultImage);

    QPushButton *clearGravatarCache = w.findChild<QPushButton *>(QStringLiteral("cleargravatarcachebutton"));
    QVERIFY(clearGravatarCache);

    QLabel *lab = w.findChild<QLabel *>(QStringLiteral("gravatarcachesizelabel"));
    QVERIFY(lab);

    KPluralHandlingSpinBox *gravatarCacheSize = w.findChild<KPluralHandlingSpinBox *>(QStringLiteral("gravatarcachesize"));
    QVERIFY(gravatarCacheSize);

    KSeparator *separator = w.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);

    KSeparator *separator2 = w.findChild<KSeparator *>(QStringLiteral("separator2"));
    QVERIFY(separator2);
}
