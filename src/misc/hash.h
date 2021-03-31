/*
   SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_private_export.h"

#include <cstdint>
#include <cstring>

class QByteArray;
class QString;

namespace Gravatar
{
template<int Size> struct UnsignedInt {
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
struct Hash128 : public UnsignedInt<16> {
};
struct Hash256 : public UnsignedInt<32> {
};

class Hash;
unsigned int qHash(const Hash &h, unsigned int seed = 0);

// exported for unit tests only
class GRAVATAR_TESTS_EXPORT Hash
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

