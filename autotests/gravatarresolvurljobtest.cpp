/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "gravatarresolvurljobtest.h"
#include "../src/job/gravatarresolvurljob.h"
#include "../src/misc/hash.h"
#include <QTest>

GravatarResolvUrlJobTest::GravatarResolvUrlJobTest(QObject *parent)
    : QObject(parent)
{
}

GravatarResolvUrlJobTest::~GravatarResolvUrlJobTest()
{
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
    QCOMPARE(url, QUrl(QStringLiteral("https://secure.gravatar.com:443/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")));
    job.setUseLibravatar(true);
    url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, QUrl(QStringLiteral("https://seccdn.libravatar.org:443/avatar/2726400c3a33ce56c0ff632cbc0474f766d3b36e68819c601fb02954c1681d85?d=404&s=1024")));
}

void GravatarResolvUrlJobTest::shouldUseDefaultPixmap()
{
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(QStringLiteral("foo@kde.org"));
    job.setSize(1024);
    job.setUseDefaultPixmap(true);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, QUrl(QStringLiteral("https://secure.gravatar.com:443/avatar/89b4e14cf2fc6d426275c019c6dc9de6?s=1024")));
}

void GravatarResolvUrlJobTest::shouldUseHttps()
{
    Gravatar::GravatarResolvUrlJob job;
    job.setEmail(QStringLiteral("foo@kde.org"));
    job.setSize(1024);
    job.setUseLibravatar(false);
    QUrl url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, QUrl(QStringLiteral("https://secure.gravatar.com:443/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404&s=1024")));
    job.setUseLibravatar(true);
    url = job.generateGravatarUrl(job.useLibravatar());
    QCOMPARE(url, QUrl(QStringLiteral("https://seccdn.libravatar.org:443/avatar/2726400c3a33ce56c0ff632cbc0474f766d3b36e68819c601fb02954c1681d85?d=404&s=1024")));
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
    QVERIFY(job.canStart());
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
                                << QUrl(QStringLiteral("https://secure.gravatar.com:443/avatar/89b4e14cf2fc6d426275c019c6dc9de6?d=404")) << false;
    QTest::newRow("validemaillibravatar") << QStringLiteral("foo@kde.org") << QStringLiteral("2726400c3a33ce56c0ff632cbc0474f766d3b36e68819c601fb02954c1681d85")
                                          << QUrl(QStringLiteral("https://seccdn.libravatar.org:443/avatar/2726400c3a33ce56c0ff632cbc0474f766d3b36e68819c601fb02954c1681d85?d=404")) << true;
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
    QCOMPARE(calculedhash, job.calculatedHash().hexString());
    QCOMPARE(url, output);
}

QTEST_MAIN(GravatarResolvUrlJobTest)
