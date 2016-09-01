/*
   Copyright (C) 2015-2016 Laurent Montel <montel@kde.org>

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

#include "gravatardownloadpixmapdialog.h"
#include "gravatardownloadpixmapwidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace Gravatar;

GravatarDownloadPixmapDialog::GravatarDownloadPixmapDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mGravatarDownloadPixmapWidget = new Gravatar::GravatarDownloadPixmapWidget(this);
    mGravatarDownloadPixmapWidget->setObjectName(QStringLiteral("gravatarwidget"));
    mainLayout->addWidget(mGravatarDownloadPixmapWidget);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &GravatarDownloadPixmapDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

GravatarDownloadPixmapDialog::~GravatarDownloadPixmapDialog()
{

}

void GravatarDownloadPixmapDialog::slotAccepted()
{
    accept();
}

QPixmap GravatarDownloadPixmapDialog::gravatarPixmap() const
{
    return mGravatarDownloadPixmapWidget->gravatarPixmap();
}
