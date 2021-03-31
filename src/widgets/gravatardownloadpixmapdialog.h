/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QDialog>

namespace Gravatar
{
class GravatarDownloadPixmapWidget;

/** Avatar download dialog. */
class GRAVATAR_EXPORT GravatarDownloadPixmapDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GravatarDownloadPixmapDialog(QWidget *parent = nullptr);
    ~GravatarDownloadPixmapDialog();

    Q_REQUIRED_RESULT QPixmap gravatarPixmap() const;

private:
    void slotAccepted();
    Gravatar::GravatarDownloadPixmapWidget *mGravatarDownloadPixmapWidget = nullptr;
};
}

