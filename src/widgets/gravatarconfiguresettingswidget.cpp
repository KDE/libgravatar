/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfiguresettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "gravatarsettings.h"
#include <Gravatar/GravatarCache>
#include <KConfigDialogManager>
#include <KLocalizedString>
#include <KPluralHandlingSpinBox>
#include <KSeparator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Gravatar;

GravatarConfigureSettingsWidget::GravatarConfigureSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName("mainlayout"_L1);
    topLayout->setContentsMargins(0, 0, 0, 0);
    mUseDefaultPixmap = new QCheckBox(i18n("Use Default Image"), this);
    mUseDefaultPixmap->setObjectName("kcfg_GravatarUseDefaultImage"_L1);
    topLayout->addWidget(mUseDefaultPixmap);

    mUseLibravatar = new QCheckBox(i18n("Use Libravatar"), this);
    mUseLibravatar->setObjectName("kcfg_LibravatarSupportEnabled"_L1);
    topLayout->addWidget(mUseLibravatar);

    mFallbackGravatar = new QCheckBox(i18n("Fallback to Gravatar"), this);
    mFallbackGravatar->setObjectName("kcfg_FallbackToGravatar"_L1);
    topLayout->addWidget(mFallbackGravatar);
    connect(mUseLibravatar, &QCheckBox::toggled, mFallbackGravatar, &QCheckBox::setEnabled);
    mFallbackGravatar->setEnabled(false);

    auto cacheSizeLayout = new QHBoxLayout;
    topLayout->addLayout(cacheSizeLayout);
    auto lab = new QLabel(i18nc("@label:textbox", "Gravatar Cache Size:"), this);
    lab->setObjectName("gravatarcachesizelabel"_L1);
    cacheSizeLayout->addWidget(lab);

    mGravatarCacheSize = new KPluralHandlingSpinBox(this);
    mGravatarCacheSize->setMinimum(1);
    mGravatarCacheSize->setMaximum(9999);
    mGravatarCacheSize->setSuffix(ki18ncp("add space before image", " image", " images"));
    mGravatarCacheSize->setObjectName("kcfg_GravatarCacheSize"_L1);
    cacheSizeLayout->addWidget(mGravatarCacheSize);
    cacheSizeLayout->addStretch();

    auto separator = new KSeparator(this);
    separator->setObjectName("separator"_L1);
    topLayout->addWidget(separator);

    auto buttonLayout = new QHBoxLayout;
    topLayout->addLayout(buttonLayout);
    mClearGravatarCache = new QPushButton(i18n("Clear Gravatar Cache"), this);
    mClearGravatarCache->setObjectName("cleargravatarcachebutton"_L1);
    buttonLayout->addWidget(mClearGravatarCache);
    buttonLayout->addStretch();

    separator = new KSeparator(this);
    separator->setObjectName("separator2"_L1);
    topLayout->addWidget(separator);

    connect(mClearGravatarCache, &QAbstractButton::clicked, this, &GravatarConfigureSettingsWidget::slotClearGravatarCache);

    m_configDialogManager = new KConfigDialogManager(this, GravatarSettings::self());
}

GravatarConfigureSettingsWidget::~GravatarConfigureSettingsWidget() = default;

void GravatarConfigureSettingsWidget::slotClearGravatarCache()
{
    Gravatar::GravatarCache::self()->clearAllCache();
}

void GravatarConfigureSettingsWidget::slotRestoreDefault()
{
    m_configDialogManager->updateWidgetsDefault();
}

void GravatarConfigureSettingsWidget::save()
{
    m_configDialogManager->updateSettings();
}

void GravatarConfigureSettingsWidget::load()
{
    m_configDialogManager->updateWidgets();
}

#include "moc_gravatarconfiguresettingswidget.cpp"
