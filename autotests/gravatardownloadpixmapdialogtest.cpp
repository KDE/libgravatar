/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

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

#include "gravatardownloadpixmapdialogtest.h"
#include "../src/widgets/gravatardownloadpixmapdialog.h"
#include <qtest.h>

#include <QDialogButtonBox>
#include <QStandardPaths>
#include "../src/widgets/gravatardownloadpixmapwidget.h"

GravatarDownloadPixmapDialogTest::GravatarDownloadPixmapDialogTest(QObject *parent)
    : QObject(parent)
{
}

GravatarDownloadPixmapDialogTest::~GravatarDownloadPixmapDialogTest()
{
}

void GravatarDownloadPixmapDialogTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void GravatarDownloadPixmapDialogTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarDownloadPixmapDialog dlg;
    Gravatar::GravatarDownloadPixmapWidget *widget = dlg.findChild<Gravatar::GravatarDownloadPixmapWidget *>(QStringLiteral("gravatarwidget"));
    QVERIFY(widget);

    QDialogButtonBox *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);
}

QTEST_MAIN(GravatarDownloadPixmapDialogTest)
