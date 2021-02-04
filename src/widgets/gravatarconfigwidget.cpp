/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfigwidget.h"
#include "gravatarconfiguresettingsdialog.h"
#include "gravatarsettings.h"
#include "misc/gravatarcache.h"
#include <KLocalizedString>
#include <PimCommon/ConfigureImmutableWidgetUtils>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>

using namespace Gravatar;
using namespace PimCommon::ConfigureImmutableWidgetUtils;

class Q_DECL_HIDDEN Gravatar::GravatarConfigWidgetPrivate
{
public:
    GravatarConfigWidgetPrivate()
    {
    }

    QCheckBox *mEnableGravatarSupport = nullptr;
    QPushButton *mConfigureGravatarSetting = nullptr;
};

GravatarConfigWidget::GravatarConfigWidget(QWidget *parent)
    : QWidget(parent)
    , d(new Gravatar::GravatarConfigWidgetPrivate)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    d->mEnableGravatarSupport = new QCheckBox(i18n("Enable Gravatar Support"));
    d->mEnableGravatarSupport->setObjectName(QStringLiteral("gravatarcheckbox"));
    d->mEnableGravatarSupport->setChecked(false);
    mainLayout->addWidget(d->mEnableGravatarSupport);

    d->mConfigureGravatarSetting = new QPushButton(i18n("Configure..."));
    d->mConfigureGravatarSetting->setObjectName(QStringLiteral("configure"));
    mainLayout->addWidget(d->mConfigureGravatarSetting);
    connect(d->mConfigureGravatarSetting, &QPushButton::clicked, this, &GravatarConfigWidget::slotConfigureSettings);
    mainLayout->addStretch();

    connect(d->mEnableGravatarSupport, &QCheckBox::toggled, this, &GravatarConfigWidget::slotGravatarEnableChanged);
    updateWidgetState(false);
}

GravatarConfigWidget::~GravatarConfigWidget()
{
    delete d;
}

void GravatarConfigWidget::slotConfigureSettings()
{
    QPointer<Gravatar::GravatarConfigureSettingsDialog> dlg = new Gravatar::GravatarConfigureSettingsDialog(this);
    dlg->exec();
    delete dlg;
}

void GravatarConfigWidget::slotGravatarEnableChanged(bool state)
{
    updateWidgetState(state);
    Q_EMIT configChanged(state);
}

void GravatarConfigWidget::updateWidgetState(bool state)
{
    d->mConfigureGravatarSetting->setEnabled(state);
}

void GravatarConfigWidget::save()
{
    saveCheckBox(d->mEnableGravatarSupport, Gravatar::GravatarSettings::self()->gravatarSupportEnabledItem());
    if (!d->mEnableGravatarSupport->isChecked()) {
        Gravatar::GravatarCache::self()->clearAllCache();
    }
}

void GravatarConfigWidget::doLoadFromGlobalSettings()
{
    loadWidget(d->mEnableGravatarSupport, Gravatar::GravatarSettings::self()->gravatarSupportEnabledItem());
    updateWidgetState(d->mEnableGravatarSupport->isChecked());
}

void GravatarConfigWidget::doResetToDefaultsOther()
{
    const bool bUseDefaults = Gravatar::GravatarSettings::self()->useDefaults(true);
    doLoadFromGlobalSettings();
    Gravatar::GravatarSettings::self()->useDefaults(bUseDefaults);
}
