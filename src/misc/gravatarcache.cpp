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

#include "gravatarcache.h"
#include <QDir>
#include "gravatar_debug.h"

#include <QCache>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
using namespace Gravatar;

Q_GLOBAL_STATIC(GravatarCache, s_gravatarCache)

class Gravatar::GravatarCachePrivate
{
public:
    GravatarCachePrivate()
    {
    }

    QCache<QString, QPixmap> mCachePixmap;
    QString mGravatarPath;
};

GravatarCache::GravatarCache()
    : d(new Gravatar::GravatarCachePrivate)
{
    d->mCachePixmap.setMaxCost(20);
    //Make sure that this folder is created. Otherwise we can't store gravatar
    d->mGravatarPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/gravatar/");
    QDir().mkpath(d->mGravatarPath);
}

GravatarCache::~GravatarCache()
{
    delete d;
}

GravatarCache *GravatarCache::self()
{
    return s_gravatarCache;
}

void GravatarCache::saveGravatarPixmap(const QString &hashStr, const QPixmap &pixmap)
{
    if (!hashStr.isEmpty() && !pixmap.isNull()) {
        if (!d->mCachePixmap.contains(hashStr)) {
            const QString path = d->mGravatarPath + hashStr + QLatin1String(".png");
            qCDebug(GRAVATAR_LOG) << " path " << path;
            if (pixmap.save(path)) {
                qCDebug(GRAVATAR_LOG) << " saved in cache " << hashStr << path;
                d->mCachePixmap.insert(hashStr, new QPixmap(pixmap));
            }
        }
    }
}

QPixmap GravatarCache::loadGravatarPixmap(const QString &hashStr, bool &gravatarStored)
{
    gravatarStored = false;
    qCDebug(GRAVATAR_LOG) << " hashStr" << hashStr;
    if (!hashStr.isEmpty()) {
        if (d->mCachePixmap.contains(hashStr)) {
            qCDebug(GRAVATAR_LOG) << " contains in cache " << hashStr;
            gravatarStored = true;
            return *(d->mCachePixmap.object(hashStr));
        } else {
            const QString path = d->mGravatarPath + hashStr + QLatin1String(".png");
            if (QFileInfo::exists(path)) {
                QPixmap pix;
                if (pix.load(path)) {
                    qCDebug(GRAVATAR_LOG) << " add to cache " << hashStr << path;
                    d->mCachePixmap.insert(hashStr, new QPixmap(pix));
                    gravatarStored = true;
                    return pix;
                }
            } else {
                return QPixmap();
            }
        }
    }
    return QPixmap();
}

int GravatarCache::maximumSize() const
{
    return d->mCachePixmap.maxCost();
}

void GravatarCache::setMaximumSize(int maximumSize)
{
    if (d->mCachePixmap.maxCost() != maximumSize) {
        d->mCachePixmap.setMaxCost(maximumSize);
    }
}

void GravatarCache::clear()
{
    d->mCachePixmap.clear();
}

void GravatarCache::clearAllCache()
{
    const QString path = d->mGravatarPath;
    if (!path.isEmpty()) {
        QDir dir(path);
        if (dir.exists()) {
            const QFileInfoList list = dir.entryInfoList();  // get list of matching files and delete all
            for (const QFileInfo &it : list) {
                dir.remove(it.fileName());
            }
        }
    }
    clear();
}
