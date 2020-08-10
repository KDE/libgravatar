/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/


#ifndef GRAVATARCONFIGURESETTINGSWIDGET_H
#define GRAVATARCONFIGURESETTINGSWIDGET_H

#include <QWidget>
#include "gravatar_export.h"
class QCheckBox;
class QPushButton;
class KPluralHandlingSpinBox;
namespace Gravatar {

/** Gravatar settings widget. */
class GRAVATAR_EXPORT GravatarConfigureSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GravatarConfigureSettingsWidget(QWidget *parent = nullptr);
    ~GravatarConfigureSettingsWidget();
    void slotRestoreDefault();
    void save();
    void load();
private:
    void slotClearGravatarCache();
    QCheckBox *mUseDefaultPixmap = nullptr;
    QPushButton *mClearGravatarCache = nullptr;
    KPluralHandlingSpinBox *mGravatarCacheSize = nullptr;
    QCheckBox *mUseLibravatar = nullptr;
    QCheckBox *mFallbackGravatar = nullptr;
};
}

#endif // GRAVATARCONFIGURESETTINGSWIDGET_H
