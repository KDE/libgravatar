/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatardownloadpixmapdialogtest.h"
#include "../src/widgets/gravatardownloadpixmapdialog.h"
#include <QTest>

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
    auto *widget = dlg.findChild<Gravatar::GravatarDownloadPixmapWidget *>(QStringLiteral("gravatarwidget"));
    QVERIFY(widget);

    auto *buttonBox = dlg.findChild<QDialogButtonBox *>(QStringLiteral("buttonbox"));
    QVERIFY(buttonBox);
}

QTEST_MAIN(GravatarDownloadPixmapDialogTest)
