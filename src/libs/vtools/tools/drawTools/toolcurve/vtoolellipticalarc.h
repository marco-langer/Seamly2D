/******************************************************************************
 *   @file   vtoolellipticalarc.h
 **  @author Douglas S Caskey
 **  @date   21 Mar, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program to create and model patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *****************************************************************************

 ************************************************************************
 **
 **  @file   vtoolellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   20 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Seamly2D project
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

#ifndef VTOOLELLIPTICALARC_H
#define VTOOLELLIPTICALARC_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"

class VFormula;

/**
 * @brief The VToolEllipticalArc class tool for creation elliptical arc.
 */
class VToolEllipticalArc : public VAbstractSpline
{
    Q_OBJECT
public:
    void setDialog() override;
    static VToolEllipticalArc* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolEllipticalArc* Create(
        const quint32 _id,
        const quint32& center,
        QString& radius1,
        QString& radius2,
        QString& f1,
        QString& f2,
        QString& rotationAngle,
        const QString& color,
        const QString& penStyle,
        const QString& lineWeight,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);
    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::EllipticalArc)
    };
    QString getTagName() const override;

    QString CenterPointName() const;

    quint32 getCenter() const;
    void setCenter(const quint32& value);

    VFormula GetFormulaRadius1() const;
    void SetFormulaRadius1(const VFormula& value);

    VFormula GetFormulaRadius2() const;
    void SetFormulaRadius2(const VFormula& value);

    VFormula GetFormulaF1() const;
    void SetFormulaF1(const VFormula& value);

    VFormula GetFormulaF2() const;
    void SetFormulaF2(const VFormula& value);

    VFormula GetFormulaRotationAngle() const;
    void SetFormulaRotationAngle(const VFormula& value);

    void ShowVisualization(bool show) override;


protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    void SetVisualization() override;
    QString makeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolEllipticalArc)
    VToolEllipticalArc(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLELLIPTICALARC_H
