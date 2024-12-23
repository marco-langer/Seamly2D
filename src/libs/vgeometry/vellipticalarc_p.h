#ifndef VELLIPTICALARC_P
#define VELLIPTICALARC_P

#include "../vmisc/vabstractapplication.h"
#include "vpointf.h"
#include <QSharedData>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VEllipticalArcData : public QSharedData
{
public:
    VEllipticalArcData() = default;
    VEllipticalArcData(
        qreal radius1,
        qreal radius2,
        const QString& formulaRadius1,
        const QString& formulaRadius2,
        qreal rotationAngle,
        const QString& formulaRotationAngle);
    VEllipticalArcData(qreal radius1, qreal radius2, qreal rotationAngle);
    VEllipticalArcData(const VEllipticalArcData& arc);

    virtual ~VEllipticalArcData();

    /** @brief radius1 elliptical arc major radius. */
    qreal radius1{ 0.0 };
    /** @brief radius2 elliptical arc minor radius. */
    qreal radius2{ 0.0 };
    /** @brief formulaRadius1 formula for elliptical arc major radius. */
    QString formulaRadius1;
    /** @brief formulaRadius2 formula for elliptical arc minor radius. */
    QString formulaRadius2;
    /** @brief rotationAngle in degree. */
    qreal rotationAngle{ 0.0 };
    /** @brief formulaRotationAngle formula for rotationAngle. */
    QString formulaRotationAngle;
    /** @brief m_transform for elliptical arc */
    QTransform m_transform;

private:
    VEllipticalArcData& operator=(const VEllipticalArcData&) = delete;
};

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(
    qreal radius1,
    qreal radius2,
    const QString& formulaRadius1,
    const QString& formulaRadius2,
    qreal rotationAngle,
    const QString& formulaRotationAngle)
    : radius1(radius1)
    , radius2(radius2)
    , formulaRadius1(formulaRadius1)
    , formulaRadius2(formulaRadius2)
    , rotationAngle(rotationAngle)
    , formulaRotationAngle(formulaRotationAngle)
    , m_transform()
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(qreal radius1, qreal radius2, qreal rotationAngle)
    : radius1(radius1)
    , radius2(radius2)
    , formulaRadius1(QString().number(qApp->fromPixel(radius1)))
    , formulaRadius2(QString().number(qApp->fromPixel(radius2)))
    , rotationAngle(rotationAngle)
    , formulaRotationAngle(QString().number(qApp->fromPixel(rotationAngle)))
    , m_transform()
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::VEllipticalArcData(const VEllipticalArcData& arc)
    : QSharedData(arc)
    , radius1(arc.radius1)
    , radius2(arc.radius2)
    , formulaRadius1(arc.formulaRadius1)
    , formulaRadius2(arc.formulaRadius2)
    , rotationAngle(arc.rotationAngle)
    , formulaRotationAngle(arc.formulaRotationAngle)
    , m_transform(arc.m_transform)
{}

//---------------------------------------------------------------------------------------------------------------------
VEllipticalArcData::~VEllipticalArcData() = default;

QT_WARNING_POP

#endif   // VELLIPTICALARC_P
