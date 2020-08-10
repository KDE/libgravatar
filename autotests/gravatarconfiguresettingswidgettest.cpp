/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
