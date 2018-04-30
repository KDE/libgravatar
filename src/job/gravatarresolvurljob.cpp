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

#include "gravatarresolvurljob.h"
#include "misc/gravatarcache.h"
#include "misc/hash.h"
#include "gravatar_debug.h"
#include <PimCommon/NetworkManager>

#include <QCryptographicHash>
#include <QPixmap>
#include <QUrlQuery>
#include <QNetworkConfigurationManager>

using namespace Gravatar;

class Q_DECL_HIDDEN Gravatar::GravatarResolvUrlJobPrivate
{
public:
    GravatarResolvUrlJobPrivate()
    {
    }

    QPixmap mPixmap;
    QString mEmail;
    Hash mCalculatedHash;
    QNetworkAccessManager *mNetworkAccessManager = nullptr;
    int mSize = 80;

    enum Backend {
        None = 0x0,
        Gravatar = 0x2
    };
    int mBackends = Gravatar;

    bool mHasGravatar = false;
    bool mUseDefaultPixmap = false;
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

QUrl GravatarResolvUrlJob::generateGravatarUrl()
{
    return createUrl();
}

bool GravatarResolvUrlJob::hasGravatar() const
{
    return d->mHasGravatar;
}

void GravatarResolvUrlJob::startNetworkManager(const QUrl &url)
{
    if (!d->mNetworkAccessManager) {
        d->mNetworkAccessManager = new QNetworkAccessManager(this);
        connect(d->mNetworkAccessManager, &QNetworkAccessManager::finished, this, &GravatarResolvUrlJob::slotFinishLoadPixmap);
    }
    d->mNetworkAccessManager->get(QNetworkRequest(url));
}

void GravatarResolvUrlJob::start()
{
    if (d->mBackends == GravatarResolvUrlJobPrivate::None) {
        d->mBackends = GravatarResolvUrlJobPrivate::Gravatar; // default is Gravatar if nothing else is selected
    }

    d->mHasGravatar = false;
    if (canStart()) {
        processNextBackend();
    } else {
        qCDebug(GRAVATAR_LOG) << "Gravatar can not start";
        deleteLater();
    }
}

void GravatarResolvUrlJob::processNextBackend()
{
    if (d->mHasGravatar || d->mBackends == GravatarResolvUrlJobPrivate::None) {
        Q_EMIT finished(this);
        deleteLater();
        return;
    }

    QUrl url;
    if (d->mBackends & GravatarResolvUrlJobPrivate::Gravatar) {
        d->mBackends &= ~GravatarResolvUrlJobPrivate::Gravatar;
        url = createUrl();
    }

    Q_EMIT resolvUrl(url);
    if (!cacheLookup(d->mCalculatedHash)) {
        startNetworkManager(url);
    } else {
        processNextBackend();
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
    } else {
        if (reply->error() == QNetworkReply::ContentNotFoundError) {
            GravatarCache::self()->saveMissingGravatar(d->mCalculatedHash);
        } else {
            qCDebug(GRAVATAR_LOG) << "Network error:" << reply->request().url() << reply->errorString();
        }
    }

    processNextBackend();
}

QString GravatarResolvUrlJob::email() const
{
    return d->mEmail;
}

void GravatarResolvUrlJob::setEmail(const QString &email)
{
    d->mEmail = email;
}

Hash GravatarResolvUrlJob::calculateHash()
{
    const auto email = d->mEmail.toLower().toUtf8();
    return Hash(QCryptographicHash::hash(email, QCryptographicHash::Md5), Hash::Md5);
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

Hash GravatarResolvUrlJob::calculatedHash() const
{
    return d->mCalculatedHash;
}

QUrl GravatarResolvUrlJob::createUrl()
{
    QUrl url;
    d->mCalculatedHash = Hash();
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
    url.setHost(QStringLiteral("secure.gravatar.com"));
    url.setPort(443);
    d->mCalculatedHash = calculateHash();
    url.setPath(QLatin1String("/avatar/") + d->mCalculatedHash.hexString());
    url.setQuery(query);
    return url;
}

bool GravatarResolvUrlJob::cacheLookup(const Hash &hash)
{
    bool haveStoredPixmap = false;
    const QPixmap pix = GravatarCache::self()->loadGravatarPixmap(hash, haveStoredPixmap);
    if (haveStoredPixmap && !pix.isNull()) { // we know a Gravatar for this hash
        d->mPixmap = pix;
        d->mHasGravatar = true;
        Q_EMIT finished(this);
        deleteLater();
        return true;
    }
    return haveStoredPixmap;
}
