/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QDialog>

namespace Gravatar
{
class GravatarDownloadPixmapWidget;

/*!
 * \class Gravatar::GravatarDownloadPixmapDialog
 * \inmodule Gravatar
 * \inheaderfile Gravatar/GravatarDownloadPixmapDialog
 * \brief Avatar download dialog
 * \author Laurent Montel <montel@kde.org>
 */
class GRAVATAR_EXPORT GravatarDownloadPixmapDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructs a Gravatar download pixmap dialog.
     * \param parent the parent widget
     */
    explicit GravatarDownloadPixmapDialog(QWidget *parent = nullptr);
    /*!
     * Destroys the Gravatar download pixmap dialog.
     */
    ~GravatarDownloadPixmapDialog() override;

    /*!
     * Returns the downloaded Gravatar pixmap.
     * \return the Gravatar pixmap
     */
    [[nodiscard]] QPixmap gravatarPixmap() const;

private:
    GRAVATAR_NO_EXPORT void slotAccepted();
    Gravatar::GravatarDownloadPixmapWidget *mGravatarDownloadPixmapWidget = nullptr;
};
}
