/******************************************************************************
 *   @file   pen_toolbar.h
 **  @author DS Caskey
 **  @date   Jan 16, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
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

#ifndef PEN_TOOLBAR_H
#define PEN_TOOLBAR_H

#include "color_combobox.h"
#include "linetype_combobox.h"
#include "lineweight_combobox.h"

#include "../ifc/xml/pen.h"

#include <QObject>
#include <QToolBar>
#include <QWidget>

class PenToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit PenToolBar(const QString& title, QWidget* parent = nullptr);

private slots:
    void colorChanged(const QString& color);
    void lineWeightChanged(const qreal& weight);
    void lineTypeChanged(const QString& type);
    void penReset();
    void savePreset();

signals:
    void penChanged(Pen pen);

private:
    Pen m_currentPen;
    ColorComboBox* m_colorBox;
    LineTypeComboBox* m_lineTypeBox;
    LineWeightComboBox* m_lineWeightBox;
};

#endif
