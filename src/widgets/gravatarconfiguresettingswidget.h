/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QWidget>
class QCheckBox;
class QPushButton;
class QSpinBox;
class KConfigDialogManager;
namespace Gravatar
{
/*!
 * \class Gravatar::GravatarConfigureSettingsWidget
 * \inmodule Gravatar
 * \inheaderfile Gravatar/GravatarConfigureSettingsWidget
 * \brief Gravatar settings widget
 * \author Laurent Montel <montel@kde.org>
 */
class GRAVATAR_EXPORT GravatarConfigureSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs a Gravatar configure settings widget.
     * \param parent the parent widget
     */
    explicit GravatarConfigureSettingsWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the Gravatar configure settings widget.
     */
    ~GravatarConfigureSettingsWidget() override;
    /*!
     * Restores the settings to their default values.
     */
    void slotRestoreDefault();
    /*!
     * Saves the current settings.
     */
    void save();
    /*!
     * Loads the settings from configuration.
     */
    void load();

private:
    GRAVATAR_NO_EXPORT void slotClearGravatarCache();
    QCheckBox *mUseDefaultPixmap = nullptr;
    QPushButton *mClearGravatarCache = nullptr;
    QSpinBox *mGravatarCacheSize = nullptr;
    QCheckBox *mUseLibravatar = nullptr;
    QCheckBox *mFallbackGravatar = nullptr;
    KConfigDialogManager *m_configDialogManager = nullptr;
};
}
