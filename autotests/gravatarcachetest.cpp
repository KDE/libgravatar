/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "gravatarcachetest.h"
#include "../src/misc/gravatarcache.h"
#include "../src/misc/hash.h"

#include <QCryptographicHash>
#include <QPixmap>
#include <QStandardPaths>
#include <qtest.h>

using namespace Gravatar;

Q_DECLARE_METATYPE(Gravatar::Hash)

GravatarCacheTest::GravatarCacheTest(QObject *parent)
    : QObject(parent)
{
}

GravatarCacheTest::~GravatarCacheTest()
{
}

void GravatarCacheTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
}

void GravatarCacheTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarCache gravatarCache;
    QCOMPARE(gravatarCache.maximumSize(), 20);
}

void GravatarCacheTest::shouldChangeCacheValue()
{
    Gravatar::GravatarCache gravatarCache;
    int val = 30;
    gravatarCache.setMaximumSize(val);

    QCOMPARE(gravatarCache.maximumSize(), val);
    val = 50;
    gravatarCache.setMaximumSize(val);

    QCOMPARE(gravatarCache.maximumSize(), val);
}

void GravatarCacheTest::testLookup()
{
    Hash hash(QCryptographicHash::hash(QByteArray("test@example.com"), QCryptographicHash::Md5), Hash::Md5);
    {
        GravatarCache cache;
        cache.clearAllCache();
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(!found);
        QVERIFY(result.isNull());
    }

    QPixmap px(42, 42);
    px.fill(Qt::blue);

    {
        GravatarCache cache;
        cache.saveGravatarPixmap(hash, px);

        // in-memory cache lookup
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(found);
        QVERIFY(!result.isNull());
        QCOMPARE(result.size(), QSize(42, 42));
    }

    {
        // disk lookup
        GravatarCache cache;
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(found);
        QVERIFY(!result.isNull());
        QCOMPARE(result.size(), QSize(42, 42));
    }
}

void GravatarCacheTest::testMissing_data()
{
    QTest::addColumn<Hash>("hash");
    QTest::newRow("md5") << Hash(QCryptographicHash::hash(QByteArray("testMD5@example.com"), QCryptographicHash::Md5), Hash::Md5);
    QTest::newRow("Sha256") << Hash(QCryptographicHash::hash(QByteArray("testSHA256@example.com"), QCryptographicHash::Sha256), Hash::Sha256);
}

void GravatarCacheTest::testMissing()
{
    QFETCH(Hash, hash);
    {
        GravatarCache cache;
        cache.clearAllCache();
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(!found);
        QVERIFY(result.isNull());
    }

    {
        // store miss and verify in memory
        GravatarCache cache;
        cache.saveMissingGravatar(hash);
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(found);
        QVERIFY(result.isNull());
    }

    {
        // verify miss in disk storage
        GravatarCache cache;
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found);
        QVERIFY(found);
        QVERIFY(result.isNull());
    }
}

QTEST_MAIN(GravatarCacheTest)
