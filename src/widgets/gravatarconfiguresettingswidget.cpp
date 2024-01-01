/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfiguresettingswidget.h"
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
    topLayout->setObjectName(QLatin1StringView("mainlayout"));
    topLayout->setContentsMargins(0, 0, 0, 0);
    mUseDefaultPixmap = new QCheckBox(i18n("Use Default Image"), this);
    mUseDefaultPixmap->setObjectName(QLatin1StringView("kcfg_GravatarUseDefaultImage"));
    topLayout->addWidget(mUseDefaultPixmap);

    mUseLibravatar = new QCheckBox(i18n("Use Libravatar"), this);
    mUseLibravatar->setObjectName(QLatin1StringView("kcfg_LibravatarSupportEnabled"));
    topLayout->addWidget(mUseLibravatar);

    mFallbackGravatar = new QCheckBox(i18n("Fallback to Gravatar"), this);
    mFallbackGravatar->setObjectName(QLatin1StringView("kcfg_FallbackToGravatar"));
    topLayout->addWidget(mFallbackGravatar);
    connect(mUseLibravatar, &QCheckBox::toggled, mFallbackGravatar, &QCheckBox::setEnabled);
    mFallbackGravatar->setEnabled(false);

    auto cacheSizeLayout = new QHBoxLayout;
    topLayout->addLayout(cacheSizeLayout);
    auto lab = new QLabel(i18n("Gravatar Cache Size:"), this);
    lab->setObjectName(QLatin1StringView("gravatarcachesizelabel"));
    cacheSizeLayout->addWidget(lab);

    mGravatarCacheSize = new KPluralHandlingSpinBox(this);
    mGravatarCacheSize->setMinimum(1);
    mGravatarCacheSize->setMaximum(9999);
    mGravatarCacheSize->setSuffix(ki18ncp("add space before image", " image", " images"));
    mGravatarCacheSize->setObjectName(QLatin1StringView("kcfg_GravatarCacheSize"));
    cacheSizeLayout->addWidget(mGravatarCacheSize);
    cacheSizeLayout->addStretch();

    auto separator = new KSeparator(this);
    separator->setObjectName(QLatin1StringView("separator"));
    topLayout->addWidget(separator);

    auto buttonLayout = new QHBoxLayout;
    topLayout->addLayout(buttonLayout);
    mClearGravatarCache = new QPushButton(i18n("Clear Gravatar Cache"), this);
    mClearGravatarCache->setObjectName(QLatin1StringView("cleargravatarcachebutton"));
    buttonLayout->addWidget(mClearGravatarCache);
    buttonLayout->addStretch();

    separator = new KSeparator(this);
    separator->setObjectName(QLatin1StringView("separator2"));
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
