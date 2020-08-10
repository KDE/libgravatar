/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARDOWNLOADPIXMAPDIALOGTEST_H
#define GRAVATARDOWNLOADPIXMAPDIALOGTEST_H

#include <QObject>

class GravatarDownloadPixmapDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarDownloadPixmapDialogTest(QObject *parent = nullptr);
    ~GravatarDownloadPixmapDialogTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void initTestCase();
};

#endif // GRAVATARDOWNLOADPIXMAPDIALOGTEST_H
