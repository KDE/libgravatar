/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef GRAVATARDOWNLOADPIXMAPWIDGET_H
#define GRAVATARDOWNLOADPIXMAPWIDGET_H

#include <QWidget>
#include "gravatar_export.h"
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
namespace Gravatar {
class GravatarResolvUrlJob;
class GRAVATAR_EXPORT GravatarDownloadPixmapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GravatarDownloadPixmapWidget(QWidget *parent = nullptr);
    ~GravatarDownloadPixmapWidget();

    Q_REQUIRED_RESULT QPixmap gravatarPixmap() const;

private:
    void slotSearchButton();

    void slotTextChanged(const QString &text);
    void slotResolvUrlFinish(Gravatar::GravatarResolvUrlJob *job);
    QPixmap mGravatarPixmap;
    QLabel *mResultLabel = nullptr;
    QLineEdit *mLineEdit = nullptr;
    QPushButton *mGetPixmapButton = nullptr;
};
}

#endif // GRAVATARDOWNLOADPIXMAPWIDGET_H
