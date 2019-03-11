/*
   Copyright (C) 2019 Laurent Montel <montel@kde.org>

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


#include "gravatarconfiguresettingswidget.h"
#include <Gravatar/GravatarCache>
#include <PimCommon/ConfigureImmutableWidgetUtils>
#include <KLocalizedString>
#include <KPluralHandlingSpinBox>
#include <KSeparator>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "gravatarsettings.h"

using namespace Gravatar;
using namespace PimCommon::ConfigureImmutableWidgetUtils;
GravatarConfigureSettingsWidget::GravatarConfigureSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("mainlayout"));
    topLayout->setContentsMargins(0, 0, 0, 0);
    mUseDefaultPixmap = new QCheckBox(i18n("Use Default Image"), this);
    mUseDefaultPixmap->setObjectName(QStringLiteral("usedefaultimage"));
    topLayout->addWidget(mUseDefaultPixmap);

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
    saveSpinBox(mGravatarCacheSize, Gravatar::GravatarSettings::self()->gravatarCacheSizeItem());
}

void GravatarConfigureSettingsWidget::load()
{
    loadWidget(mUseDefaultPixmap, Gravatar::GravatarSettings::self()->gravatarUseDefaultImageItem());
    loadWidget(mGravatarCacheSize, Gravatar::GravatarSettings::self()->gravatarCacheSizeItem());
}
