/******************************************************************************
 *   @file   vtoolarc.h
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
 **  @file   vtoolarc.h
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

#ifndef VTOOLARC_H
#define VTOOLARC_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/varc.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"

class VFormula;

/**
 * @brief The VToolArc class tool for creation arc.
 */
class VToolArc : public VAbstractSpline
{
    Q_OBJECT
public:
    void setDialog() override;

    static VToolArc* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolArc* Create(
        const quint32 _id,
        const quint32& center,
        QString& radius,
        QString& f1,
        QString& f2,
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
        Type = UserType + static_cast<int>(Tool::Arc)
    };
    QString getTagName() const override;

    QString CenterPointName() const;

    quint32 getCenter() const;
    void setCenter(const quint32& value);

    VFormula GetFormulaRadius() const;
    void SetFormulaRadius(const VFormula& value);

    VFormula GetFormulaF1() const;
    void SetFormulaF1(const VFormula& value);

    VFormula GetFormulaF2() const;
    void SetFormulaF2(const VFormula& value);

    VArc getArc() const;
    void setArc(const VArc& arc);

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
    Q_DISABLE_COPY(VToolArc)

    VToolArc(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLARC_H
