/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarcachetest.h"
#include "../src/misc/gravatarcache.h"
#include "../src/misc/hash.h"

#include <QCryptographicHash>
#include <QPixmap>
#include <QStandardPaths>
#include <QTest>

using namespace Gravatar;

Q_DECLARE_METATYPE(Gravatar::Hash)

GravatarCacheTest::GravatarCacheTest(QObject *parent)
    : QObject(parent)
{
}

GravatarCacheTest::~GravatarCacheTest() = default;

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
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(!found);
        QVERIFY(result.isNull());
    }

    QPixmap px(42, 42);
    px.fill(Qt::blue);

    {
        GravatarCache cache;
        cache.saveGravatarPixmap(hash, px, 80);

        // in-memory cache lookup
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QVERIFY(!result.isNull());
        QCOMPARE(result.size(), QSize(42, 42));
    }

    {
        // disk lookup
        GravatarCache cache;
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
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
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(!found);
        QVERIFY(result.isNull());
    }

    {
        // store miss and verify in memory
        GravatarCache cache;
        cache.saveMissingGravatar(hash);
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QVERIFY(result.isNull());
    }

    {
        // verify miss in disk storage
        GravatarCache cache;
        bool found = false;
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QVERIFY(result.isNull());

        // a miss does not depend on the requested size
        found = false;
        const auto otherSize = cache.loadGravatarPixmap(hash, found, 256);
        QVERIFY(found);
        QVERIFY(otherSize.isNull());
    }
}

void GravatarCacheTest::testLookupUsesSize()
{
    const Hash hash(QCryptographicHash::hash(QByteArray("testsize@example.com"), QCryptographicHash::Md5), Hash::Md5);

    QPixmap small(80, 80);
    small.fill(Qt::blue);
    QPixmap big(256, 256);
    big.fill(Qt::red);

    {
        GravatarCache cache;
        cache.clearAllCache();
        cache.saveGravatarPixmap(hash, small, 80);

        // another size must not be served from the in-memory cache
        bool found = true;
        const auto other = cache.loadGravatarPixmap(hash, found, 256);
        QVERIFY(!found);
        QVERIFY(other.isNull());

        // the stored size still hits
        found = false;
        const auto result = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QCOMPARE(result.size(), QSize(80, 80));
    }

    {
        // ... nor from the on-disk cache
        GravatarCache cache;
        bool found = true;
        const auto other = cache.loadGravatarPixmap(hash, found, 256);
        QVERIFY(!found);
        QVERIFY(other.isNull());
    }

    {
        // both sizes coexist, each lookup returns its own pixmap
        GravatarCache cache;
        cache.saveGravatarPixmap(hash, big, 256);

        bool found = false;
        const auto large = cache.loadGravatarPixmap(hash, found, 256);
        QVERIFY(found);
        QCOMPARE(large.size(), QSize(256, 256));

        found = false;
        const auto tiny = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QCOMPARE(tiny.size(), QSize(80, 80));
    }

    {
        // same, from disk only
        GravatarCache cache;
        bool found = false;
        const auto large = cache.loadGravatarPixmap(hash, found, 256);
        QVERIFY(found);
        QCOMPARE(large.size(), QSize(256, 256));

        found = false;
        const auto tiny = cache.loadGravatarPixmap(hash, found, 80);
        QVERIFY(found);
        QCOMPARE(tiny.size(), QSize(80, 80));
    }
}

QTEST_MAIN(GravatarCacheTest)

#include "moc_gravatarcachetest.cpp"
