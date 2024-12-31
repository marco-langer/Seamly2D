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
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QTypeInfo>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include <memory>
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

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer
{
    Q_DECLARE_TR_FUNCTIONS(VContainer)

public:
    VContainer(const VTranslateVars* trVars, const Unit* patternUnit);

    template <typename T>
    QSharedPointer<T> GeometricObject(const quint32& id) const;
    const VGObject& GetGObject(quint32 id) const;
    VGObject& GetGObject(quint32 id);
    static std::unique_ptr<VGObject> GetFakeGObject(quint32 id);
    const VPiece& GetPiece(quint32 id) const;
    const VPiecePath& GetPiecePath(quint32 id) const;
    template <typename T>
    const T& getVariable(const QString& name) const;
    template <typename T>
    T& getVariable(const QString& name);
    static quint32 getId();
    static quint32 getNextId();
    static void UpdateId(quint32 newId);

    quint32 AddGObject(std::unique_ptr<VGObject> obj);
    quint32 AddPiece(const VPiece& piece);
    quint32 AddPiecePath(const VPiecePath& path);
    void AddLine(const quint32& firstPointId, const quint32& secondPointId);
    void AddArc(const VAbstractCurve& arc, const quint32& id, const quint32& parentId = NULL_ID);
    void AddSpline(const VAbstractBezier& curve, quint32 id, quint32 parentId = NULL_ID);
    void AddCurveWithSegments(
        const VAbstractCubicBezierPath& curve, const quint32& id, quint32 parentId = NULL_ID);

    template <typename T>
    void AddVariable(std::unique_ptr<T> var);

    void RemoveVariable(const QString& name);
    void RemovePiece(quint32 id);

    template <class T>
    void UpdateGObject(quint32 id, std::unique_ptr<T> obj);

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

    const QHash<quint32, QSharedPointer<VGObject>>& DataGObjects() const;
    const QHash<quint32, VPiece>& DataPieces() const;
    const QHash<QString, QSharedPointer<VInternalVariable>>& DataVariables() const;

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
    template <class T>
    void UpdateGObject(quint32 id, const QSharedPointer<T>& obj);

    template <typename T>
    void AddVariable(const QString& name, const QSharedPointer<T>& var);

    template <typename T>
    QSharedPointer<T> getVariablePtr(const QString& name) const;

    void AddCurve(const VAbstractCurve& curve, const quint32& id, quint32 parentId = NULL_ID);

    template <class T>
    uint qHash(const QSharedPointer<T>& p);

    template <typename T>
    void UpdateObject(const quint32& id, const QSharedPointer<T>& point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val>& obj, val value);

    template <typename T>
    QMap<QString, QSharedPointer<T>> DataVar(const VarType& type) const;

    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32 _id;
    static qreal _size;
    static qreal _height;
    static QSet<QString> uniqueNames;

    QHash<quint32, QSharedPointer<VGObject>> m_gObjects;
    QHash<QString, QSharedPointer<VInternalVariable>> m_variables;
    QSharedPointer<QHash<quint32, VPiece>>
        m_pieces;   // TODO some callers store the pointer to this variable. Fix that first before
                    // removing this shared pointer
    QHash<quint32, VPiecePath> m_piecePaths;
    const VTranslateVars* m_trVars;
    const Unit* m_patternUnit;
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QSharedPointer<T> VContainer::GeometricObject(const quint32& id) const
{
    if (id == NULL_ID) {
        throw VExceptionBadId(tr("Can't find object"), id);
    }

    QSharedPointer<VGObject> gObj = QSharedPointer<VGObject>();

    if (auto iter{ m_gObjects.find(id) }; iter != m_gObjects.end()) {
        gObj = *iter;
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

template <typename T>
const T& VContainer::getVariable(const QString& name) const
{
    return *getVariablePtr<T>(name);
}

template <typename T>
T& VContainer::getVariable(const QString& name)
{
    return *getVariablePtr<T>(name);
}

template <typename T>
QSharedPointer<T> VContainer::getVariablePtr(const QString& name) const
{
    SCASSERT(name.isEmpty() == false)

    if (auto iter{ m_variables.find(name) }; iter != m_variables.end()) {
        try {
            QSharedPointer<T> value = qSharedPointerDynamicCast<T>(*iter);
            SCASSERT(value.isNull() == false)
            return value;
        } catch (const std::bad_alloc&) {
            throw VExceptionBadId(tr("Can't cast object"), name);
        }
    } else {
        throw VExceptionBadId(tr("Can't find object"), name);
    }
}

template <typename T>
void VContainer::AddVariable(std::unique_ptr<T> var)
{
    const QString& name{ var->GetName() };
    AddVariable(name, QSharedPointer<T>(var.release()));
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(const QString& name, const QSharedPointer<T>& var)
{
    if (auto iter{ m_variables.find(name) }; iter != m_variables.end()) {
        if ((*iter)->GetType() == var->GetType()) {
            QSharedPointer<T> v = qSharedPointerDynamicCast<T>(*iter);
            if (v.isNull()) {
                throw VExceptionBadId(tr("Can't cast object."), name);
            }
            *v = *var;
        } else {
            throw VExceptionBadId(tr("Can't find object. Type mismatch."), name);
        }
    } else {
        m_variables.insert(name, var);
    }

    uniqueNames.insert(name);
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
uint VContainer::qHash(const QSharedPointer<T>& p)
{
    return qHash(p.data());
}

template <class T>
void VContainer::UpdateGObject(quint32 id, std::unique_ptr<T> obj)
{
    SCASSERT(obj != nullptr)
    UpdateGObject(id, QSharedPointer<T>(obj.release()));
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

    if (auto iter{ m_gObjects.find(id) }; iter != m_gObjects.end()) {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(*iter);
        if (obj.isNull()) {
            throw VExceptionBadId(tr("Can't cast object"), id);
        }
        *obj = *point;
    } else {
        m_gObjects.insert(id, point);
    }
    UpdateId(id);
}
#endif   // VCONTAINER_H
