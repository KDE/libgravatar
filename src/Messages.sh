#! /bin/sh
# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
$XGETTEXT `find . -name '*.h' -o -name '*.cpp' ` -o $podir/libgravatar6.pot
rm -f rc.cpp
