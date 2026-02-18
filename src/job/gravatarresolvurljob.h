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

/*!
 * \class Gravatar::GravatarResolvUrlJob
 * \inmodule Gravatar
 * \inheaderfile Gravatar/GravatarResolvUrlJob
 * \brief Avatar lookup job
 * \author Laurent Montel <montel@kde.org>
 */
class GRAVATAR_EXPORT GravatarResolvUrlJob : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a Gravatar resolve URL job.
     * \param parent the parent object
     */
    explicit GravatarResolvUrlJob(QObject *parent = nullptr);
    /*!
     * Destroys the Gravatar resolve URL job.
     */
    ~GravatarResolvUrlJob() override;

    /*!
     * Checks if the job can start.
     * \return true if the job can start, false otherwise
     */
    [[nodiscard]] bool canStart() const;
    /*!
     * Starts the job to resolve the Gravatar URL.
     */
    void start();

    /*!
     * Returns the email address to look up.
     * \return the email address
     */
    [[nodiscard]] QString email() const;
    /*!
     * Sets the email address to look up.
     * \param email the email address
     */
    void setEmail(const QString &email);

    /*!
     * Checks if a Gravatar exists for the email.
     * \return true if a Gravatar was found, false otherwise
     */
    [[nodiscard]] bool hasGravatar() const;

    /*!
     * Sets the size of the Gravatar pixmap.
     * \param size the size in pixels
     */
    void setSize(int size);
    /*!
     * Returns the size of the Gravatar pixmap.
     * \return the size in pixels
     */
    [[nodiscard]] int size() const;

    /*!
     * Returns the downloaded Gravatar pixmap.
     * \return the Gravatar pixmap
     */
    [[nodiscard]] QPixmap pixmap() const;

    /*!
     * Checks if a default pixmap should be used when no Gravatar is found.
     * \return true if default pixmap is used, false otherwise
     */
    [[nodiscard]] bool useDefaultPixmap() const;
    /*!
     * Sets whether to use a default pixmap when no Gravatar is found.
     * \param useDefaultPixmap whether to use a default pixmap
     */
    void setUseDefaultPixmap(bool useDefaultPixmap);

    /*!
     * Checks if Libravatar should be used as a fallback.
     * \return true if Libravatar is used, false otherwise
     */
    [[nodiscard]] bool useLibravatar() const;
    /*!
     * Sets whether to use Libravatar as a fallback.
     * \param useLibravatar whether to use Libravatar
     */
    void setUseLibravatar(bool useLibravatar);

    /*!
     * Checks if Gravatar should be used as a fallback after Libravatar.
     * \return true if Gravatar is used as fallback, false otherwise
     */
    [[nodiscard]] bool fallbackGravatar() const;
    /*!
     * Sets whether to use Gravatar as a fallback.
     * \param fallbackGravatar whether to use Gravatar as fallback
     */
    void setFallbackGravatar(bool fallbackGravatar);

Q_SIGNALS:
    /*!
     * Emitted when the job finishes resolving the Gravatar.
     * \param job the job that finished
     */
    void finished(Gravatar::GravatarResolvUrlJob *);
    /*!
     * Emitted when the Gravatar URL has been resolved.
     * \param url the resolved Gravatar URL
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
