/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarresolvurljobtest.h"
#include "../src/job/gravatarresolvurljob.h"
#include "../src/misc/hash.h"

#include <QNetworkInformation>
#include <QTest>

GravatarResolvUrlJobTest::GravatarResolvUrlJobTest(QObject *parent)
    : QObject(parent)
{
    QNetworkInformation::loadDefaultBackend();
}

GravatarResolvUrlJobTest::~GravatarResolvUrlJobTest() = default;

bool GravatarResolvUrlJobTest::isOnline() const
{
    return QNetworkInformation::instance()->reachability() == QNetworkInformation::Reachability::Online
        && !QNetworkInformation::instance()->isBehindCaptivePortal();
}

void GravatarResolvUrlJobTest::shouldHaveDefaultValue()
{
    Gravatar::GravatarResolvUrlJob job;
    QVERIFY(job.email().isEmpty());
    QCOMPARE(job.size(), 80);
    QCOMPARE(job.hasGravatar(), false);
    QCOMPARE(job.pixmap().isNull(), true);
    QCOMPARE(job.useDefaultPixmap(), false);
    QCOMPARE(job.useLibravatar(), false);
    QCOMPARE(job.fallbackGravatar(), true);
}

void GravatarResolvUrlJobTest::shouldChangeValue()
{
    Gravatar::GravatarResolvUrlJob job;
    bool useDefaultPixmap = true;
    job.setUseDefaultPixmap(useDefaultPixmap);
    QCOMPARE(job.useDefaultPixmap(), useDefaultPixmap);

    useDefaultPixmap = false;
    job.setUseDefaultPixmap(useDefaultPixmap);
    QCOMPARE(job.useDefaultPixmap(), useDefaultPixmap);

    bool useLibravatar = true;
    job.setUseLibravatar(useLibravatar);
    QCOMPARE(job.useLibravatar(), useLibravatar);

    useLibravatar = false;
    job.setUseLibravatar(useLibravatar);
    QCOMPARE(job.useLibravatar(), useLibravatar);

    bool fallBackGravatar = false;
    job.setFallbackGravatar(fallBackGravatar);
    QCOMPARE(job.fallbackGravatar(), fallBackGravatar);
    fallBackGravatar = true;
    job.setFallbackGravatar(fallBackGravatar);
    QCOMPARE(job.fallbackGravatar(), fallBackGravatar);
}

void GravatarResolvUrlJobTest::shouldChangeSize()
{
    Gravatar::GravatarResolvUrlJob job;
    int size = 50;
    job.setSize(size);
    QCOMPARE(job.size(), size);
    size = 0;
    job.setSize(size);
    QCOMPARE(job.size(), 80);

    size = 10;
    job.setSize(size);
    QCOMPARE(job.size(), size);

    size = 2048;
    job.setSize(size);
    QCOMPARE(job.size(), size);

    size = 4096;
    job.setSize(size);
    QCOMPARE(job.size(), 2048);
}

void GravatarResolvUrlJobTest::shouldAddSizeInUrl()
{
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(QStringLiteral("foo@kde.org"));
    job.setSize(1024);
    job.setUseLibravatar(false);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, isOnline() ? QUrl(QStringLiteral("https://secure.gravatar.com:/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")) : QUrl());
    job.setUseLibravatar(true);
    url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, isOnline() ? QUrl(QStringLiteral("https://seccdn.libravatar.org:/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")) : QUrl());
}

void GravatarResolvUrlJobTest::shouldUseDefaultPixmap()
{
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(QStringLiteral("foo@kde.org"));
    job.setSize(1024);
    job.setUseDefaultPixmap(true);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, isOnline() ? QUrl(QStringLiteral("https://secure.gravatar.com:/avatar/89b4e14cf2fc6d426275c019c6dc9de6?s=1024")) : QUrl());
}

void GravatarResolvUrlJobTest::shouldUseHttps()
{
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(QStringLiteral("foo@kde.org"));
    job.setSize(1024);
    job.setUseLibravatar(false);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, isOnline() ? QUrl(QStringLiteral("https://secure.gravatar.com:/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")) : QUrl());
    job.setUseLibravatar(true);
    url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, isOnline() ? QUrl(QStringLiteral("https://seccdn.libravatar.org/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")) : QUrl());
}

void GravatarResolvUrlJobTest::shouldNotStart()
{
    Gravatar::GravatarResolvUrlJob job;
    QVERIFY(!job.canStart());

    job.setEmail(QStringLiteral("foo"));
    QVERIFY(!job.canStart());

    job.setEmail(QStringLiteral(" "));
    QVERIFY(!job.canStart());

    job.setEmail(QStringLiteral("foo@kde.org"));
    QCOMPARE(job.canStart(), isOnline());
}

void GravatarResolvUrlJobTest::shouldGenerateGravatarUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("calculedhash");
    QTest::addColumn<QUrl>("output");
    QTest::addColumn<bool>("uselibravatar");
    QTest::newRow("empty") << QString() << QString() << QUrl() << false;
    QTest::newRow("no domain") << QStringLiteral("foo") << QString() << QUrl() << false;
    QTest::newRow("validemail") << QStringLiteral("foo@kde.org") << QStringLiteral("89b4e14cf2fc6d426275c019c6dc9de6")
                                << QUrl(QStringLiteral("https://secure.gravatar.com:/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404")) << false;
    QTest::newRow("validemaillibravatar") << QStringLiteral("foo@kde.org") << QStringLiteral("89b4e14cf2fc6d426275c019c6dc9de6")
                                          << QUrl(QStringLiteral("https://seccdn.libravatar.org/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404")) << true;
}

void GravatarResolvUrlJobTest::shouldGenerateGravatarUrl()
{
    QFETCH(QString, input);
    QFETCH(QString, calculedhash);
    QFETCH(QUrl, output);
    QFETCH(bool, uselibravatar);
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(input);
    job.setUseLibravatar(uselibravatar);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    if (isOnline()) {
        QCOMPARE(job.calculatedHash().hexString(), calculedhash);
        QCOMPARE(url, output);
    } else {
        QCOMPARE(job.calculatedHash().hexString(), QString());
        QVERIFY(url.isEmpty());
    }
}

QTEST_MAIN(GravatarResolvUrlJobTest)

#include "moc_gravatarresolvurljobtest.cpp"
