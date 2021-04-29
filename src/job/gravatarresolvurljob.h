/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QObject>
#include <QPixmap>
#include <QUrl>
class QNetworkReply;
class GravatarResolvUrlJobTest;
namespace Gravatar
{
class GravatarResolvUrlJobPrivate;
class Hash;

/** Avartar lookup job. */
class GRAVATAR_EXPORT GravatarResolvUrlJob : public QObject
{
    Q_OBJECT
public:
    explicit GravatarResolvUrlJob(QObject *parent = nullptr);
    ~GravatarResolvUrlJob();

    Q_REQUIRED_RESULT bool canStart() const;
    void start();

    Q_REQUIRED_RESULT QString email() const;
    void setEmail(const QString &email);

    Q_REQUIRED_RESULT bool hasGravatar() const;

    void setSize(int size);
    Q_REQUIRED_RESULT int size() const;

    Q_REQUIRED_RESULT QPixmap pixmap() const;

    Q_REQUIRED_RESULT bool useDefaultPixmap() const;
    void setUseDefaultPixmap(bool useDefaultPixmap);

    bool useLibravatar() const;
    void setUseLibravatar(bool useLibravatar);

    bool fallbackGravatar() const;
    void setFallbackGravatar(bool fallbackGravatar);

Q_SIGNALS:
    void finished(Gravatar::GravatarResolvUrlJob *);
    void resolvUrl(const QUrl &url);

private:
    friend class ::GravatarResolvUrlJobTest;

    void slotFinishLoadPixmap(QNetworkReply *reply);
    Q_REQUIRED_RESULT QUrl generateGravatarUrl(bool useLibravatar);
    Q_REQUIRED_RESULT Hash calculatedHash() const;
    void processNextBackend();
    void startNetworkManager(const QUrl &url);
    Q_REQUIRED_RESULT QUrl createUrl(bool useLibravatar);
    Q_REQUIRED_RESULT Hash calculateHash();
    Q_REQUIRED_RESULT bool cacheLookup(const Hash &hash);
    GravatarResolvUrlJobPrivate *const d;
};
}

