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
 **  @file   calculator.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QHash>
#include <QMap>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../qmuparser/qmuformulabase.h"

// class VInternalVariable;
#include "variables/vinternalvariable.h"
/**
 * @brief The Calculator class for calculation formula.
 *
 * Main purpose make easy evaluate value of formula and get tokens.
 * Note. If created to many parser for different purpes in the same time parser can work wrong.
 * Example:
 * EditFormulaDialog *dialog = new EditFormulaDialog(data);
 * dialog->SetFormula(formula);
 * if (dialog->exec() == QDialog::Accepted)
 * {
 *     formula = dialog->GetFormula();
 *     //Need delete dialog here because parser in dialog don't allow use correct separator for
 * parsing here.
 *     //Don't know why.
 *     delete dialog;
 *     Calculator cal;
 *     result = cal.EvalFormula(data->PlainVariables(), formula);
 * }
 */
class Calculator : public qmu::QmuFormulaBase
{
public:
    Calculator();

    qreal EvalFormula(
        const QHash<QString, QSharedPointer<VInternalVariable>>& vars, const QString& formula);

private:
    Q_DISABLE_COPY(Calculator)

    void InitVariables(
        const QHash<QString, QSharedPointer<VInternalVariable>>& vars,
        const QMap<int, QString>& tokens,
        const QString& formula);
};

#endif   // CALCULATOR_H
