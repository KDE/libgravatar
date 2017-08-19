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

#include "gravatarresolvurljob.h"
#include "misc/gravatarcache.h"
#include "gravatar_debug.h"
#include <PimCommon/NetworkManager>

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QPixmap>
#include <QUrlQuery>
#include <QNetworkConfigurationManager>

using namespace Gravatar;

class Gravatar::GravatarResolvUrlJobPrivate
{
public:
    GravatarResolvUrlJobPrivate()
        : mNetworkAccessManager(nullptr)
        , mSize(80)
        , mHasGravatar(false)
        , mUseDefaultPixmap(false)
        , mUseLibravatar(false)
        , mFallbackGravatar(true)
        , mFallbackDone(false)
    {
    }

    QPixmap mPixmap;
    QString mEmail;
    QString mCalculatedHash;
    QNetworkAccessManager *mNetworkAccessManager;
    int mSize;
    bool mHasGravatar;
    bool mUseDefaultPixmap;
    bool mUseLibravatar;
    bool mFallbackGravatar;
    bool mFallbackDone;
};

GravatarResolvUrlJob::GravatarResolvUrlJob(QObject *parent)
    : QObject(parent)
    , d(new Gravatar::GravatarResolvUrlJobPrivate)
{
}

GravatarResolvUrlJob::~GravatarResolvUrlJob()
{
    delete d;
}

bool GravatarResolvUrlJob::canStart() const
{
    if (PimCommon::NetworkManager::self()->networkConfigureManager()->isOnline()) {
        return !d->mEmail.trimmed().isEmpty() && (d->mEmail.contains(QLatin1Char('@')));
    } else {
        return false;
    }
}

QUrl GravatarResolvUrlJob::generateGravatarUrl(bool useLibravatar)
{
    return createUrl(useLibravatar);
}

bool GravatarResolvUrlJob::hasGravatar() const
{
    return d->mHasGravatar;
}

void GravatarResolvUrlJob::startNetworkManager(const QUrl &url)
{
    if (PimCommon::NetworkManager::self()->networkConfigureManager()->isOnline()) {
        if (!d->mNetworkAccessManager) {
            d->mNetworkAccessManager = new QNetworkAccessManager(this);
            connect(d->mNetworkAccessManager, &QNetworkAccessManager::finished, this, &GravatarResolvUrlJob::slotFinishLoadPixmap);
        }
        QNetworkReply *reply = d->mNetworkAccessManager->get(QNetworkRequest(url));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    } else {
        qCDebug(GRAVATAR_LOG) << " network is not connected";
        deleteLater();
        return;
    }
}

void GravatarResolvUrlJob::start()
{
    d->mHasGravatar = false;
    d->mFallbackDone = false;
    if (canStart()) {
        d->mCalculatedHash.clear();
        const QUrl url = createUrl(d->mUseLibravatar);
        Q_EMIT resolvUrl(url);
        if (!cacheLookup(d->mCalculatedHash))
            startNetworkManager(url);
    } else {
        qCDebug(GRAVATAR_LOG) << "Gravatar can not start";
        deleteLater();
    }
}

void GravatarResolvUrlJob::slotFinishLoadPixmap(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        d->mPixmap.loadFromData(reply->readAll());
        d->mHasGravatar = true;
        //For the moment don't use cache other we will store a lot of pixmap
        if (!d->mUseDefaultPixmap) {
            GravatarCache::self()->saveGravatarPixmap(d->mCalculatedHash, d->mPixmap);
        }
    } else if (d->mUseLibravatar && d->mFallbackGravatar && !d->mFallbackDone) {
        d->mFallbackDone = true;
        d->mCalculatedHash.clear();
        const QUrl url = createUrl(false);
        Q_EMIT resolvUrl(url);
        if (!cacheLookup(d->mCalculatedHash))
            startNetworkManager(url);
        return;
    }
    reply->deleteLater();
    Q_EMIT finished(this);
    deleteLater();
}

void GravatarResolvUrlJob::slotError(QNetworkReply::NetworkError error)
{
    if (error == QNetworkReply::ContentNotFoundError) {
        d->mHasGravatar = false;
    }
}

QString GravatarResolvUrlJob::email() const
{
    return d->mEmail;
}

void GravatarResolvUrlJob::setEmail(const QString &email)
{
    d->mEmail = email;
}

QString GravatarResolvUrlJob::calculateHash(bool useLibravator)
{
    QCryptographicHash hash(useLibravator ? QCryptographicHash::Sha256 : QCryptographicHash::Md5);
    hash.addData(d->mEmail.toLower().toUtf8());
    return QString::fromUtf8(hash.result().toHex());
}

bool GravatarResolvUrlJob::fallbackGravatar() const
{
    return d->mFallbackGravatar;
}

void GravatarResolvUrlJob::setFallbackGravatar(bool fallbackGravatar)
{
    d->mFallbackGravatar = fallbackGravatar;
}

bool GravatarResolvUrlJob::useLibravatar() const
{
    return d->mUseLibravatar;
}

void GravatarResolvUrlJob::setUseLibravatar(bool useLibravatar)
{
    d->mUseLibravatar = useLibravatar;
}

bool GravatarResolvUrlJob::useDefaultPixmap() const
{
    return d->mUseDefaultPixmap;
}

void GravatarResolvUrlJob::setUseDefaultPixmap(bool useDefaultPixmap)
{
    d->mUseDefaultPixmap = useDefaultPixmap;
}

int GravatarResolvUrlJob::size() const
{
    return d->mSize;
}

QPixmap GravatarResolvUrlJob::pixmap() const
{
    return d->mPixmap;
}

void GravatarResolvUrlJob::setSize(int size)
{
    if (size <= 0) {
        size = 80;
    } else if (size > 2048) {
        size = 2048;
    }
    d->mSize = size;
}

QString GravatarResolvUrlJob::calculatedHash() const
{
    return d->mCalculatedHash;
}

QUrl GravatarResolvUrlJob::createUrl(bool useLibravatar)
{
    QUrl url;
    d->mCalculatedHash.clear();
    if (!canStart()) {
        return url;
    }
    QUrlQuery query;
    if (!d->mUseDefaultPixmap) {
        //Add ?d=404
        query.addQueryItem(QStringLiteral("d"), QStringLiteral("404"));
    }
    if (d->mSize != 80) {
        query.addQueryItem(QStringLiteral("s"), QString::number(d->mSize));
    }
    url.setScheme(QStringLiteral("https"));
    if (useLibravatar) {
        url.setHost(QStringLiteral("seccdn.libravatar.org"));
    } else {
        url.setHost(QStringLiteral("secure.gravatar.com"));
    }
    url.setPort(443);
    d->mCalculatedHash = calculateHash(useLibravatar);
    url.setPath(QLatin1String("/avatar/") + d->mCalculatedHash);
    url.setQuery(query);
    return url;
}

bool GravatarResolvUrlJob::cacheLookup(const QString &hash)
{
    bool haveStoredPixmap = false;
    const QPixmap pix = GravatarCache::self()->loadGravatarPixmap(hash, haveStoredPixmap);
    if (haveStoredPixmap && !pix.isNull()) {
        d->mPixmap = pix;
        d->mHasGravatar = true;
        Q_EMIT finished(this);
        deleteLater();
        return true;
    }
    return false;
}
