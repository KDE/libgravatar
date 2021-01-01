/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatardownloadpixmapwidgettest.h"
#include "../src/widgets/gravatardownloadpixmapwidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>

GravatarDownloadPixmapWidgetTest::GravatarDownloadPixmapWidgetTest(QObject *parent)
    : QObject(parent)
{
}

GravatarDownloadPixmapWidgetTest::~GravatarDownloadPixmapWidgetTest()
{
}

void GravatarDownloadPixmapWidgetTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarDownloadPixmapWidget w;
    auto *lab = w.findChild<QLabel *>(QStringLiteral("labemail"));
    QVERIFY(lab);

    auto *lineEdit = w.findChild<QLineEdit *>(QStringLiteral("email"));
    QVERIFY(lineEdit);

    auto *getPixmapButton = w.findChild<QPushButton *>(QStringLiteral("searchbutton"));
    QVERIFY(getPixmapButton);
    QVERIFY(!getPixmapButton->isEnabled());

    auto *resultLabel = w.findChild<QLabel *>(QStringLiteral("resultlabel"));
    QVERIFY(resultLabel);

    auto *useLibravatar = w.findChild<QCheckBox *>(QStringLiteral("uselibravatar"));
    QVERIFY(useLibravatar);
    QVERIFY(!useLibravatar->isChecked());

    auto *fallBackGravatar = w.findChild<QCheckBox *>(QStringLiteral("fallbackgravatar"));
    QVERIFY(fallBackGravatar);
    QVERIFY(!fallBackGravatar->isChecked());
}

void GravatarDownloadPixmapWidgetTest::shouldChangeButtonEnableState()
{
    Gravatar::GravatarDownloadPixmapWidget w;
    auto *lineEdit = w.findChild<QLineEdit *>(QStringLiteral("email"));

    auto *getPixmapButton = w.findChild<QPushButton *>(QStringLiteral("searchbutton"));
    QVERIFY(!getPixmapButton->isEnabled());

    lineEdit->setText(QStringLiteral("foo"));
    QVERIFY(getPixmapButton->isEnabled());

    lineEdit->setText(QStringLiteral("   "));
    QVERIFY(!getPixmapButton->isEnabled());
}

QTEST_MAIN(GravatarDownloadPixmapWidgetTest)
