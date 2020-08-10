/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARCONFIGURESETTINGSDIALOG_H
#define GRAVATARCONFIGURESETTINGSDIALOG_H

#include <QDialog>
#include "gravatar_export.h"
namespace Gravatar {
class GravatarConfigureSettingsWidget;

/** Gravatar settings dialog. */
class GRAVATAR_EXPORT GravatarConfigureSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsDialog(QWidget *parent = nullptr);
    ~GravatarConfigureSettingsDialog();

private:
    void slotRestoreDefault();
    void save();
    void load();
    GravatarConfigureSettingsWidget *mGravatarConfigureSettings;
};
}

#endif // GRAVATARCONFIGURESETTINGSDIALOG_H
