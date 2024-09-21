/************************************************************************
 ** @file   application_2d.h
 ** @author Douglas S Caskey
 ** @date   7 Mar, 2024
 **
 ** @brief
 ** @copyright
 ** This source code is part of the Seamly2D project, a pattern making
 ** program to create and model patterns of clothing.
 ** Copyright (C) 2017-2024 Seamly2D project
 ** <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 ** Seamly2D is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** Seamly2D is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************

 ************************************************************************
 **
 **  @file   vapplication.h
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

#ifndef APPLICATION_2D_H
#define APPLICATION_2D_H

#include "../options.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vsettings.h"
#include "../vpatterndb/vtranslatevars.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "vcmdexport.h"

#include <memory>


class Application2D;   // use in define

#if defined(qApp)
#    undef qApp
#endif
#define qApp (static_cast<Application2D*>(VAbstractApplication::instance()))

/**
 * @brief The Application2D class reimplamentation QApplication class.
 */
class Application2D : public VAbstractApplication
{
    Q_OBJECT
public:
    Application2D(int& argc, char** argv);
    ~Application2D() override;
    static void startNewSeamly2D(const QString& fileName = QString());
    bool notify(QObject* receiver, QEvent* event) override;

    void initOptions();

    QString seamlyMeFilePath() const;

    QTimer* getAutoSaveTimer() const;
    void setAutoSaveTimer(QTimer* value);

    static QStringList pointNameLanguages();

    void startLogging();
    QTextStream* logFile();

    const VTranslateVars* translateVariables() override;

    bool static isGUIMode();
    bool isAppInGUIMode() const override;

    void openSettings() override;
    VSettings* Seamly2DSettings();

protected:
    void initTranslateVariables() override;
    bool event(QEvent* e) override;

private:
    Q_DISABLE_COPY(Application2D)
    VTranslateVars* m_trVars;
    QTimer* m_autoSaveTimer;

    std::shared_ptr<VLockGuard<QFile>> m_lockLog;
    std::shared_ptr<QTextStream> m_out;

    QString logDirPath() const;
    QString logPath() const;
    bool createLogDir() const;
    void beginLogging();
    void clearOldLogs() const;

public:
    // moved to the end of class so merge should go
    const VCommandLinePtr commandLine() const;
};

//---------------------------------------------------------------------------------------------------------------------
inline QTimer* Application2D::getAutoSaveTimer() const { return m_autoSaveTimer; }

//---------------------------------------------------------------------------------------------------------------------
inline void Application2D::setAutoSaveTimer(QTimer* value) { m_autoSaveTimer = value; }
//---------------------------------------------------------------------------------------------------------------------
#endif   // APPLICATION_2D_H
