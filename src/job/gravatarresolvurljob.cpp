/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarresolvurljob.h"
#include "gravatar_debug.h"
#include "misc/gravatarcache.h"
#include "misc/hash.h"
#include <PimCommon/NetworkManager>

#include <QCryptographicHash>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QUrlQuery>

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

    enum Backend { None = 0x0, Libravatar = 0x1, Gravatar = 0x2 };
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
        // qCDebug(GRAVATAR_LOG) << "email " << d->mEmail;
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
    if (!d->mNetworkAccessManager) {
        d->mNetworkAccessManager = new QNetworkAccessManager(this);
        d->mNetworkAccessManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
        d->mNetworkAccessManager->setStrictTransportSecurityEnabled(true);
        d->mNetworkAccessManager->enableStrictTransportSecurityStore(true);
        connect(d->mNetworkAccessManager, &QNetworkAccessManager::finished, this, &GravatarResolvUrlJob::slotFinishLoadPixmap);
    }

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    req.setAttribute(QNetworkRequest::Http2AllowedAttribute, true);
    d->mNetworkAccessManager->get(req);
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
    if (d->mBackends & GravatarResolvUrlJobPrivate::Libravatar) {
        d->mBackends &= ~GravatarResolvUrlJobPrivate::Libravatar;
        url = createUrl(true);
    } else if (d->mBackends & GravatarResolvUrlJobPrivate::Gravatar) {
        d->mBackends &= ~GravatarResolvUrlJobPrivate::Gravatar;
        url = createUrl(false);
    }

    // qDebug() << " url " << url;
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
        const QByteArray data = reply->readAll();
        d->mPixmap.loadFromData(data);
        d->mHasGravatar = true;
        // For the moment don't use cache other we will store a lot of pixmap
        if (!d->mUseDefaultPixmap) {
            GravatarCache::self()->saveGravatarPixmap(d->mCalculatedHash, d->mPixmap);
        }
    } else {
        if (reply->error() != QNetworkReply::ContentNotFoundError) {
            GravatarCache::self()->saveMissingGravatar(d->mCalculatedHash);
        } else {
            qCDebug(GRAVATAR_LOG) << "Network error:" << reply->request().url() << reply->errorString();
        }
    }
    reply->deleteLater();

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

bool GravatarResolvUrlJob::fallbackGravatar() const
{
    return d->mBackends & GravatarResolvUrlJobPrivate::Gravatar;
}

void GravatarResolvUrlJob::setFallbackGravatar(bool fallbackGravatar)
{
    if (fallbackGravatar)
        d->mBackends |= GravatarResolvUrlJobPrivate::Gravatar;
    else
        d->mBackends &= ~GravatarResolvUrlJobPrivate::Gravatar;
}

bool GravatarResolvUrlJob::useLibravatar() const
{
    return d->mBackends & GravatarResolvUrlJobPrivate::Libravatar;
}

void GravatarResolvUrlJob::setUseLibravatar(bool useLibravatar)
{
    if (useLibravatar)
        d->mBackends |= GravatarResolvUrlJobPrivate::Libravatar;
    else
        d->mBackends &= ~GravatarResolvUrlJobPrivate::Libravatar;
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

QUrl GravatarResolvUrlJob::createUrl(bool useLibravatar)
{
    QUrl url;
    d->mCalculatedHash = Hash();
    if (!canStart()) {
        return url;
    }
    QUrlQuery query;
    if (!d->mUseDefaultPixmap) {
        // Add ?d=404
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
