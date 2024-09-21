#ifndef VSAPOINT_H
#define VSAPOINT_H

#include "../vmisc/def.h"

#include <QPointF>
#include <QTypeInfo>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

/**
 * @brief The VSAPoint class seam allowance point
 */
class VSAPoint : public QPointF
{
public:
    constexpr VSAPoint() = default;
    constexpr VSAPoint(qreal xpos, qreal ypos);
    constexpr explicit VSAPoint(const QPointF& p);

    constexpr qreal GetSABefore() const;
    qreal GetSABefore(qreal width) const;
    void SetSABefore(qreal value);

    constexpr qreal GetSAAfter() const;
    qreal GetSAAfter(qreal width) const;
    void SetSAAfter(qreal value);

    constexpr PieceNodeAngle GetAngleType() const;
    void SetAngleType(PieceNodeAngle value);

private:
    qreal m_before{ -1.0 };
    qreal m_after{ -1.0 };
    PieceNodeAngle m_angle{ PieceNodeAngle::ByLength };
};

QT_WARNING_POP

Q_DECLARE_METATYPE(VSAPoint)
Q_DECLARE_TYPEINFO(VSAPoint, Q_MOVABLE_TYPE);

//---------------------------------------------------------------------------------------------------------------------
constexpr inline VSAPoint::VSAPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos)
{}

//---------------------------------------------------------------------------------------------------------------------
constexpr inline VSAPoint::VSAPoint(const QPointF& p)
    : QPointF(p)
{}

//---------------------------------------------------------------------------------------------------------------------
constexpr inline qreal VSAPoint::GetSABefore() const { return m_before; }

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSABefore(qreal value) { value < 0 ? m_before = -1 : m_before = value; }

//---------------------------------------------------------------------------------------------------------------------
constexpr inline qreal VSAPoint::GetSAAfter() const { return m_after; }

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetSAAfter(qreal value) { value < 0 ? m_after = -1 : m_after = value; }

//---------------------------------------------------------------------------------------------------------------------
constexpr inline PieceNodeAngle VSAPoint::GetAngleType() const { return m_angle; }

//---------------------------------------------------------------------------------------------------------------------
inline void VSAPoint::SetAngleType(PieceNodeAngle value) { m_angle = value; }

#endif   // VSAPOINT_H
