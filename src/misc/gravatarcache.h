/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"

#include <QPixmap>

#include <memory>

namespace Gravatar
{
class GravatarCachePrivate;
class Hash;

/*!
 * \class Gravatar::GravatarCache
 * \inmodule Gravatar
 * \inheaderfile Gravatar/GravatarCache
 * \brief Cache for both positive and negative avatar lookups
 * \author Laurent Montel <montel@kde.org>
 */
class GRAVATAR_EXPORT GravatarCache
{
public:
    /*!
     * Returns the singleton instance of GravatarCache.
     * \return the GravatarCache singleton instance
     */
    static GravatarCache *self();

    /*!
     * Constructs a Gravatar cache.
     */
    GravatarCache();
    /*!
     * Destroys the Gravatar cache.
     */
    ~GravatarCache();

    /*!
     * Saves a Gravatar pixmap to the cache.
     * \param hash the hash associated with the pixmap
     * \param pixmap the pixmap to save
     */
    void saveGravatarPixmap(const Hash &hash, const QPixmap &pixmap);
    /*!
     * Records a missing Gravatar for the given hash.
     * \param hash the hash for which no Gravatar was found
     */
    void saveMissingGravatar(const Hash &hash);

    /*!
     * Loads a Gravatar pixmap from the cache.
     * \param hash the hash to look up
     * \param gravatarStored whether the Gravatar was found in cache
     * \return the cached Gravatar pixmap if found, otherwise a null pixmap
     */
    [[nodiscard]] QPixmap loadGravatarPixmap(const Hash &hash, bool &gravatarStored);

    /*!
     * Returns the maximum cache size in bytes.
     * \return the maximum cache size
     */
    [[nodiscard]] int maximumSize() const;
    /*!
     * Sets the maximum cache size in bytes.
     * \param maximumSize the maximum size in bytes
     */
    void setMaximumSize(int maximumSize);

    /*!
     * Clears the current cache.
     */
    void clear();
    /*!
     * Clears all cached Gravatar data.
     */
    void clearAllCache();

private:
    Q_DISABLE_COPY(GravatarCache)
    std::unique_ptr<GravatarCachePrivate> const d;
};
}
