/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARDOWNLOADPIXMAPWIDGETTEST_H
#define GRAVATARDOWNLOADPIXMAPWIDGETTEST_H

#include <QObject>

class GravatarDownloadPixmapWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarDownloadPixmapWidgetTest(QObject *parent = nullptr);
    ~GravatarDownloadPixmapWidgetTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeButtonEnableState();
};

#endif // GRAVATARDOWNLOADPIXMAPWIDGETTEST_H
