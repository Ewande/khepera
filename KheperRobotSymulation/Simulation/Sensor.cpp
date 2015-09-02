#include "Sensor.h"

Sensor::Sensor(double range, float rangeAngle, float placingAngle)
    : _range(range), _rangeAngle(rangeAngle), _placingAngle(placingAngle)
{
    _state = 0;
}

void Sensor::updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity)
{
    Point rangeBeg(_robot->getCenter());
    float sensorAngle = _placingAngle - _robot->getDirectionAngle();
    rangeBeg.translate((_robot->getRadius() + 1) * cos(sensorAngle), (_robot->getRadius() + 1) * sin(sensorAngle));
    const int ends = 3;
    Point rangeEnds[3];
    for (int i = 0; i < ends; i++)
        rangeEnds[i].setCoords(rangeBeg);
    rangeEnds[0].translate(_range * cos(sensorAngle - _rangeAngle / 2), _range * sin(sensorAngle - _rangeAngle / 2));
    rangeEnds[1].translate(_range * cos(sensorAngle), _range * sin(sensorAngle));
    rangeEnds[2].translate(_range * cos(sensorAngle + _rangeAngle / 2), _range * sin(sensorAngle + _rangeAngle / 2));

    for (SimEntMap::const_iterator it = firstEntity; it != lastEntity; it++)
    {
        // detecting entities
    }
}