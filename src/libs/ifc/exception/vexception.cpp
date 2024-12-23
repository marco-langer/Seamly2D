/***************************************************************************
 **  @file   vexception.cpp
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
 **  @file   vexception.cpp
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

#include "vexception.h"

#include <QByteArray>
#include <QtGlobal>

#include "../ifcdef.h"

// Q_LOGGING_CATEGORY(vExcep, "v.excep") //Commented because don't use now

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VException constructor exception
 * @param error string with error
 */
VException::VException(QString error)
    : error{ std::move(error) }
    , m_errorUtf8{ this->error.toUtf8() }
{
    Q_ASSERT_X(!this->error.isEmpty(), Q_FUNC_INFO, "Error message is empty");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return error message
 */
QString VException::ErrorMessage() const { return tr("Exception: %1").arg(error); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddMoreInformation add more information for error
 * @param info information
 */
void VException::AddMoreInformation(const QString& info)
{
    if (info.isEmpty()) {
        return;
    }
    moreInfo = QString("%1\n%2").arg(moreInfo, info);
}

//---------------------------------------------------------------------------------------------------------------------
QString VException::MoreInfo(const QString& detInfo) const
{
    if (moreInfo.isEmpty() == false) {
        return QString("%1\n%2").arg(moreInfo, detInfo);
    } else {
        return detInfo;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DetailedInformation return detailed information about error
 * @return detailed information
 */
QString VException::DetailedInformation() const { return moreInfo; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clone clone exception
 * @return new exception
 */
// cppcheck-suppress unusedFunction
VException* VException::clone() const { return new VException(*this); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief raise method raise for exception
 */
// cppcheck-suppress unusedFunction
[[noreturn]] void VException::raise() const { throw *this; }

//---------------------------------------------------------------------------------------------------------------------
const char* VException::what() const noexcept { return m_errorUtf8.constData(); }

//-----------------------------------------VExceptionToolWasDeleted----------------------------------------------------
VExceptionToolWasDeleted::VExceptionToolWasDeleted(QString error)
    : VException{ std::move(error) }
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief raise method raise for exception
 */
// cppcheck-suppress unusedFunction
[[noreturn]] void VExceptionToolWasDeleted::raise() const { throw *this; }

//---------------------------------------------------------------------------------------------------------------------
VExceptionToolWasDeleted* VExceptionToolWasDeleted::clone() const
{
    return new VExceptionToolWasDeleted(*this);
}
