/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                             *
 *                                                                         *
 ***************************************************************************
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 **************************************************************************

 ************************************************************************
 **
 **  @file   vwidgetpopup.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 2, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vwidgetpopup.h"

#include <QApplication>
#include <QFont>
#include <QGuiApplication>
#include <QLabel>
#include <QLayout>
#include <QMessageLogger>
#include <QPoint>
#include <QRect>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <Qt>

#include "../vmisc/scassert.h"

//---------------------------------------------------------------------------------------------------------------------
VWidgetPopup::VWidgetPopup(QWidget* parent)
    : QFrame(parent, Qt::Popup)
{
    setAttribute(Qt::WA_WindowPropagation);

    if (parentWidget() == nullptr) {
        setAttribute(Qt::WA_DeleteOnClose);
    }

    setLayout(new QVBoxLayout());
    layout()->setContentsMargins(0, 0, 0, 0);
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetPopup::SetWidget(QWidget* widget, bool own)
{
    if (mWidget) {
        layout()->removeWidget(mWidget);

        if (mOwn) {
            mWidget->setParent(nullptr);
            delete mWidget;
        } else {
            mWidget->setParent(mOldParent);
        }
    }

    mWidget = widget;
    mOwn = own;
    mOldParent = nullptr;

    if (mWidget) {
        mOldParent = mWidget->parentWidget();
        mWidget->setParent(this);
        layout()->addWidget(mWidget);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetPopup::PopupMessage(QWidget* w, const QString& msg)
{
    SCASSERT(w != nullptr)

    VWidgetPopup* popup = new VWidgetPopup(w);
    QLabel* label = new QLabel(msg);
    QFont f = label->font();
    f.setBold(true);
    f.setPixelSize(16);
    label->setFont(f);
    popup->SetWidget(label);
    popup->SetLifeTime(2000);
    popup->Show(w->frameGeometry().center());
}

//---------------------------------------------------------------------------------------------------------------------
void VWidgetPopup::Show(QPoint coord)
{
    // important to do this before following adjustments!
    QFrame::show();

    const QRect screen(QGuiApplication::primaryScreen()->availableGeometry());
    coord.setX(coord.x() - width() / 2);

    if (coord.x() < screen.x()) {
        coord.setX(screen.x());
    }

    if (coord.y() < screen.y()) {
        coord.setY(screen.y());
    }

    if (coord.x() > (screen.right() - width())) {
        coord.setX(screen.right() - width());
    }

    if (coord.y() > (screen.bottom() - height())) {
        coord.setY(screen.bottom() - height());
    }
    move(coord);

    if (lifeTime > 0) {
        QTimer::singleShot(lifeTime, this, SLOT(close()));
    }
}
