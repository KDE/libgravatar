/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfigwidgettest.h"
#include "../src/widgets/gravatarconfigwidget.h"
#include <QCheckBox>
#include <QTest>
#include <QPushButton>
#include <qtestmouse.h>
#include <QSignalSpy>

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
    QCheckBox *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));
    QVERIFY(enableGravatar);

    QPushButton *configure = w.findChild<QPushButton *>(QStringLiteral("configure"));
    QVERIFY(configure);
}

void GravatarConfigWidgetTest::shouldChangeState()
{
    Gravatar::GravatarConfigWidget w;
    w.show();
    QCheckBox *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));

    QPushButton *configure = w.findChild<QPushButton *>(QStringLiteral("configure"));
    QVERIFY(!configure->isEnabled());
    enableGravatar->toggle();
    QVERIFY(configure->isEnabled());
}

void GravatarConfigWidgetTest::shoulEmitConfigChangedSignal()
{
    Gravatar::GravatarConfigWidget w;
    w.show();
    QCheckBox *enableGravatar = w.findChild<QCheckBox *>(QStringLiteral("gravatarcheckbox"));

    QSignalSpy spy(&w, &Gravatar::GravatarConfigWidget::configChanged);
    QTest::mouseClick(enableGravatar, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(GravatarConfigWidgetTest)
