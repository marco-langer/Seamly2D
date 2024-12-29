/******************************************************************************
 *   @file   vformulapropertyeditor.h
 **  @author DS Caskey
 **  @date   Feb 18, 2023
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
 *************************************************************************

 ************************************************************************
 **
 **  @file   vformulapropertyeditor.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VFORMULAPROPERTYEDITOR_H
#define VFORMULAPROPERTYEDITOR_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QToolButton>
#include <QWidget>

#include "../vpatterndb/vformula.h"

class VFormulaPropertyEditor : public QWidget
{
    Q_OBJECT

public:
    //! Constructor taking a widget as parent
    explicit VFormulaPropertyEditor(QWidget* parent);

    //! Returns the formula currently set
    VFormula GetFormula() const;

    //! Needed for proper event handling
    bool eventFilter(QObject* obj, QEvent* ev) override;

signals:
    //! This is emitted, when the user changes the color
    void dataChangedByUser(const VFormula& GetFormula, VFormulaPropertyEditor* editor);

    void dataChanged();

public slots:
    //! Sets the color of the widget
    void SetFormula(const VFormula& formula);

private slots:
    void onToolButtonClicked();

private:
    Q_DISABLE_COPY(VFormulaPropertyEditor)
    VFormula formula;
    QToolButton* toolButton;
    QLabel* textLabel;
    QSpacerItem* spacer;
};


#endif   // VFORMULAPROPERTYEDITOR_H
