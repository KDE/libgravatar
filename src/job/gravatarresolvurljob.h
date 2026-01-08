/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QObject>
#include <QPixmap>
#include <QUrl>

#include <memory>

class QNetworkReply;
class GravatarResolvUrlJobTest;
namespace Gravatar
{
class GravatarResolvUrlJobPrivate;
class Hash;

/*! Avartar lookup job. */
class GRAVATAR_EXPORT GravatarResolvUrlJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief GravatarResolvUrlJob
     * \param parent
     */
    explicit GravatarResolvUrlJob(QObject *parent = nullptr);
    ~GravatarResolvUrlJob() override;

    /*!
     */
    [[nodiscard]] bool canStart() const;
    /*!
     */
    void start();

    /*!
     */
    [[nodiscard]] QString email() const;
    /*!
     */
    void setEmail(const QString &email);

    /*!
     */
    [[nodiscard]] bool hasGravatar() const;

    /*!
     */
    void setSize(int size);
    /*!
     */
    [[nodiscard]] int size() const;

    /*!
     */
    [[nodiscard]] QPixmap pixmap() const;

    /*!
     */
    [[nodiscard]] bool useDefaultPixmap() const;
    /*!
     */
    void setUseDefaultPixmap(bool useDefaultPixmap);

    /*!
     */
    [[nodiscard]] bool useLibravatar() const;
    /*!
     */
    void setUseLibravatar(bool useLibravatar);

    /*!
     */
    [[nodiscard]] bool fallbackGravatar() const;
    /*!
     */
    void setFallbackGravatar(bool fallbackGravatar);

Q_SIGNALS:
    /*!
     */
    void finished(Gravatar::GravatarResolvUrlJob *);
    /*!
     */
    void resolvUrl(const QUrl &url);

private:
    friend class ::GravatarResolvUrlJobTest;

    void slotFinishLoadPixmap(QNetworkReply *reply);
    [[nodiscard]] QUrl generateGravatarUrl(bool useLibravatar);
    [[nodiscard]] Hash calculatedHash() const;
    GRAVATAR_NO_EXPORT void processNextBackend();
    GRAVATAR_NO_EXPORT void startNetworkManager(const QUrl &url);
    [[nodiscard]] GRAVATAR_NO_EXPORT QUrl createUrl(bool useLibravatar);
    [[nodiscard]] GRAVATAR_NO_EXPORT Hash calculateHash();
    [[nodiscard]] GRAVATAR_NO_EXPORT bool cacheLookup(const Hash &hash);

    std::unique_ptr<GravatarResolvUrlJobPrivate> const d;
};
}
