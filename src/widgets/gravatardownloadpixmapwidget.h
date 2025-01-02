/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "gravatar_export.h"
#include <QWidget>
class QLabel;
class QLineEdit;
class QPushButton;
class QCheckBox;
namespace Gravatar
{
class GravatarResolvUrlJob;

/** Avatar download widget. */
class GRAVATAR_EXPORT GravatarDownloadPixmapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GravatarDownloadPixmapWidget(QWidget *parent = nullptr);
    ~GravatarDownloadPixmapWidget() override;

    [[nodiscard]] QPixmap gravatarPixmap() const;

private:
    GRAVATAR_NO_EXPORT void slotSearchButton();

    GRAVATAR_NO_EXPORT void slotTextChanged(const QString &text);
    GRAVATAR_NO_EXPORT void slotResolvUrlFinish(Gravatar::GravatarResolvUrlJob *job);
    QPixmap mGravatarPixmap;
    QLabel *mResultLabel = nullptr;
    QLineEdit *mLineEdit = nullptr;
    QPushButton *mGetPixmapButton = nullptr;
    QCheckBox *mUseLibravatar = nullptr;
    QCheckBox *mFallbackGravatar = nullptr;
};
}
