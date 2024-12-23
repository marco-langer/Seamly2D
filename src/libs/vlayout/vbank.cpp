/***************************************************************************
 **  @file   vbank.cpp
 **  @author Douglas S Caskey
 **  @date   Dec 27, 2022
 **
 **  @copyright
 **  Copyright (C) 2017 - 2022 Seamly, LLC
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
 **
 **  @file   vbank.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#include "vbank.h"

#include <limits>

#include "../vmisc/logging.h"
#include "vlayoutpiece.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(lBank, "layout.bank")

QT_WARNING_POP

// An annoying char define, from the Windows team in <rpcndr.h>
// #define small char
// http://stuartjames.info/Journal/c--visual-studio-2012-vs2012--win8--converting-projects-up-some-conflicts-i-found.aspx
#if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
#    pragma push_macro("small")
#    undef small
#endif

//---------------------------------------------------------------------------------------------------------------------
qreal VBank::GetLayoutWidth() const { return layoutWidth; }

//---------------------------------------------------------------------------------------------------------------------
void VBank::SetLayoutWidth(const qreal& value)
{
    layoutWidth = value;
    Reset();
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::setPieces(const QVector<VLayoutPiece>& pieces)
{
    this->pieces = pieces;
    Reset();
}

//---------------------------------------------------------------------------------------------------------------------
int VBank::GetTiket()
{
    if (prepare == false) {
        return -1;
    }

    if (LeftArrange() == 0) {
        if (unsorted.isEmpty()) {
            return -1;
        } else {
            PrepareGroup();
        }
    }

    switch (caseType) {
    case Cases::CaseThreeGroup: return GetNextThreeGroups();
    case Cases::CaseTwoGroup: return GetNextTwoGroups();
    case Cases::CaseDesc: return GetNextDescGroup();
    default: return -1;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiece VBank::getPiece(int i) const
{
    if (i >= 0 && i < pieces.size()) {
        return pieces.at(i);
    } else {
        return VLayoutPiece();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::Arranged(int i)
{
    if (big.contains(i)) {
        big.remove(i);
        return;
    }

    if (middle.contains(i)) {
        middle.remove(i);
        return;
    }

    if (small.contains(i)) {
        small.remove(i);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::NotArranged(int i)
{
    if (big.contains(i)) {
        unsorted.insert(i, big.value(i));
        big.remove(i);
        return;
    }

    if (middle.contains(i)) {
        unsorted.insert(i, middle.value(i));
        middle.remove(i);
        return;
    }

    if (small.contains(i)) {
        unsorted.insert(i, small.value(i));
        small.remove(i);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VBank::Prepare()
{
    if (layoutWidth <= 0) {
        qCDebug(lBank, "Preparing data for layout error: Layout paper sheet <= 0");
        prepare = false;
        return prepare;
    }

    if (pieces.isEmpty()) {
        qCDebug(lBank, "Preparing data for layout error: List of pieces is empty");
        prepare = false;
        return prepare;
    }

    diagonal = 0;
    for (int i = 0; i < pieces.size(); ++i) {
        pieces[i].SetLayoutWidth(layoutWidth);
        pieces[i].SetLayoutAllowancePoints();

        const qreal d = pieces.at(i).Diagonal();
        if (d > diagonal) {
            diagonal = d;
        }

        const qint64 square = pieces.at(i).Square();
        if (square <= 0) {
            qCDebug(lBank, "Preparing data for layout error: Detail squere <= 0");
            prepare = false;
            return prepare;
        }
        unsorted.insert(i, square);
    }

    PrepareGroup();

    prepare = true;
    return prepare;
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::Reset()
{
    prepare = false;
    unsorted.clear();
    big.clear();
    middle.clear();
    small.clear();
    diagonal = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::SetCaseType(Cases caseType) { this->caseType = caseType; }

//---------------------------------------------------------------------------------------------------------------------
int VBank::allPieceCount() const
{
    return unsorted.count() + big.count() + middle.count() + small.count();
}

//---------------------------------------------------------------------------------------------------------------------
int VBank::LeftArrange() const { return big.count() + middle.count() + small.count(); }

//---------------------------------------------------------------------------------------------------------------------
qreal VBank::GetBiggestDiagonal() const { return diagonal; }

//---------------------------------------------------------------------------------------------------------------------
int VBank::ArrangedCount() const { return pieces.size() - allPieceCount(); }

//---------------------------------------------------------------------------------------------------------------------
void VBank::PrepareGroup()
{
    switch (caseType) {
    case Cases::CaseThreeGroup: PrepareThreeGroups(); break;
    case Cases::CaseTwoGroup: PrepareTwoGroups(); break;
    case Cases::CaseDesc: PrepareDescGroup(); break;
    default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::PrepareThreeGroups()
{
    qint64 sMax = std::numeric_limits<qint64>::min();
    qint64 sMin = std::numeric_limits<qint64>::max();

    SqMaxMin(sMax, sMin);

    const qint64 s1 = sMax - (sMax - sMin) / 3;
    const qint64 s2 = sMin + (sMax - sMin) / 3;

    QHash<int, qint64>::const_iterator i = unsorted.constBegin();
    while (i != unsorted.constEnd()) {
        if (i.value() > s1) {
            big.insert(i.key(), i.value());
        } else if (s1 >= i.value() && i.value() > s2) {
            middle.insert(i.key(), i.value());
        } else {
            small.insert(i.key(), i.value());
        }
        ++i;
    }
    unsorted.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::PrepareTwoGroups()
{
    qint64 sMax = std::numeric_limits<qint64>::min();
    qint64 sMin = std::numeric_limits<qint64>::max();

    SqMaxMin(sMax, sMin);

    const qint64 s = (sMax + sMin) / 2;
    QHash<int, qint64>::const_iterator i = unsorted.constBegin();
    while (i != unsorted.constEnd()) {
        if (i.value() >= s) {
            big.insert(i.key(), i.value());
        } else {
            small.insert(i.key(), i.value());
        }
        ++i;
    }
    unsorted.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::PrepareDescGroup()
{
    big = unsorted;
    unsorted.clear();
}

//---------------------------------------------------------------------------------------------------------------------
int VBank::GetNextThreeGroups() const
{
    if (big.isEmpty() == false) {
        QHash<int, qint64>::const_iterator i = big.constBegin();
        return i.key();
    }

    if (middle.isEmpty() == false) {
        QHash<int, qint64>::const_iterator i = middle.constBegin();
        return i.key();
    }

    if (small.isEmpty() == false) {
        QHash<int, qint64>::const_iterator i = small.constBegin();
        return i.key();
    }

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
int VBank::GetNextTwoGroups() const
{
    if (big.isEmpty() == false) {
        QHash<int, qint64>::const_iterator i = big.constBegin();
        return i.key();
    }

    if (small.isEmpty() == false) {
        QHash<int, qint64>::const_iterator i = small.constBegin();
        return i.key();
    }

    return -1;
}

//---------------------------------------------------------------------------------------------------------------------
int VBank::GetNextDescGroup() const
{
    int index = -1;
    qint64 sMax = std::numeric_limits<qint64>::min();

    QHash<int, qint64>::const_iterator i = big.constBegin();
    while (i != big.constEnd()) {
        if (i.value() > sMax) {
            sMax = i.value();
            index = i.key();
        }

        ++i;
    }

    return index;
}

//---------------------------------------------------------------------------------------------------------------------
void VBank::SqMaxMin(qint64& sMax, qint64& sMin) const
{
    sMax = std::numeric_limits<qint64>::min();
    sMin = std::numeric_limits<qint64>::max();

    QHash<int, qint64>::const_iterator i = unsorted.constBegin();
    while (i != unsorted.constEnd()) {
        if (i.value() < sMin) {
            sMin = i.value();
        }

        if (i.value() > sMax) {
            sMax = i.value();
        }
        ++i;
    }
}

#if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
#    pragma pop_macro("small")
#endif
