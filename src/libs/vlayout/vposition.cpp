/***************************************************************************
 **  @file   vposition.cpp
 **  @author Douglas S Caskey
 **  @date   Dec 11, 2022
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
 **  along with Seamly2D. if not, see <http://www.gnu.org/licenses/>.
 **************************************************************************

 ************************************************************************
 **
 **  @file   vposition.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 1, 2015
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

#include "vposition.h"

#include "geometry/geometry.h"
#include "math/math.h"

#include <QDir>
#include <QImage>
#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPicture>
#include <QPointF>
#include <QPolygonF>
#include <QRect>
#include <QRectF>
#include <QSizeF>
#include <QStaticStringData>
#include <QString>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>

#include "../vmisc/def.h"


//---------------------------------------------------------------------------------------------------------------------
VPosition::VPosition(
    const VContour& gContour,
    int j,
    const VLayoutPiece& piece,
    int i,
    std::atomic_bool* stop,
    bool rotate,
    int rotationIncrease,
    bool saveLength)
    : QRunnable()
    , bestResult(VBestSquare(gContour.GetSize(), saveLength))
    , gContour(gContour)
    , piece(piece)
    , i(i)
    , j(j)
    , paperIndex(0)
    , frame(0)
    , piecesCount(0)
    , pieces()
    , stop(stop)
    , rotate(rotate)
    , rotationIncrease(rotationIncrease)
    , angle_between(0)
{
    if ((rotationIncrease >= 1 && rotationIncrease <= 180 && 360 % rotationIncrease == 0)
        == false) {
        this->rotationIncrease = 180;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::run()
{
    if (stop->load()) {
        return;
    }

    // We should use copy of the piece.
    VLayoutPiece workpiece = piece;

    int dEdge = i;   // For mirror piece edge will be different
    if (CheckCombineEdges(workpiece, j, dEdge)) {
#ifdef LAYOUT_DEBUG
#    ifdef SHOW_CANDIDATE_BEST
        DrawDebug(gContour, workpiece, frame + 2, paperIndex, piecesCount, pieces);
#    endif
#endif

        SaveCandidate(bestResult, workpiece, j, dEdge, BestFrom::Combine);
    }
    frame = frame + 3;

    if (rotate) {
        Rotate(rotationIncrease);
    } else {
        if (gContour.GetContour().isEmpty()) {
            Rotate(rotationIncrease);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getPaperIndex() const { return paperIndex; }

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setPaperIndex(const quint32& value) { paperIndex = value; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getFrame() const { return frame; }

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setFrame(const quint32& value) { frame = value; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VPosition::getPieceCount() const { return piecesCount; }

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setPieceCount(const quint32& value) { piecesCount = value; }

//---------------------------------------------------------------------------------------------------------------------
void VPosition::setPieces(const QVector<VLayoutPiece>& pieces) { this->pieces = pieces; }

//---------------------------------------------------------------------------------------------------------------------
VBestSquare VPosition::getBestResult() const { return bestResult; }

//---------------------------------------------------------------------------------------------------------------------
void VPosition::DrawDebug(
    const VContour& contour,
    const VLayoutPiece& piece,
    int frame,
    quint32 paperIndex,
    int piecesCount,
    const QVector<VLayoutPiece>& pieces)
{
    const int biasWidth = Bias(contour.GetWidth(), QIMAGE_MAX);
    const int biasHeight = Bias(contour.GetHeight(), QIMAGE_MAX);

    QPicture picture;
    QPainter paint;
    paint.begin(&picture);

    paint.setPen(QPen(Qt::black, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    QPainterPath path;
    if (contour.GetContour().isEmpty()) {
        path = DrawContour(contour.CutEmptySheetEdge());
        path.translate(biasWidth / 2, biasHeight / 2);
        paint.drawPath(path);
    } else {
        path = DrawContour(contour.GetContour());
        path.translate(biasWidth / 2, biasHeight / 2);
        paint.drawPath(path);
    }

#ifdef SHOW_CANDIDATE
    paint.setPen(QPen(Qt::darkGreen, 6, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    path = DrawContour(piece.getLayoutAllowancePoints());
    path.translate(biasWidth / 2, biasHeight / 2);
    paint.drawPath(path);
#else
    Q_UNUSED(piece)
    Q_UNUSED(pieces)
#endif

#ifdef ARRANGED_PIECES
    paint.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    path = drawPieces(pieces);
    path.translate(biasWidth / 2, biasHeight / 2);
    paint.drawPath(path);
#else
    Q_UNUSED(pieces)
#endif

    // Calculate bounding rect before draw sheet rect
    const QRect pictureRect = picture.boundingRect();

    // Sheet
#ifdef SHOW_SHEET
    paint.setPen(QPen(Qt::darkRed, 15, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    paint.drawRect(QRectF(biasWidth / 2, biasHeight / 2, contour.GetWidth(), contour.GetHeight()));
#endif

    paint.end();

    // Dump frame to image
    // Note. If program was build with Address Sanitizer possible crashes. Address Sanitizer doesn't
    // support big allocations.
    QImage frameImage(
        pictureRect.width() + biasWidth, pictureRect.height() + biasHeight, QImage::Format_RGB32);

    if (frameImage.isNull()) {
        return;
    }

    frameImage.fill(Qt::white);

    QPainter paintFrameImage;
    paintFrameImage.begin(&frameImage);
    paintFrameImage.drawPicture(0, 0, picture);
    paintFrameImage.end();

    const QString fileName = QDir::homePath() + QStringLiteral("/LayoutDebug/")
                           + QString("%1_%2_%3.png").arg(paperIndex).arg(piecesCount).arg(frame);
    frameImage.save(fileName);
}

//---------------------------------------------------------------------------------------------------------------------
int VPosition::Bias(int length, int maxLength)
{
    if (length < maxLength && length * 2 < maxLength) {
        return length;
    } else {
        return maxLength - length;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::SaveCandidate(
    VBestSquare& bestResult, const VLayoutPiece& piece, int globalI, int detJ, BestFrom type)
{
    QVector<QPointF> newGContour = gContour.UniteWithContour(piece, globalI, detJ, type);
    newGContour.append(newGContour.first());
    const QSizeF size = QPolygonF(newGContour).boundingRect().size();
    bestResult.NewResult(size, globalI, detJ, piece.getTransform(), piece.isMirror(), type);
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckCombineEdges(VLayoutPiece& piece, int j, int& dEdge)
{
    const QLineF globalEdge = gContour.GlobalEdge(j);
    bool flagMirror = false;
    bool flagSquare = false;

    CombineEdges(piece, globalEdge, dEdge);

#ifdef LAYOUT_DEBUG
#    ifdef SHOW_COMBINE
    DrawDebug(gContour, piece, frame, paperIndex, piecesCount, pieces);
#    endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(piece.pieceBoundingRect())) {
        if (not gContour.GetContour().isEmpty()) {
            type = Crossing(piece);
        } else {
            type = CrossingType::NoIntersection;
        }
    }

    switch (type) {
    case CrossingType::EdgeError: return false;
    case CrossingType::Intersection:
        piece.Mirror(globalEdge);
        flagMirror = true;
        break;
    case CrossingType::NoIntersection: flagSquare = true; break;
    }

    if (flagMirror && not piece.IsForbidFlipping()) {
#ifdef LAYOUT_DEBUG
#    ifdef SHOW_MIRROR
        DrawDebug(gContour, piece, frame + 1, paperIndex, piecesCount, pieces);
#    endif
#endif

        if (gContour.GetContour().isEmpty()) {
            dEdge = piece.pieceEdgeByPoint(globalEdge.p2());
        } else {
            dEdge = piece.LayoutEdgeByPoint(globalEdge.p2());
        }

        if (dEdge <= 0) {
            return false;
        }

        CrossingType type = CrossingType::Intersection;
        if (SheetContains(piece.pieceBoundingRect())) {
            type = Crossing(piece);
        }

        switch (type) {
        case CrossingType::EdgeError: return false;
        case CrossingType::Intersection: flagSquare = false; break;
        case CrossingType::NoIntersection: flagSquare = true; break;
        }
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::CheckRotationEdges(VLayoutPiece& piece, int j, int dEdge, int angle) const
{
    const QLineF globalEdge = gContour.GlobalEdge(j);
    bool flagSquare = false;

    RotateEdges(piece, globalEdge, dEdge, angle);

#ifdef LAYOUT_DEBUG
#    ifdef SHOW_ROTATION
    DrawDebug(gContour, piece, frame, paperIndex, piecesCount, pieces);
#    endif
#endif

    CrossingType type = CrossingType::Intersection;
    if (SheetContains(piece.pieceBoundingRect())) {
        type = Crossing(piece);
    }

    switch (type) {
    case CrossingType::EdgeError: return false;
    case CrossingType::Intersection: flagSquare = false; break;
    case CrossingType::NoIntersection: flagSquare = true; break;
    }
    return flagSquare;
}

//---------------------------------------------------------------------------------------------------------------------
VPosition::CrossingType VPosition::Crossing(const VLayoutPiece& piece) const
{
    const QRectF gRect = geo::boundingRect(gContour.GetContour());
    if (not gRect.intersects(piece.LayoutBoundingRect())
        && not gRect.contains(piece.pieceBoundingRect())) {
        // This we can determine efficiently.
        return CrossingType::NoIntersection;
    }

    const QPainterPath gPath = gContour.ContourPath();
    if (not gPath.intersects(piece.LayoutAllowancePath())
        && not gPath.contains(piece.createMainPath())) {
        return CrossingType::NoIntersection;
    } else {
        return CrossingType::Intersection;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPosition::SheetContains(const QRectF& rect) const
{
    const QRectF bRect(0, 0, gContour.GetWidth(), gContour.GetHeight());
    return bRect.contains(rect);
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::CombineEdges(VLayoutPiece& piece, const QLineF& globalEdge, const int& dEdge)
{
    QLineF pieceEdge;
    if (gContour.GetContour().isEmpty()) {
        pieceEdge = piece.pieceEdge(dEdge);
    } else {
        pieceEdge = piece.LayoutEdge(dEdge);
    }

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - pieceEdge.x2();
    const qreal dy = globalEdge.y2() - pieceEdge.y2();

    pieceEdge.translate(dx, dy);   // Use values for translate piece edge.

    angle_between = globalEdge.angleTo(pieceEdge);   // Seek angle between two edges.

    // Now we move piece to position near to global contour edge.
    piece.Translate(dx, dy);
    if (not qFuzzyIsNull(angle_between) || not qFuzzyCompare(angle_between, 360)) {
        piece.Rotate(pieceEdge.p2(), -angle_between);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::RotateEdges(
    VLayoutPiece& piece, const QLineF& globalEdge, int dEdge, int angle) const
{
    QLineF pieceEdge;
    if (gContour.GetContour().isEmpty()) {
        pieceEdge = piece.pieceEdge(dEdge);
    } else {
        pieceEdge = piece.LayoutEdge(dEdge);
    }

    // Find distance between two edges for two begin vertex.
    const qreal dx = globalEdge.x2() - pieceEdge.x2();
    const qreal dy = globalEdge.y2() - pieceEdge.y2();

    pieceEdge.translate(dx, dy);   // Use values for translate piece edge.

    // Now we move piece to position near to global contour edge.
    piece.Translate(dx, dy);
    piece.Rotate(globalEdge.p2(), angle);
}

//---------------------------------------------------------------------------------------------------------------------
void VPosition::Rotate(int increase)
{
    int startAngle = 0;
    if (math::isFuzzyEqual(angle_between, 360)) {
        startAngle = increase;
    }
    for (int angle = startAngle; angle < 360; angle = angle + increase) {
        if (stop->load()) {
            return;
        }

        // We should use copy of the piece.
        VLayoutPiece workpiece = piece;

        if (CheckRotationEdges(workpiece, j, i, angle)) {
#ifdef LAYOUT_DEBUG
#    ifdef SHOW_CANDIDATE_BEST
            ++frame;
            DrawDebug(gContour, workpiece, frame, paperIndex, piecesCount, pieces);
#    endif
#endif

            SaveCandidate(bestResult, workpiece, j, i, BestFrom::Rotation);
        }
        ++frame;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPosition::ShowDirection(const QLineF& edge)
{
    const int arrowLength = 14;
    QPainterPath path;
    if (edge.length() / arrowLength < 5) {
        return path;
    }

    QLineF arrow = edge;
    arrow.setLength(edge.length() / 2.0);

    // Reverse line because we want start arrow from this point
    arrow = QLineF(arrow.p2(), arrow.p1());
    const qreal angle =
        arrow.angle();   // we each time change line angle, better save original angle value
    arrow.setLength(arrowLength);   // arrow length in pixels

    arrow.setAngle(angle - 35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());

    arrow.setAngle(angle + 35);
    path.moveTo(arrow.p1());
    path.lineTo(arrow.p2());
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPosition::DrawContour(const QVector<QPointF>& points)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    if (points.count() >= 2) {
        for (qint32 i = 0; i < points.count() - 1; ++i) {
            path.moveTo(points.at(i));
            path.lineTo(points.at(i + 1));
        }
        path.lineTo(points.at(0));

#ifdef SHOW_DIRECTION
        for (qint32 i = 0; i < points.count() - 1; ++i) {
            path.addPath(ShowDirection(QLineF(points.at(i), points.at(i + 1))));
        }
#endif

#ifdef SHOW_VERTICES
        for (qint32 i = 0; i < points.count(); ++i) {
            path.addRect(points.at(i).x() - 3, points.at(i).y() - 3, 6, 6);
        }
#endif
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QPainterPath VPosition::drawPieces(const QVector<VLayoutPiece>& pieces)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    for (int i = 0; i < pieces.size(); ++i) {
        path.addPath(pieces.at(i).createMainPath());
    }

    return path;
}
