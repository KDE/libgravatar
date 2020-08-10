/*
   SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "hash.h"

#include <QHash>
#include <QString>

using namespace Gravatar;

Hash::Hash()
    : m_type(Invalid)
{
}

Hash::Hash(const QByteArray &data, Type type)
    : m_type(type)
{
    switch (type) {
    case Invalid:
        break;
    case Md5:
        Q_ASSERT(sizeof(Hash128) == data.size());
        m_hash.md5 = *reinterpret_cast<const Hash128 *>(data.constData());
        break;
    case Sha256:
        Q_ASSERT(sizeof(Hash256) == data.size());
        m_hash.sha256 = *reinterpret_cast<const Hash256 *>(data.constData());
        break;
    }
}

bool Hash::operator==(const Hash &other) const
{
    if (m_type != other.m_type) {
        return false;
    }
    switch (m_type) {
    case Invalid:
        return true;
    case Md5:
        return m_hash.md5 == other.m_hash.md5;
    case Sha256:
        return m_hash.sha256 == other.m_hash.sha256;
    }
    Q_UNREACHABLE();
}

bool Hash::isValid() const
{
    return m_type != Invalid;
}

Hash::Type Hash::type() const
{
    return m_type;
}

Hash128 Hash::md5() const
{
    return m_hash.md5;
}

Hash256 Hash::sha256() const
{
    return m_hash.sha256;
}

QString Hash::hexString() const
{
    switch (m_type) {
    case Invalid:
        return QString();
    case Md5:
        return QString::fromLatin1(QByteArray::fromRawData(reinterpret_cast<const char *>(&m_hash.md5), sizeof(Hash128)).toHex());
    case Sha256:
        return QString::fromLatin1(QByteArray::fromRawData(reinterpret_cast<const char *>(&m_hash.sha256), sizeof(Hash256)).toHex());
    }
    Q_UNREACHABLE();
}

uint Gravatar::qHash(const Hash &h, uint seed)
{
    switch (h.type()) {
    case Hash::Invalid:
        return seed;
    case Hash::Md5:
        return qHashBits(&h.m_hash.md5, sizeof(Hash128), seed);
    case Hash::Sha256:
        return qHashBits(&h.m_hash.sha256, sizeof(Hash256), seed);
    }
    Q_UNREACHABLE();
}
