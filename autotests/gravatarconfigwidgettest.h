/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GravatarConfigWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarConfigWidgetTest(QObject *parent = nullptr);
    ~GravatarConfigWidgetTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeState();
    void shoulEmitConfigChangedSignal();
};

