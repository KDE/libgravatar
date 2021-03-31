/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QWidget>
class QCheckBox;
class QPushButton;
class KPluralHandlingSpinBox;
namespace Gravatar
{
/** Gravatar settings widget. */
class GRAVATAR_EXPORT GravatarConfigureSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsWidget(QWidget *parent = nullptr);
    ~GravatarConfigureSettingsWidget();
    void slotRestoreDefault();
    void save();
    void load();

private:
    void slotClearGravatarCache();
    QCheckBox *mUseDefaultPixmap = nullptr;
    QPushButton *mClearGravatarCache = nullptr;
    KPluralHandlingSpinBox *mGravatarCacheSize = nullptr;
    QCheckBox *mUseLibravatar = nullptr;
    QCheckBox *mFallbackGravatar = nullptr;
};
}

