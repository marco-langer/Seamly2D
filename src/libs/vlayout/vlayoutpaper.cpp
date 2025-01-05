/***************************************************************************
 **  @file   vlayoutpaper.cpp
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
 **  @file   vlayoutpaper.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#include "vlayoutpaper.h"

#include <QBrush>
#include <QCoreApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QList>
#include <QPen>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QThread>
#include <QThreadPool>
#include <QVector>
#include <Qt>
#include <QtAlgorithms>

#include <limits>

#include "vbestsquare.h"
#include "vcontour.h"
#include "vlayoutpiece.h"
#include "vposition.h"


VLayoutPaper::VLayoutPaper(int height, int width)
    : m_globalContour(VContour{ height, width })
{
}


//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetHeight() const { return m_globalContour.GetHeight(); }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::setHeight(int height) { m_globalContour.setHeight(height); }

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetWidth() const { return m_globalContour.GetWidth(); }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetWidth(int width) { m_globalContour.SetWidth(width); }

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutPaper::GetLayoutWidth() const { return m_layoutWidth; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetLayoutWidth(qreal width)
{
    if (width >= 0) {
        m_layoutWidth = width;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VLayoutPaper::GetShift() const { return m_globalContour.GetShift(); }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetShift(quint32 shift) { m_globalContour.SetShift(shift); }

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::GetRotate() const { return m_globalRotate; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotate(bool value)
{
    m_globalRotate = value;
    m_localRotate = m_globalRotate;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::GetRotationIncrease() const { return m_globalRotationIncrease; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetRotationIncrease(int value)
{
    m_globalRotationIncrease = value;

    if ((m_globalRotationIncrease >= 1 && m_globalRotationIncrease <= 180
         && 360 % m_globalRotationIncrease == 0)
        == false) {
        m_globalRotationIncrease = 180;
    }

    m_localRotationIncrease = m_globalRotationIncrease;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::IsSaveLength() const { return m_saveLength; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetSaveLength(bool value) { m_saveLength = value; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::SetPaperIndex(quint32 index) { m_paperIndex = index; }

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::arrangePiece(const VLayoutPiece& piece, std::atomic_bool& stop)
{
    // First need set size of paper
    if (m_globalContour.GetHeight() <= 0 || m_globalContour.GetWidth() <= 0) {
        return false;
    }

    if (piece.LayoutEdgesCount() < 3 || piece.pieceEdgesCount() < 3) {
        return false;   // Not enough edges
    }

    if (piece.IsForbidFlipping() && not m_globalRotate) {   // Compensate forbidden flipping by
                                                            // rotating. 180 degree will be enough.
        m_localRotate = true;
        m_localRotationIncrease = 180;
    } else {   // Return to global values if was changed
        m_localRotate = m_globalRotate;
        m_localRotationIncrease = m_globalRotationIncrease;
    }

    m_frame = 0;

    return AddToSheet(piece, stop);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutPaper::Count() const { return m_pieces.count(); }

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::AddToSheet(const VLayoutPiece& piece, std::atomic_bool& stop)
{
    VBestSquare bestResult(m_globalContour.GetSize(), m_saveLength);
    QThreadPool* thread_pool = QThreadPool::globalInstance();
    thread_pool->setExpiryTimeout(1000);
    QVector<VPosition*> threads;

    int pieceEdgesCount = 0;

    if (m_globalContour.GetContour().isEmpty()) {
        pieceEdgesCount = piece.pieceEdgesCount();
    } else {
        pieceEdgesCount = piece.LayoutEdgesCount();
    }

    for (int j = 1; j <= m_globalContour.GlobalEdgesCount(); ++j) {
        for (int i = 1; i <= pieceEdgesCount; ++i) {
            VPosition* thread = new VPosition(
                m_globalContour,
                j,
                piece,
                i,
                &stop,
                m_localRotate,
                m_localRotationIncrease,
                m_saveLength);
// Info for debug
#ifdef LAYOUT_DEBUG
            thread->setPaperIndex(d->paperIndex);
            thread->setFrame(d->frame);
            thread->setPieceCount(d->pieces.count());
            thread->setPieces(d->pieces);
#endif

            thread->setAutoDelete(false);
            threads.append(thread);
            thread_pool->start(thread);

            m_frame = m_frame + 3 + static_cast<quint32>(360 / m_localRotationIncrease * 2);
        }
    }

    // Wait for done
    do {
        QCoreApplication::processEvents();
        QThread::msleep(250);
    } while (thread_pool->activeThreadCount() > 0 && not stop.load());

    if (stop.load()) {
        qDeleteAll(threads.begin(), threads.end());
        threads.clear();
        return false;
    }

    for (int i = 0; i < threads.size(); ++i) {
        bestResult.NewResult(threads.at(i)->getBestResult());
    }

    qDeleteAll(threads.begin(), threads.end());
    threads.clear();

    return SaveResult(bestResult, piece);
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPaper::SaveResult(const VBestSquare& bestResult, const VLayoutPiece& piece)
{
    if (bestResult.ValidResult()) {
        VLayoutPiece workDetail = piece;
        workDetail.setTransform(bestResult.Transform());   // Don't forget set transform
        workDetail.SetMirror(bestResult.isMirror());
        const QVector<QPointF> newGContour = m_globalContour.UniteWithContour(
            workDetail, bestResult.GContourEdge(), bestResult.pieceEdge(), bestResult.Type());
        if (newGContour.isEmpty()) {
            return false;
        }
        m_pieces.append(workDetail);
        m_globalContour.SetContour(newGContour);

#ifdef LAYOUT_DEBUG
#    ifdef SHOW_BEST
        VPosition::DrawDebug(
            d->globalContour,
            workDetail,
            std::numeric_limits<unsigned int>::max(),
            d->paperIndex,
            d->pieces.count(),
            d->pieces);
#    endif
#endif
    }

    return bestResult.ValidResult();   // Do we have the best result?
}

//---------------------------------------------------------------------------------------------------------------------
QGraphicsRectItem* VLayoutPaper::GetPaperItem(bool autoCrop, bool textAsPaths) const
{
    QGraphicsRectItem* paper;
    if (autoCrop) {
        QGraphicsScene scene;
        QList<QGraphicsItem*> list = getPieceItems(textAsPaths);
        for (int i = 0; i < list.size(); ++i) {
            scene.addItem(list.at(i));
        }
        const int height = scene.itemsBoundingRect().toRect().height();
        if (m_globalContour.GetHeight() > height)   //-V807
        {
            paper = new QGraphicsRectItem(QRectF(0, 0, m_globalContour.GetWidth(), height));
        } else {
            paper = new QGraphicsRectItem(
                QRectF(0, 0, m_globalContour.GetWidth(), m_globalContour.GetHeight()));
        }
    } else {
        paper = new QGraphicsRectItem(
            QRectF(0, 0, m_globalContour.GetWidth(), m_globalContour.GetHeight()));
    }
    paper->setPen(QPen(Qt::black, 1));
    paper->setBrush(QBrush(Qt::white));
    return paper;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem*> VLayoutPaper::getPieceItems(bool textAsPaths) const
{
    QList<QGraphicsItem*> list;
    for (int i = 0; i < m_pieces.count(); ++i) {
        list.append(m_pieces.at(i).GetItem(textAsPaths));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
const QVector<VLayoutPiece>& VLayoutPaper::getPieces() const { return m_pieces; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPaper::setPieces(const QList<VLayoutPiece>& pieces) { m_pieces = pieces.toVector(); }

//---------------------------------------------------------------------------------------------------------------------
QRectF VLayoutPaper::piecesBoundingRect() const
{
    QRectF rec;
    for (int i = 0; i < m_pieces.count(); ++i) {
        rec = rec.united(m_pieces.at(i).pieceBoundingRect());
    }

    return rec;
}
