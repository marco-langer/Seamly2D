/***************************************************************************
 **  @file   vcontainer.cpp
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
 **
 **  @file   vcontainer.cpp
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

#include "vcontainer.h"

#include <QVector>
#include <QtDebug>

#include <limits>

#include "../ifc/exception/vexception.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "core_utils/qhash_extensions.h"

#include "variables/custom_variable.h"
#include "variables/measurement_variable.h"
#include "variables/varcradius.h"
#include "variables/vcurveangle.h"
#include "variables/vcurveclength.h"
#include "variables/vcurvelength.h"
#include "variables/vlineangle.h"
#include "variables/vlinelength.h"
#include "variables/vvariable.h"
#include "vtranslatevars.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vCon, "v.container")

QT_WARNING_POP

quint32 VContainer::_id = NULL_ID;
qreal VContainer::_size = 50;
qreal VContainer::_height = 176;
QSet<QString> VContainer::uniqueNames = QSet<QString>();

VContainer::VContainer(const VTranslateVars* trVars, const Unit* patternUnit)
    : m_pieces{ new QHash<quint32, VPiece>() }
    , m_trVars{ trVars }
    , m_patternUnit{ patternUnit }
{
}

// cppcheck-suppress unusedFunction
const VGObject& VContainer::GetGObject(quint32 id) const
{
    auto iter{ m_gObjects.find(id) };
    if (iter != m_gObjects.end()) {
        return **iter;
    }

    throw VExceptionBadId(tr("Can't find object: "), id);
}


VGObject& VContainer::GetGObject(quint32 id)
{
    auto iter{ m_gObjects.find(id) };
    if (iter != m_gObjects.end()) {
        return **iter;
    }

    throw VExceptionBadId(tr("Can't find object: "), id);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<VGObject> VContainer::GetFakeGObject(quint32 id)
{
    auto obj{ std::make_unique<VGObject>() };
    obj->setId(id);
    return obj;
}


//---------------------------------------------------------------------------------------------------------------------
const VPiece& VContainer::GetPiece(quint32 id) const
{
    if (auto iter{ m_pieces->find(id) }; iter != m_pieces->end()) {
        return *iter;
    }

    throw VExceptionBadId(tr("Can't find piece: "), id);
}

//---------------------------------------------------------------------------------------------------------------------
const VPiecePath& VContainer::GetPiecePath(quint32 id) const
{
    if (auto iter{ m_piecePaths.find(id) }; iter != m_piecePaths.end()) {
        return *iter;
    }

    throw VExceptionBadId(tr("Can't find path: "), id);
}


quint32 VContainer::AddGObject(std::unique_ptr<VGObject> obj)
{
    SCASSERT(obj != nullptr)
    QSharedPointer<VGObject> pointer(obj.release());
    uniqueNames.insert(pointer->name());
    return AddObject(m_gObjects, pointer);
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::AddPiece(const VPiece& piece)
{
    const quint32 id = getNextId();
    m_pieces->insert(id, piece);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::AddPiecePath(const VPiecePath& path)
{
    const quint32 id = getNextId();
    m_piecePaths.insert(id, path);
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VContainer::getId() { return _id; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getNextId generate next unique id
 * @return next unique id
 */
quint32 VContainer::getNextId()
{
    // TODO. Current count of ids are very big and allow us save time before someone will reach its
    // max value. Better way, of cource, is to seek free ids inside the set of values and reuse
    // them. But for now better to keep it as it is now.
    if (_id == std::numeric_limits<unsigned int>::max()) {
        qCritical() << (tr("Number of free id exhausted."));
    }
    _id++;
    return _id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief UpdateId update id. If new id bigger when current save new like current.
 * @param newId id
 */
void VContainer::UpdateId(quint32 newId)
{
    if (newId > _id) {
        _id = newId;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear clear data in container. Id will be 0.
 */
void VContainer::Clear()
{
    qCDebug(vCon, "Clearing container data.");
    _id = NULL_ID;

    m_pieces->clear();
    m_piecePaths.clear();
    ClearVariables();
    ClearGObjects();
    ClearUniqueNames();
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearForFullParse()
{
    qCDebug(vCon, "Clearing container data for full parse.");
    _id = NULL_ID;

    m_pieces->clear();
    m_piecePaths.clear();
    ClearVariables(VarType::Variable);
    ClearVariables(VarType::LineAngle);
    ClearVariables(VarType::LineLength);
    ClearVariables(VarType::CurveLength);
    ClearVariables(VarType::CurveCLength);
    ClearVariables(VarType::ArcRadius);
    ClearVariables(VarType::CurveAngle);
    ClearGObjects();
    ClearUniqueNames();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClearObject points, splines, arcs, spline paths will be cleared.
 */
void VContainer::ClearGObjects() { m_gObjects.clear(); }

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearCalculationGObjects()
{
    erase_if(m_gObjects, [](const auto& keyAndValue) {
        return keyAndValue.second->getMode() == Draw::Calculation;
    });
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearVariables(const VarType& type)
{
    if (type == VarType::Unknown) {
        m_variables.clear();
    } else {
        erase_if(m_variables, [type](const auto& keyAndValue) {
            return keyAndValue.second->GetType() == type;
        });
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddLine add line to container
 * @param firstPointId id of first point of line
 * @param secondPointId id of second point of line
 */
void VContainer::AddLine(const quint32& firstPointId, const quint32& secondPointId)
{
    const auto& first{ *GeometricObject<VPointF>(firstPointId) };
    const auto& second{ *GeometricObject<VPointF>(secondPointId) };

    AddVariable(std::make_unique<VLengthLine>(
        first, firstPointId, second, secondPointId, *GetPatternUnit()));

    AddVariable(std::make_unique<VLineAngle>(first, firstPointId, second, secondPointId));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddArc(const VAbstractCurve& arc, const quint32& id, const quint32& parentId)
{
    AddCurve(arc, id, parentId);

    if (arc.getType() == GOType::Arc) {
        const auto& casted{ static_cast<const VArc&>(arc) };

        AddVariable(std::make_unique<VArcRadius>(id, parentId, casted, *GetPatternUnit()));
    } else if (arc.getType() == GOType::EllipticalArc) {
        const auto& casted{ static_cast<const VEllipticalArc&>(arc) };

        AddVariable(std::make_unique<VArcRadius>(id, parentId, casted, 1, *GetPatternUnit()));
        AddVariable(std::make_unique<VArcRadius>(id, parentId, casted, 2, *GetPatternUnit()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddCurve(const VAbstractCurve& curve, const quint32& id, quint32 parentId)
{
    const GOType curveType = curve.getType();
    if (curveType != GOType::Spline && curveType != GOType::SplinePath
        && curveType != GOType::CubicBezier && curveType != GOType::CubicBezierPath
        && curveType != GOType::Arc && curveType != GOType::EllipticalArc) {
        throw VException(
            tr("Can't create a curve with type '%1'").arg(static_cast<int>(curveType)));
    }

    AddVariable(std::make_unique<VCurveLength>(id, parentId, curve, *GetPatternUnit()));
    AddVariable(std::make_unique<VCurveAngle>(id, parentId, curve, CurveAngle::StartAngle));
    AddVariable(std::make_unique<VCurveAngle>(id, parentId, curve, CurveAngle::EndAngle));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddSpline(const VAbstractBezier& curve, quint32 id, quint32 parentId)
{
    AddCurve(curve, id, parentId);

    AddVariable(
        std::make_unique<VCurveCLength>(id, parentId, curve, CurveCLength::C1, *GetPatternUnit()));
    AddVariable(
        std::make_unique<VCurveCLength>(id, parentId, curve, CurveCLength::C2, *GetPatternUnit()));
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::AddCurveWithSegments(
    const VAbstractCubicBezierPath& curve, const quint32& id, quint32 parentId)
{
    AddSpline(curve, id, parentId);

    for (qint32 i = 1; i <= curve.CountSubSpl(); ++i) {
        const VSpline spl = curve.GetSpline(i);

        AddVariable(
            std::make_unique<VCurveLength>(id, parentId, curve.name(), spl, *GetPatternUnit(), i));
        AddVariable(std::make_unique<VCurveAngle>(
            id, parentId, curve.name(), spl, CurveAngle::StartAngle, i));
        AddVariable(std::make_unique<VCurveAngle>(
            id, parentId, curve.name(), spl, CurveAngle::EndAngle, i));
        AddVariable(std::make_unique<VCurveCLength>(
            id, parentId, curve.name(), spl, CurveCLength::C1, *GetPatternUnit(), i));
        AddVariable(std::make_unique<VCurveCLength>(
            id, parentId, curve.name(), spl, CurveCLength::C2, *GetPatternUnit(), i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::RemoveVariable(const QString& name) { m_variables.remove(name); }

//---------------------------------------------------------------------------------------------------------------------
void VContainer::RemovePiece(quint32 id) { m_pieces->remove(id); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddObject add object to container
 * @param obj container
 * @param value object
 * @return id of object in container
 */
template <typename key, typename val>
quint32 VContainer::AddObject(QHash<key, val>& obj, val value)
{
    SCASSERT(value != nullptr)
    const quint32 id = getNextId();
    value->setId(id);
    obj[id] = value;
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::UpdatePiece(quint32 id, const VPiece& piece)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0");   //-V654 //-V712
    m_pieces->insert(id, piece);
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::UpdatePiecePath(quint32 id, const VPiecePath& path)
{
    Q_ASSERT_X(id != NULL_ID, Q_FUNC_INFO, "id == 0");   //-V654 //-V712
    m_piecePaths.insert(id, path);
    UpdateId(id);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief removeCustomVariable remove increment by name from increment table
 * @param name name of existing increment
 */
void VContainer::removeCustomVariable(const QString& name) { m_variables.remove(name); }

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<MeasurementVariable>> VContainer::DataMeasurements() const
{
    return DataVar<MeasurementVariable>(VarType::Measurement);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<CustomVariable>> VContainer::variablesData() const
{
    return DataVar<CustomVariable>(VarType::Variable);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VLengthLine>> VContainer::lineLengthsData() const
{
    return DataVar<VLengthLine>(VarType::LineLength);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VCurveLength>> VContainer::curveLengthsData() const
{
    return DataVar<VCurveLength>(VarType::CurveLength);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VCurveCLength>> VContainer::controlPointLengthsData() const
{
    return DataVar<VCurveCLength>(VarType::CurveCLength);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VLineAngle>> VContainer::lineAnglesData() const
{
    return DataVar<VLineAngle>(VarType::LineAngle);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VArcRadius>> VContainer::arcRadiusesData() const
{
    return DataVar<VArcRadius>(VarType::ArcRadius);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<QString, QSharedPointer<VCurveAngle>> VContainer::curveAnglesData() const
{
    return DataVar<VCurveAngle>(VarType::CurveAngle);
}

//---------------------------------------------------------------------------------------------------------------------
bool VContainer::IsUnique(const QString& name)
{
    return (!uniqueNames.contains(name) && !builInFunctions.contains(name));
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VContainer::AllUniqueNames()
{
    QStringList names = builInFunctions;
    names.append(uniqueNames.values());
    return names;
}

//---------------------------------------------------------------------------------------------------------------------
const Unit* VContainer::GetPatternUnit() const { return m_patternUnit; }

//---------------------------------------------------------------------------------------------------------------------
const VTranslateVars* VContainer::getTranslateVariables() const { return m_trVars; }

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
QMap<QString, QSharedPointer<T>> VContainer::DataVar(const VarType& type) const
{
    QMap<QString, QSharedPointer<T>> map;
    // Sorting QHash by id
    QHash<QString, QSharedPointer<VInternalVariable>>::const_iterator i;
    for (i = m_variables.constBegin(); i != m_variables.constEnd(); ++i) {
        if (i.value()->GetType() == type) {
            QSharedPointer<T> var = getVariablePtr<T>(i.key());
            map.insert(m_trVars->VarToUser(i.key()), var);
        }
    }
    return map;
}

//---------------------------------------------------------------------------------------------------------------------
void VContainer::ClearUniqueNames() { uniqueNames.clear(); }

//---------------------------------------------------------------------------------------------------------------------
void VContainer::clearUniqueVariableNames()
{
    const QList<QString> list = uniqueNames.values();
    ClearUniqueNames();

    for (int i = 0; i < list.size(); ++i) {
        if (not list.at(i).startsWith('#')) {
            uniqueNames.insert(list.at(i));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetSize set value of size
 * @param size value of size
 */
void VContainer::setSize(qreal size) { _size = size; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetGrowth set value of growth
 * @param height value of height
 */
void VContainer::setHeight(qreal height) { _height = height; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief size return size
 * @return size in mm
 */
qreal VContainer::size() { return _size; }

//---------------------------------------------------------------------------------------------------------------------
qreal* VContainer::rsize() { return &_size; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief height return height
 * @return height in pattern units
 */
qreal VContainer::height() { return _height; }

//---------------------------------------------------------------------------------------------------------------------
qreal* VContainer::rheight() { return &_height; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief data container with datagObjects return container of gObjects
 * @return pointer on container of gObjects
 */
const QHash<quint32, QSharedPointer<VGObject>>& VContainer::DataGObjects() const
{
    return m_gObjects;
}

//---------------------------------------------------------------------------------------------------------------------
const QHash<quint32, VPiece>& VContainer::DataPieces() const { return *m_pieces.data(); }

//---------------------------------------------------------------------------------------------------------------------
const QHash<QString, QSharedPointer<VInternalVariable>>& VContainer::DataVariables() const
{
    return m_variables;
}
