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
 **  @file   tst_qmuparsererrormsg.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2015
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

#ifndef TST_QMUPARSERERRORMSG_H
#define TST_QMUPARSERERRORMSG_H

#include "../qmuparser/qmuparsererror.h"
#include "../vtest/abstracttest.h"

#include <QPointer>
#include <QTranslator>

class TST_QmuParserErrorMsg : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_QmuParserErrorMsg(const QString& locale, QObject* parent = nullptr);
    virtual ~TST_QmuParserErrorMsg() Q_DECL_OVERRIDE;

private slots:
    void initTestCase();
    void TestEErrorCodes_data();
    void TestEErrorCodes();
    void cleanupTestCase();

private:
    Q_DISABLE_COPY(TST_QmuParserErrorMsg)

    QString m_locale;
    QPointer<QTranslator> appTranslator;
    qmu::QmuParserErrorMsg* msg;

    void AddCase(int code, bool tok, bool pos);
    int loadTranslations(const QString& checkedLocale);
    void RemoveTranslation();
};

#endif   // TST_QMUPARSERERRORMSG_H
