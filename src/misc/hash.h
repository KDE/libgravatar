/*
   Copyright (c) 2017 Volker Krause <vkrause@kde.org>

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

#ifndef GRAVATAR_HASH_H
#define GRAVATAR_HASH_H

#include "gravatar_export.h"

#include <cstdint>
#include <cstring>

class QByteArray;
class QString;

namespace Gravatar {

template <int Size> struct UnsignedInt
{
    bool operator<(const UnsignedInt<Size> &other) const
    {
        return memcmp(data, other.data, Size) < 0;
    }

    bool operator==(const UnsignedInt<Size> &other) const
    {
        return memcmp(data, other.data, Size) == 0;
    }

    uint8_t data[Size];
};
struct Hash128 : public UnsignedInt<16> {};
struct Hash256 : public UnsignedInt<32> {};

class Hash;
unsigned int qHash(const Hash &h, unsigned int seed = 0);

// exported for unit tests only
class GRAVATAR_EXPORT Hash
{
public:
    enum Type { Invalid, Md5, Sha256 };
    Hash();
    explicit Hash(const QByteArray &data, Type type);

    bool operator==(const Hash &other) const;

    bool isValid() const;

    Type type() const;
    Hash128 md5() const;
    Hash256 sha256() const;

    QString hexString() const;

private:
    friend unsigned int qHash(const Hash &h, unsigned int seed);
    union {
        Hash128 md5;
        Hash256 sha256;
    } m_hash;
    Type m_type;
};

}

#endif // GRAVATAR_HASH_H
