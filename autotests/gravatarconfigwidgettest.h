/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARCONFIGWIDGETTEST_H
#define GRAVATARCONFIGWIDGETTEST_H

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

#endif // GRAVATARCONFIGWIDGETTEST_H
