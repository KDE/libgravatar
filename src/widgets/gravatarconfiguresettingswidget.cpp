/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfiguresettingswidget.h"
#include "gravatarsettings.h"
#include <Gravatar/GravatarCache>
#include <KLocalizedString>
#include <KPluralHandlingSpinBox>
#include <KSeparator>
#include <PimCommon/ConfigureImmutableWidgetUtils>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Gravatar;
using namespace PimCommon::ConfigureImmutableWidgetUtils;
GravatarConfigureSettingsWidget::GravatarConfigureSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("mainlayout"));
    topLayout->setContentsMargins(0, 0, 0, 0);
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

    auto cacheSizeLayout = new QHBoxLayout;
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

    auto buttonLayout = new QHBoxLayout;
    topLayout->addLayout(buttonLayout);
    mClearGravatarCache = new QPushButton(i18n("Clear Gravatar Cache"), this);
    mClearGravatarCache->setObjectName(QStringLiteral("cleargravatarcachebutton"));
    buttonLayout->addWidget(mClearGravatarCache);
    buttonLayout->addStretch();

    separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator2"));
    topLayout->addWidget(separator);

    connect(mClearGravatarCache, &QAbstractButton::clicked, this, &GravatarConfigureSettingsWidget::slotClearGravatarCache);
}

GravatarConfigureSettingsWidget::~GravatarConfigureSettingsWidget()
{
}

void GravatarConfigureSettingsWidget::slotClearGravatarCache()
{
    Gravatar::GravatarCache::self()->clearAllCache();
}

void GravatarConfigureSettingsWidget::slotRestoreDefault()
{
    const bool bUseDefaults = Gravatar::GravatarSettings::self()->useDefaults(true);
    load();
    Gravatar::GravatarSettings::self()->useDefaults(bUseDefaults);
}

void GravatarConfigureSettingsWidget::save()
{
    saveCheckBox(mUseDefaultPixmap, Gravatar::GravatarSettings::self()->gravatarUseDefaultImageItem());
    saveCheckBox(mUseLibravatar, Gravatar::GravatarSettings::self()->libravatarSupportEnabledItem());
    saveCheckBox(mFallbackGravatar, Gravatar::GravatarSettings::self()->fallbackToGravatarItem());
    saveSpinBox(mGravatarCacheSize, Gravatar::GravatarSettings::self()->gravatarCacheSizeItem());
}

void GravatarConfigureSettingsWidget::load()
{
    loadWidget(mUseDefaultPixmap, Gravatar::GravatarSettings::self()->gravatarUseDefaultImageItem());
    loadWidget(mGravatarCacheSize, Gravatar::GravatarSettings::self()->gravatarCacheSizeItem());
    loadWidget(mUseLibravatar, Gravatar::GravatarSettings::self()->libravatarSupportEnabledItem());
    loadWidget(mFallbackGravatar, Gravatar::GravatarSettings::self()->fallbackToGravatarItem());
}
