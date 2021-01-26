/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarcache.h"
#include "gravatar_debug.h"
#include "hash.h"

#include <QCache>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <QStandardPaths>

#include <algorithm>
#include <vector>

using namespace Gravatar;

Q_GLOBAL_STATIC(GravatarCache, s_gravatarCache)

class Q_DECL_HIDDEN Gravatar::GravatarCachePrivate
{
public:
    template<typename T> inline void insertMissingHash(std::vector<T> &vec, const T &hash)
    {
        auto it = std::lower_bound(vec.begin(), vec.end(), hash);
        if (it != vec.end() && *it == hash) {
            return; // already present (shouldn't happen)
        }
        vec.insert(it, hash);
    }

    template<typename T> inline void saveVector(const std::vector<T> &vec, const QString &fileName)
    {
        QSaveFile f(mGravatarPath + fileName);
        if (!f.open(QFile::WriteOnly)) {
            qCWarning(GRAVATAR_LOG) << "Can't write missing hashes cache file:" << f.fileName() << f.errorString();
            return;
        }

        f.resize(vec.size() * sizeof(T));
        f.write(reinterpret_cast<const char *>(vec.data()), vec.size() * sizeof(T));
        f.commit();
    }

    template<typename T> inline void loadVector(std::vector<T> &vec, const QString &fileName)
    {
        if (!vec.empty()) { // already loaded
            return;
        }

        QFile f(mGravatarPath + fileName);
        if (!f.open(QFile::ReadOnly)) {
            return; // does not exist yet
        }
        if (f.size() % sizeof(T) != 0) {
            qCWarning(GRAVATAR_LOG) << "Missing hash cache is corrupt:" << f.fileName();
            return;
        }
        vec.resize(f.size() / sizeof(T));
        f.read(reinterpret_cast<char *>(vec.data()), f.size());
    }

    QCache<Hash, QPixmap> mCachePixmap;
    QString mGravatarPath;
    std::vector<Hash128> mMd5Misses;
    std::vector<Hash256> mSha256Misses;
};

GravatarCache::GravatarCache()
    : d(new Gravatar::GravatarCachePrivate)
{
    d->mCachePixmap.setMaxCost(20);
    // Make sure that this folder is created. Otherwise we can't store gravatar
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

void GravatarCache::saveGravatarPixmap(const Hash &hash, const QPixmap &pixmap)
{
    if (!hash.isValid() || pixmap.isNull()) {
        return;
    }

    const QString path = d->mGravatarPath + hash.hexString() + QLatin1String(".png");
    qCDebug(GRAVATAR_LOG) << " path " << path;
    if (pixmap.save(path)) {
        qCDebug(GRAVATAR_LOG) << " saved in cache " << path;
        d->mCachePixmap.insert(hash, new QPixmap(pixmap));
    }
}

void GravatarCache::saveMissingGravatar(const Hash &hash)
{
    switch (hash.type()) {
    case Hash::Invalid:
        break;
    case Hash::Md5:
        d->insertMissingHash(d->mMd5Misses, hash.md5());
        d->saveVector(d->mMd5Misses, QStringLiteral("missing.md5"));
        break;
    case Hash::Sha256:
        d->insertMissingHash(d->mSha256Misses, hash.sha256());
        d->saveVector(d->mSha256Misses, QStringLiteral("missing.sha256"));
        break;
    }
}

QPixmap GravatarCache::loadGravatarPixmap(const Hash &hash, bool &gravatarStored)
{
    gravatarStored = false;
    // qCDebug(GRAVATAR_LOG) << " hashStr" << hash.hexString();
    if (!hash.isValid()) {
        return QPixmap();
    }

    // in-memory cache
    if (d->mCachePixmap.contains(hash)) {
        qCDebug(GRAVATAR_LOG) << " contains in cache " << hash.hexString();
        gravatarStored = true;
        return *(d->mCachePixmap.object(hash));
    }

    // file-system cache
    const QString path = d->mGravatarPath + hash.hexString() + QLatin1String(".png");
    if (QFileInfo::exists(path)) {
        QPixmap pix;
        if (pix.load(path)) {
            qCDebug(GRAVATAR_LOG) << " add to cache " << hash.hexString() << path;
            d->mCachePixmap.insert(hash, new QPixmap(pix));
            gravatarStored = true;
            return pix;
        }
    }

    // missing gravatar cache (ie. known to not exist one)
    switch (hash.type()) {
    case Hash::Invalid:
        break;
    case Hash::Md5:
        d->loadVector(d->mMd5Misses, QStringLiteral("missing.md5"));
        gravatarStored = std::binary_search(d->mMd5Misses.begin(), d->mMd5Misses.end(), hash.md5());
        break;
    case Hash::Sha256:
        d->loadVector(d->mSha256Misses, QStringLiteral("missing.sha256"));
        gravatarStored = std::binary_search(d->mSha256Misses.begin(), d->mSha256Misses.end(), hash.sha256());
        break;
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
            const QFileInfoList list = dir.entryInfoList(); // get list of matching files and delete all
            for (const QFileInfo &it : list) {
                dir.remove(it.fileName());
            }
        }
    }
    clear();
    d->mMd5Misses.clear();
    d->mSha256Misses.clear();
}
