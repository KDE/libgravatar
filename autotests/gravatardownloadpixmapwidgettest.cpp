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

#include "gravatardownloadpixmapwidgettest.h"
#include "../src/widgets/gravatardownloadpixmapwidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <qtest.h>

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
    QLabel *lab = w.findChild<QLabel *>(QStringLiteral("labemail"));
    QVERIFY(lab);

    QLineEdit *lineEdit = w.findChild<QLineEdit *>(QStringLiteral("email"));
    QVERIFY(lineEdit);

    QPushButton *getPixmapButton = w.findChild<QPushButton *>(QStringLiteral("searchbutton"));
    QVERIFY(getPixmapButton);
    QVERIFY(!getPixmapButton->isEnabled());

    QLabel *resultLabel = w.findChild<QLabel *>(QStringLiteral("resultlabel"));
    QVERIFY(resultLabel);

    QCheckBox *useLibravatar = w.findChild<QCheckBox *>(QStringLiteral("uselibravatar"));
    QVERIFY(useLibravatar);
    QVERIFY(!useLibravatar->isChecked());

    QCheckBox *fallBackGravatar = w.findChild<QCheckBox *>(QStringLiteral("fallbackgravatar"));
    QVERIFY(fallBackGravatar);
    QVERIFY(!fallBackGravatar->isChecked());
}

void GravatarDownloadPixmapWidgetTest::shouldChangeButtonEnableState()
{
    Gravatar::GravatarDownloadPixmapWidget w;
    QLineEdit *lineEdit = w.findChild<QLineEdit *>(QStringLiteral("email"));

    QPushButton *getPixmapButton = w.findChild<QPushButton *>(QStringLiteral("searchbutton"));
    QVERIFY(!getPixmapButton->isEnabled());

    lineEdit->setText(QStringLiteral("foo"));
    QVERIFY(getPixmapButton->isEnabled());

    lineEdit->setText(QStringLiteral("   "));
    QVERIFY(!getPixmapButton->isEnabled());
}

QTEST_MAIN(GravatarDownloadPixmapWidgetTest)
