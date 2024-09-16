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
 **  @file   vcontainer.h
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

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <QCoreApplication>
#include <QHash>
#include <QMap>
#include <QMessageLogger>
#include <QSet>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QTypeInfo>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include <new>

#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/scassert.h"
#include "variables.h"
#include "variables/vinternalvariable.h"
#include "vpiece.h"
#include "vpiecepath.h"
#include "vtranslatevars.h"

class VEllipticalArc;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_INTEL(2021)
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VContainerData : public QSharedData   //-V690
{
public:
    VContainerData(const VTranslateVars* trVars, const Unit* patternUnit)
        : gObjects(QHash<quint32, QSharedPointer<VGObject>>())
        , variables(QHash<QString, QSharedPointer<VInternalVariable>>())
        , pieces(QSharedPointer<QHash<quint32, VPiece>>(new QHash<quint32, VPiece>()))
        , piecePaths(QSharedPointer<QHash<quint32, VPiecePath>>(new QHash<quint32, VPiecePath>()))
        , trVars(trVars)
        , patternUnit(patternUnit)
    {}

    VContainerData(const VContainerData& data)
        : QSharedData(data)
        , gObjects(data.gObjects)
        , variables(data.variables)
        , pieces(data.pieces)
        , piecePaths(data.piecePaths)
        , trVars(data.trVars)
        , patternUnit(data.patternUnit)
    {}

    virtual ~VContainerData();

    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<quint32, QSharedPointer<VGObject>> gObjects;

    /**
     * @brief variables container for measurements, custom variables, lines lengths, lines angles,
     * arcs lengths, curve lengths
     */
    QHash<QString, QSharedPointer<VInternalVariable>> variables;

    QSharedPointer<QHash<quint32, VPiece>> pieces;
    QSharedPointer<QHash<quint32, VPiecePath>> piecePaths;

    const VTranslateVars* trVars;
    const Unit* patternUnit;

private:
    VContainerData& operator=(const VContainerData&) = delete;
};

QT_WARNING_POP

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer
{
    Q_DECLARE_TR_FUNCTIONS(VContainer)
public:
    VContainer(const VTranslateVars* trVars, const Unit* patternUnit);
    VContainer(const VContainer& data);
    ~VContainer();

    VContainer& operator=(const VContainer& data);
    VContainer& operator=(VContainer&& data) noexcept;

    void Swap(VContainer& data) noexcept;

    template <typename T>
    QSharedPointer<T> GeometricObject(const quint32& id) const;
    QSharedPointer<VGObject> GetGObject(quint32 id) const;
    static QSharedPointer<VGObject> GetFakeGObject(quint32 id);
    VPiece GetPiece(quint32 id) const;
    VPiecePath GetPiecePath(quint32 id) const;
    template <typename T>
    QSharedPointer<T> getVariable(QString name) const;
    static quint32 getId();
    static quint32 getNextId();
    static void UpdateId(quint32 newId);

    quint32 AddGObject(VGObject* obj);
    quint32 AddPiece(const VPiece& piece);
    quint32 AddPiecePath(const VPiecePath& path);
    void AddLine(const quint32& firstPointId, const quint32& secondPointId);
    void AddArc(
        const QSharedPointer<VAbstractCurve>& arc,
        const quint32& id,
        const quint32& parentId = NULL_ID);
    void
    AddSpline(const QSharedPointer<VAbstractBezier>& curve, quint32 id, quint32 parentId = NULL_ID);
    void AddCurveWithSegments(
        const QSharedPointer<VAbstractCubicBezierPath>& curve,
        const quint32& id,
        quint32 parentId = NULL_ID);

    template <typename T>
    void AddVariable(const QString& name, T* var);
    template <typename T>
    void AddVariable(const QString& name, const QSharedPointer<T>& var);
    void RemoveVariable(const QString& name);
    void RemovePiece(quint32 id);

    template <class T>
    void UpdateGObject(quint32 id, T* obj);
    template <class T>
    void UpdateGObject(quint32 id, const QSharedPointer<T>& obj);

    void UpdatePiece(quint32 id, const VPiece& piece);
    void UpdatePiecePath(quint32 id, const VPiecePath& path);

    void Clear();
    void ClearForFullParse();
    void ClearGObjects();
    void ClearCalculationGObjects();
    void ClearVariables(const VarType& type = VarType::Unknown);
    static void ClearUniqueNames();
    static void clearUniqueVariableNames();

    static void setSize(qreal size);
    static void setHeight(qreal height);
    static qreal size();
    static qreal* rsize();
    static qreal height();
    static qreal* rheight();

    void removeCustomVariable(const QString& name);

    const QHash<quint32, QSharedPointer<VGObject>>* DataGObjects() const;
    const QHash<quint32, VPiece>* DataPieces() const;
    const QHash<QString, QSharedPointer<VInternalVariable>>* DataVariables() const;

    QMap<QString, QSharedPointer<MeasurementVariable>> DataMeasurements() const;
    QMap<QString, QSharedPointer<CustomVariable>> variablesData() const;
    QMap<QString, QSharedPointer<VLengthLine>> lineLengthsData() const;
    QMap<QString, QSharedPointer<VCurveLength>> curveLengthsData() const;
    QMap<QString, QSharedPointer<VCurveCLength>> controlPointLengthsData() const;
    QMap<QString, QSharedPointer<VLineAngle>> lineAnglesData() const;
    QMap<QString, QSharedPointer<VArcRadius>> arcRadiusesData() const;
    QMap<QString, QSharedPointer<VCurveAngle>> curveAnglesData() const;

    static bool IsUnique(const QString& name);
    static QStringList AllUniqueNames();

    const Unit* GetPatternUnit() const;
    const VTranslateVars* getTranslateVariables() const;

private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32 _id;
    static qreal _size;
    static qreal _height;
    static QSet<QString> uniqueNames;

    QSharedDataPointer<VContainerData> d;

    void AddCurve(
        const QSharedPointer<VAbstractCurve>& curve, const quint32& id, quint32 parentId = NULL_ID);

    template <class T>
    uint qHash(const QSharedPointer<T>& p);

    template <typename key, typename val>
    // cppcheck-suppress functionStatic
    val GetObject(const QHash<key, val>& obj, key id) const;

    template <typename T>
    void UpdateObject(const quint32& id, const QSharedPointer<T>& point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val>& obj, val value);

    template <typename T>
    QMap<QString, QSharedPointer<T>> DataVar(const VarType& type) const;
};

Q_DECLARE_TYPEINFO(VContainer, Q_MOVABLE_TYPE);

/*
 *  Defintion of templated member functions of VContainer
 */

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QSharedPointer<T> VContainer::GeometricObject(const quint32& id) const
{
    if (id == NULL_ID) {
        throw VExceptionBadId(tr("Can't find object"), id);
    }

    QSharedPointer<VGObject> gObj = QSharedPointer<VGObject>();
    if (d->gObjects.contains(id)) {
        gObj = d->gObjects.value(id);
    } else {
        throw VExceptionBadId(tr("Can't find object Id: "), id);
    }
    try {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(gObj);
        SCASSERT(obj.isNull() == false)
        return obj;
    } catch (const std::bad_alloc&) {
        throw VExceptionBadId(tr("Can't cast object"), id);
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getVariable return varible by name
 * @param name variable's name
 * @return variable
 */
template <typename T>
QSharedPointer<T> VContainer::getVariable(QString name) const
{
    SCASSERT(name.isEmpty() == false)
    if (d->variables.contains(name)) {
        try {
            QSharedPointer<T> value = qSharedPointerDynamicCast<T>(d->variables.value(name));
            SCASSERT(value.isNull() == false)
            return value;
        } catch (const std::bad_alloc&) {
            throw VExceptionBadId(tr("Can't cast object"), name);
        }
    } else {
        throw VExceptionBadId(tr("Can't find object"), name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(const QString& name, T* var)
{
    AddVariable(name, QSharedPointer<T>(var));
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(const QString& name, const QSharedPointer<T>& var)
{
    if (d->variables.contains(name)) {
        if (d->variables.value(name)->GetType() == var->GetType()) {
            QSharedPointer<T> v = qSharedPointerDynamicCast<T>(d->variables.value(name));
            if (v.isNull()) {
                throw VExceptionBadId(tr("Can't cast object."), name);
            }
            *v = *var;
        } else {
            throw VExceptionBadId(tr("Can't find object. Type mismatch."), name);
        }
    } else {
        d->variables.insert(name, var);
    }

    uniqueNames.insert(name);
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
uint VContainer::qHash(const QSharedPointer<T>& p)
{
    return qHash(p.data());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateGObject update GObject by id
 * @param id id of existing GObject
 * @param obj object
 */
template <class T>
void VContainer::UpdateGObject(quint32 id, T* obj)
{
    SCASSERT(obj != nullptr)
    UpdateGObject(id, QSharedPointer<T>(obj));
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
void VContainer::UpdateGObject(quint32 id, const QSharedPointer<T>& obj)
{
    SCASSERT(not obj.isNull())
    UpdateObject(id, obj);
    uniqueNames.insert(obj->name());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateObject update object in container
 * @param id id of existing object
 * @param point object
 */
template <typename T>
void VContainer::UpdateObject(const quint32& id, const QSharedPointer<T>& point)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0");   //-V654 //-V712
    SCASSERT(point.isNull() == false)
    point->setId(id);
    if (d->gObjects.contains(id)) {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(d->gObjects.value(id));
        if (obj.isNull()) {
            throw VExceptionBadId(tr("Can't cast object"), id);
        }
        *obj = *point;
    } else {
        d->gObjects.insert(id, point);
    }
    UpdateId(id);
}
#endif   // VCONTAINER_H
