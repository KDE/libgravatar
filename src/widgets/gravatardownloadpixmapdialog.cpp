/*
   SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatardownloadpixmapdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "gravatardownloadpixmapwidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

using namespace Gravatar;

GravatarDownloadPixmapDialog::GravatarDownloadPixmapDialog(QWidget *parent)
    : QDialog(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mGravatarDownloadPixmapWidget = new Gravatar::GravatarDownloadPixmapWidget(this);
    mGravatarDownloadPixmapWidget->setObjectName("gravatarwidget"_L1);
    mainLayout->addWidget(mGravatarDownloadPixmapWidget);
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName("buttonbox"_L1);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &GravatarDownloadPixmapDialog::slotAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

GravatarDownloadPixmapDialog::~GravatarDownloadPixmapDialog() = default;

void GravatarDownloadPixmapDialog::slotAccepted()
{
    accept();
}

QPixmap GravatarDownloadPixmapDialog::gravatarPixmap() const
{
    return mGravatarDownloadPixmapWidget->gravatarPixmap();
}

#include "moc_gravatardownloadpixmapdialog.cpp"
