#include "Sensor.h"

Sensor::Sensor(double range, float rangeAngle, float placingAngle)
    : _range(range), _rangeAngle(rangeAngle), _placingAngle(placingAngle)
{
    _state = 0;
    _beams = 2 + 6 * (int) (_rangeAngle / M_PI);
}