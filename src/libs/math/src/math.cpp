#include "math/math.h"

#include <QtGlobal>


namespace math {

bool isFuzzyEqual(double first, double second)
{
    if (qFuzzyIsNull(first)) {
        return qFuzzyIsNull(second);
    }

    // TODO this check was part of the original code but I don't think it is necessary
    if (qFuzzyIsNull(second)) {
        return false;
    }

    return qFuzzyCompare(first, second);
}

}   // namespace math
