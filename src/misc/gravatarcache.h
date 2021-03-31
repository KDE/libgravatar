/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"

#include <QPixmap>

namespace Gravatar
{
class GravatarCachePrivate;
class Hash;

/** Cache for both positive and negative avatar lookups. */
class GRAVATAR_EXPORT GravatarCache
{
public:
    static GravatarCache *self();

    GravatarCache();
    ~GravatarCache();

    void saveGravatarPixmap(const Hash &hash, const QPixmap &pixmap);
    void saveMissingGravatar(const Hash &hash);

    Q_REQUIRED_RESULT QPixmap loadGravatarPixmap(const Hash &hash, bool &gravatarStored);

    Q_REQUIRED_RESULT int maximumSize() const;
    void setMaximumSize(int maximumSize);

    void clear();
    void clearAllCache();

private:
    Q_DISABLE_COPY(GravatarCache)
    GravatarCachePrivate *const d;
};
}

