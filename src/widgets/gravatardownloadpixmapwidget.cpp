/*
   SPDX-FileCopyrightText: 2015-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatardownloadpixmapwidget.h"
#include "gravatar/gravatarresolvurljob.h"
#include "gravatar_debug.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Gravatar;
GravatarDownloadPixmapWidget::GravatarDownloadPixmapWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto hbox = new QHBoxLayout;
    mainLayout->addLayout(hbox);
    auto lab = new QLabel(i18n("Email:"));
    lab->setObjectName(QLatin1StringView("labemail"));
    hbox->addWidget(lab);
    mLineEdit = new QLineEdit;
    mLineEdit->setObjectName(QLatin1StringView("email"));
    connect(mLineEdit, &QLineEdit::textChanged, this, &GravatarDownloadPixmapWidget::slotTextChanged);
    hbox->addWidget(mLineEdit);

    mUseLibravatar = new QCheckBox(i18n("Use Libravatar"));
    mUseLibravatar->setObjectName(QLatin1StringView("uselibravatar"));
    mainLayout->addWidget(mUseLibravatar);

    mFallbackGravatar = new QCheckBox(i18n("Fallback to Gravatar"));
    mFallbackGravatar->setObjectName(QLatin1StringView("fallbackgravatar"));
    mainLayout->addWidget(mFallbackGravatar);

    mGetPixmapButton = new QPushButton(i18n("&Search"));
    mGetPixmapButton->setObjectName(QLatin1StringView("searchbutton"));
    connect(mGetPixmapButton, &QAbstractButton::clicked, this, &GravatarDownloadPixmapWidget::slotSearchButton);
    hbox->addWidget(mGetPixmapButton);
    mGetPixmapButton->setEnabled(false);

    mResultLabel = new QLabel;
    QFont font = mResultLabel->font();
    font.setBold(true);
    mResultLabel->setFont(font);
    mResultLabel->setObjectName(QLatin1StringView("resultlabel"));
    mainLayout->addWidget(mResultLabel);
}

GravatarDownloadPixmapWidget::~GravatarDownloadPixmapWidget() = default;

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
    auto job = new Gravatar::GravatarResolvUrlJob(this);
    job->setEmail(mLineEdit->text());
    // For testing
    // job->setUseDefaultPixmap(true);
    job->setUseLibravatar(mUseLibravatar->isChecked());
    job->setFallbackGravatar(mFallbackGravatar->isChecked());
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

#include "moc_gravatardownloadpixmapwidget.cpp"
