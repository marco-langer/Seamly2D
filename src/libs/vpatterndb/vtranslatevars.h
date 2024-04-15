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
 **  @file   vtranslatevars.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
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

#ifndef VTRANSLATEVARS_H
#define VTRANSLATEVARS_H

#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vtranslatemeasurements.h"

class VTranslateVars : public VTranslateMeasurements
{
public:
    explicit VTranslateVars();
    virtual ~VTranslateVars() override;

    bool
    VariablesFromUser(QString& newFormula, int position, const QString& token, int& bias) const;
    bool PostfixOperatorsFromUser(
        QString& newFormula, int position, const QString& token, int& bias) const;
    bool
    FunctionsFromUser(QString& newFormula, int position, const QString& token, int& bias) const;
    bool VariablesToUser(QString& newFormula, int position, const QString& token, int& bias) const;

    QString InternalVarToUser(const QString& var) const;
    QString PlaceholderToUser(const QString& var) const;

    QString PlaceholderToUserText(QString text) const;
    QString PlaceholderFromUserText(QString text) const;

    QString VarToUser(const QString& var) const;
    QString VarFromUser(const QString& var) const;

    QString PMSystemName(const QString& code) const;
    QString PMSystemAuthor(const QString& code) const;
    QString PMSystemBook(const QString& code) const;

    QString PostfixOperator(const QString& name) const;

    QString FormulaFromUser(const QString& formula, bool osSeparator) const;
    static QString TryFormulaFromUser(const QString& formula, bool osSeparator);
    QString FormulaToUser(const QString& formula, bool osSeparator) const;

    virtual void Retranslate() override;

    QMap<QString, qmu::QmuTranslation> getFunctions() const;

    static void BiasTokens(int position, int bias, QMap<int, QString>& tokens);

private:
    Q_DISABLE_COPY(VTranslateVars)
    QMap<QString, qmu::QmuTranslation> PMSystemNames;
    QMap<QString, qmu::QmuTranslation> PMSystemAuthors;
    QMap<QString, qmu::QmuTranslation> PMSystemBooks;
    QMap<QString, qmu::QmuTranslation> variables;
    QMap<QString, qmu::QmuTranslation> functions;
    QMap<QString, qmu::QmuTranslation> postfixOperators;
    QMap<QString, qmu::QmuTranslation> placeholders;
    QMap<QString, qmu::QmuTranslation> stDescriptions;

    void InitPatternMakingSystems();
    void InitVariables();
    void InitFunctions();
    void InitPostfixOperators();
    void InitPlaceholder();

    void InitSystem(
        const QString& code,
        const qmu::QmuTranslation& name,
        const qmu::QmuTranslation& author,
        const qmu::QmuTranslation& book);

    void CorrectionsPositions(
        int position, int bias, QMap<int, QString>& tokens, QMap<int, QString>& numbers) const;
};

#endif   // VTRANSLATEVARS_H
