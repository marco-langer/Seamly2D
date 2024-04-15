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
 **  @file   main.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 5, 2014
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

#include "../qmuparser/qmuparsertest.h"
#include <QCoreApplication>
#include <QTimer>
#include <QtGlobal>

//---------------------------------------------------------------------------------------------------------------------
void testMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
    case QtWarningMsg: fprintf(stderr, "%s\n", localMsg.constData()); break;
    case QtCriticalMsg:
        fprintf(
            stderr,
            "Critical: %s (%s:%i, %s)\n",
            localMsg.constData(),
            context.file,
            context.line,
            context.function);
        break;
    case QtFatalMsg:
        fprintf(
            stderr,
            "Fatal: %s (%s:%i, %s)\n",
            localMsg.constData(),
            context.file,
            context.line,
            context.function);
        abort();
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(testMessageOutput);
    qmu::Test::QmuParserTester pt;
    QTimer::singleShot(0, &pt, SLOT(Run()));
    return a.exec();
}
