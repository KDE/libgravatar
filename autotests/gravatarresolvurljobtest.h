/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GravatarResolvUrlJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GravatarResolvUrlJobTest(QObject *parent = nullptr);
    ~GravatarResolvUrlJobTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeValue();
    void shouldNotStart();

    void shouldGenerateGravatarUrl_data();
    void shouldGenerateGravatarUrl();
    void shouldChangeSize();

    void shouldAddSizeInUrl();

    void shouldUseDefaultPixmap();
    void shouldUseHttps();
};
