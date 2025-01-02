/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatardownloadpixmapwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    auto lab = new QLabel(i18nc("@label:textbox", "Email:"), this);
    lab->setObjectName("labemail"_L1);
    hbox->addWidget(lab);
    mLineEdit = new QLineEdit;
    mLineEdit->setObjectName("email"_L1);
    connect(mLineEdit, &QLineEdit::textChanged, this, &GravatarDownloadPixmapWidget::slotTextChanged);
    hbox->addWidget(mLineEdit);

    mUseLibravatar = new QCheckBox(i18nc("@option:check", "Use Libravatar"), this);
    mUseLibravatar->setObjectName("uselibravatar"_L1);
    mainLayout->addWidget(mUseLibravatar);

    mFallbackGravatar = new QCheckBox(i18nc("@option:check", "Fallback to Gravatar"), this);
    mFallbackGravatar->setObjectName("fallbackgravatar"_L1);
    mainLayout->addWidget(mFallbackGravatar);

    mGetPixmapButton = new QPushButton(i18nc("@action:button", "&Search"), this);
    mGetPixmapButton->setObjectName("searchbutton"_L1);
    connect(mGetPixmapButton, &QAbstractButton::clicked, this, &GravatarDownloadPixmapWidget::slotSearchButton);
    hbox->addWidget(mGetPixmapButton);
    mGetPixmapButton->setEnabled(false);

    mResultLabel = new QLabel(this);
    QFont font = mResultLabel->font();
    font.setBold(true);
    mResultLabel->setFont(font);
    mResultLabel->setObjectName("resultlabel"_L1);
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
