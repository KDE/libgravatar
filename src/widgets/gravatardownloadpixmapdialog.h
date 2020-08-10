/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef GRAVATARDOWNLOADPIXMAPDIALOG_H
#define GRAVATARDOWNLOADPIXMAPDIALOG_H

#include <QDialog>
#include "gravatar_export.h"

namespace Gravatar {
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

#endif // GRAVATARDOWNLOADPIXMAPDIALOG_H
