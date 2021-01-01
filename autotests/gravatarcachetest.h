/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARCACHETEST_H
#define GRAVATARCACHETEST_H

#include <QObject>

class GravatarCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarCacheTest(QObject *parent = nullptr);
    ~GravatarCacheTest();
private Q_SLOTS:
    void initTestCase();
    void shouldHaveDefaultValue();
    void shouldChangeCacheValue();
    void testLookup();
    void testMissing();
    void testMissing_data();
};

#endif // GRAVATARCACHETEST_H
