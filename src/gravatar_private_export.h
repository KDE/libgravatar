/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef GRAVATAR_TESTS_EXPORT
#define GRAVATAR_TESTS_EXPORT GRAVATAR_EXPORT
#endif
#else /* not compiling tests */
#define GRAVATAR_TESTS_EXPORT
#endif

