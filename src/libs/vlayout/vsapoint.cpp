#include "vsapoint.h"


qreal VSAPoint::GetSABefore(qreal width) const
{
    if (m_before < 0) {
        return width;
    }
    return m_before;
}


qreal VSAPoint::GetSAAfter(qreal width) const
{
    if (m_after < 0) {
        return width;
    }
    return m_after;
}
