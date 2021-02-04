/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "gravatarconfiguresettingsdialog.h"
#include "gravatarconfiguresettingswidget.h"
#include "misc/gravatarcache.h"
#include <PimCommon/ConfigureImmutableWidgetUtils>

#include "gravatarsettings.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Gravatar;
using namespace PimCommon::ConfigureImmutableWidgetUtils;

GravatarConfigureSettingsDialog::GravatarConfigureSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure Gravatar"));
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("toplayout"));

    mGravatarConfigureSettings = new GravatarConfigureSettingsWidget(this);
    mGravatarConfigureSettings->setObjectName(QStringLiteral("gravatarconfiguresettings"));
    topLayout->addWidget(mGravatarConfigureSettings);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
    buttonBox->setObjectName(QStringLiteral("buttonbox"));
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &GravatarConfigureSettingsDialog::save);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &GravatarConfigureSettingsDialog::reject);
    connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &GravatarConfigureSettingsDialog::slotRestoreDefault);
    okButton->setDefault(true);

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
