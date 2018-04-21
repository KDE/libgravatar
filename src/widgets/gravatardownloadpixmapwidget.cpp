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

#include "gravatardownloadpixmapwidget.h"
#include "gravatar/gravatarresolvurljob.h"
#include "gravatar_debug.h"
#include <QLabel>
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

using namespace Gravatar;
GravatarDownloadPixmapWidget::GravatarDownloadPixmapWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *hbox = new QHBoxLayout;
    mainLayout->addLayout(hbox);
    QLabel *lab = new QLabel(i18n("Email:"));
    lab->setObjectName(QStringLiteral("labemail"));
    hbox->addWidget(lab);
    mLineEdit = new QLineEdit;
    mLineEdit->setObjectName(QStringLiteral("email"));
    connect(mLineEdit, &QLineEdit::textChanged, this, &GravatarDownloadPixmapWidget::slotTextChanged);
    hbox->addWidget(mLineEdit);

    mGetPixmapButton = new QPushButton(i18n("&Search"));
    mGetPixmapButton->setObjectName(QStringLiteral("searchbutton"));
    connect(mGetPixmapButton, &QAbstractButton::clicked, this, &GravatarDownloadPixmapWidget::slotSearchButton);
    hbox->addWidget(mGetPixmapButton);
    mGetPixmapButton->setEnabled(false);

    mResultLabel = new QLabel;
    QFont font = mResultLabel->font();
    font.setBold(true);
    mResultLabel->setFont(font);
    mResultLabel->setObjectName(QStringLiteral("resultlabel"));
    mainLayout->addWidget(mResultLabel);
}

GravatarDownloadPixmapWidget::~GravatarDownloadPixmapWidget()
{
}

QPixmap GravatarDownloadPixmapWidget::gravatarPixmap() const
{
    return mGravatarPixmap;
}

void GravatarDownloadPixmapWidget::slotResolvUrlFinish(Gravatar::GravatarResolvUrlJob *job)
{
    if (job) {
        qCDebug(GRAVATAR_LOG) << job->hasGravatar();
        if (job->hasGravatar()) {
            mGravatarPixmap = job->pixmap();
            mResultLabel->setPixmap(mGravatarPixmap);
        } else {
            mResultLabel->setText(i18n("No Gravatar found."));
            mGravatarPixmap = QPixmap();
        }
    }
}

void GravatarDownloadPixmapWidget::slotSearchButton()
{
    mResultLabel->setText(QString());
    Gravatar::GravatarResolvUrlJob *job = new Gravatar::GravatarResolvUrlJob(this);
    job->setEmail(mLineEdit->text());
    //For testing
    //job->setUseDefaultPixmap(true);
    if (job->canStart()) {
        connect(job, &GravatarResolvUrlJob::finished, this, &GravatarDownloadPixmapWidget::slotResolvUrlFinish);
        job->start();
    } else {
        job->deleteLater();
    }
}

void GravatarDownloadPixmapWidget::slotTextChanged(const QString &text)
{
    mGetPixmapButton->setEnabled(!text.trimmed().isEmpty());
}
