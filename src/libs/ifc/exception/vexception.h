/***************************************************************************
 **  @file   vexception.h
 **  @author Douglas S Caskey
 **  @date   17 Sep, 2023
 **
 **  @copyright
 **  Copyright (C) 2017 - 2023 Seamly, LLC
 **  https://github.com/fashionfreedom/seamly2d
 **
 **  @brief
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
 **  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************

 ************************************************************************
 **  @file   vexception.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VEXCEPTION_H
#define VEXCEPTION_H

#include <QByteArray>
#include <QCoreApplication>
#include <QException>
#include <QString>
#include <qcompilerdetection.h>

/**
 * @brief The VException class parent for all exception. Could be use for abstract exception
 */
class VException : public QException
{
    Q_DECLARE_TR_FUNCTIONS(VException)

public:
    explicit VException(QString error);

    [[noreturn]] void raise() const override;

    // cppcheck-suppress unusedFunction
    [[nodiscard]] VException* clone() const override;

    virtual QString ErrorMessage() const;
    virtual QString DetailedInformation() const;
    void AddMoreInformation(const QString& info);
    QString MoreInformation() const;
    const char* what() const noexcept override;

protected:
    /** @brief error string with error */
    QString error;
    QByteArray m_errorUtf8;
    /** @brief moreInfo more information about error */
    QString moreInfo;

    QString MoreInfo(const QString& detInfo) const;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MoreInformation return more information for error
 * @return information
 */
inline QString VException::MoreInformation() const { return moreInfo; }

// Want have special exception for catching unhadled deleting a tool
class VExceptionToolWasDeleted : public VException
{
    Q_DECLARE_TR_FUNCTIONS(VExceptionToolDeleted)

public:
    explicit VExceptionToolWasDeleted(QString error);

    [[noreturn]] void raise() const override;
    // cppcheck-suppress unusedFunction
    VExceptionToolWasDeleted* clone() const override;
};

#endif   // VEXCEPTION_H
