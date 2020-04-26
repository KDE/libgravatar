/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#include "gravatarconfiguresettingsdialog.h"
#include "gravatarconfiguresettingswidget.h"
#include "misc/gravatarcache.h"
#include <PimCommon/ConfigureImmutableWidgetUtils>

#include <KLocalizedString>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "gravatarsettings.h"
using namespace Gravatar;
using namespace PimCommon::ConfigureImmutableWidgetUtils;

GravatarConfigureSettingsDialog::GravatarConfigureSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure Gravatar"));
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("toplayout"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &GravatarConfigureSettingsDialog::save);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &GravatarConfigureSettingsDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &GravatarConfigureSettingsDialog::slotRestoreDefault);
    okButton->setDefault(true);

    mUseDefaultPixmap = new QCheckBox(i18n("Use Default Image"), this);
    mUseDefaultPixmap->setObjectName(QStringLiteral("usedefaultimage"));
    topLayout->addWidget(mUseDefaultPixmap);

    mUseLibravatar = new QCheckBox(i18n("Use Libravatar"), this);
    mUseLibravatar->setObjectName(QStringLiteral("uselibravatarcheckbox"));
    topLayout->addWidget(mUseLibravatar);

    mFallbackGravatar = new QCheckBox(i18n("Fallback to Gravatar"), this);
    mFallbackGravatar->setObjectName(QStringLiteral("fallbackgravatar"));
    topLayout->addWidget(mFallbackGravatar);
    connect(mUseLibravatar, &QCheckBox::toggled, mFallbackGravatar, &QCheckBox::setEnabled);
    mFallbackGravatar->setEnabled(false);

    QHBoxLayout *cacheSizeLayout = new QHBoxLayout;
    topLayout->addLayout(cacheSizeLayout);
    QLabel *lab = new QLabel(i18n("Gravatar Cache Size:"), this);
    lab->setObjectName(QStringLiteral("gravatarcachesizelabel"));
    cacheSizeLayout->addWidget(lab);

    mGravatarCacheSize = new KPluralHandlingSpinBox(this);
    mGravatarCacheSize->setMinimum(1);
    mGravatarCacheSize->setMaximum(9999);
    mGravatarCacheSize->setSuffix(ki18ncp("add space before image", " image", " images"));
    mGravatarCacheSize->setObjectName(QStringLiteral("gravatarcachesize"));
    cacheSizeLayout->addWidget(mGravatarCacheSize);
    cacheSizeLayout->addStretch();

    KSeparator *separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator"));
    topLayout->addWidget(separator);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    topLayout->addLayout(buttonLayout);
    mClearGravatarCache = new QPushButton(i18n("Clear Gravatar Cache"), this);
    mClearGravatarCache->setObjectName(QStringLiteral("cleargravatarcachebutton"));
    buttonLayout->addWidget(mClearGravatarCache);
    buttonLayout->addStretch();

    separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator2"));
    topLayout->addWidget(separator);

    connect(mClearGravatarCache, &QAbstractButton::clicked, this, &GravatarConfigureSettingsDialog::slotClearGravatarCache);
    topLayout->addWidget(buttonBox);
    load();
}

GravatarConfigureSettingsDialog::~GravatarConfigureSettingsDialog()
{
}

void GravatarConfigureSettingsDialog::slotRestoreDefault()
{
    mGravatarConfigureSettings->load();
}

void GravatarConfigureSettingsDialog::save()
{
    mGravatarConfigureSettings->save();
    accept();
}

void GravatarConfigureSettingsDialog::load()
{
    mGravatarConfigureSettings->load();
}

