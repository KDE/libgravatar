/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QDialog>
namespace Gravatar
{
class GravatarConfigureSettingsWidget;

/** Gravatar settings dialog. */
class GRAVATAR_EXPORT GravatarConfigureSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsDialog(QWidget *parent = nullptr);
    ~GravatarConfigureSettingsDialog() override;

private:
    GRAVATAR_NO_EXPORT void slotRestoreDefault();
    GRAVATAR_NO_EXPORT void save();
    GRAVATAR_NO_EXPORT void load();
    GravatarConfigureSettingsWidget *mGravatarConfigureSettings = nullptr;
};
}
