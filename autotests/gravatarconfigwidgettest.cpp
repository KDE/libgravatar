/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfigwidgettest.h"
#include "../src/widgets/gravatarconfigwidget.h"
#include <QCheckBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <qtestmouse.h>

GravatarConfigWidgetTest::GravatarConfigWidgetTest(QObject *parent)
    : QObject(parent)
{
}

GravatarConfigWidgetTest::~GravatarConfigWidgetTest()
{
}

void GravatarConfigWidgetTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarConfigWidget w;
    auto *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));
    QVERIFY(enableGravatar);

    auto *configure = w.findChild<QPushButton *>(QStringLiteral("configure"));
    QVERIFY(configure);
}

void GravatarConfigWidgetTest::shouldChangeState()
{
    Gravatar::GravatarConfigWidget w;
    w.show();
    auto *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));

    auto *configure = w.findChild<QPushButton *>(QStringLiteral("configure"));
    QVERIFY(!configure->isEnabled());
    enableGravatar->toggle();
    QVERIFY(configure->isEnabled());
}

void GravatarConfigWidgetTest::shoulEmitConfigChangedSignal()
{
    Gravatar::GravatarConfigWidget w;
    w.show();
    auto *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));

    QSignalSpy spy(&w, &Gravatar::GravatarConfigWidget::configChanged);
    QTest::mouseClick(enableGravatar, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(GravatarConfigWidgetTest)
