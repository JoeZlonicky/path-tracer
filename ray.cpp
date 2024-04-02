#include "ray.h"

Point3 Ray::at(double t) const
{
    return _origin + _direction * t;
}
