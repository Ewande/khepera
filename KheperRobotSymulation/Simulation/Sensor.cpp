#include "Sensor.h"

Sensor::Sensor(double minRange, double maxRange, float rangeAngle, float placingAngle)
    : _minRange(minRange), _maxRange(maxRange), _rangeAngle(rangeAngle), _placingAngle(placingAngle)
{

}

void Sensor::updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity)
{
    for (SimEntMap::const_iterator it = firstEntity; it != lastEntity; it++)
    {

    }
}