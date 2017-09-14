/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "gravatarconfigwidgettest.h"
#include "../src/widgets/gravatarconfigwidget.h"
#include <QCheckBox>
#include <qtest.h>
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

    QSignalSpy spy(&w, SIGNAL(configChanged(bool)));
    QTest::mouseClick(enableGravatar, Qt::LeftButton);
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(GravatarConfigWidgetTest)
