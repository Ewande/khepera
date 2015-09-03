#include "Sensor.h"
#include <iostream>

Sensor::Sensor(double range, float rangeAngle, float placingAngle)
    : _range(range), _rangeAngle(rangeAngle), _placingAngle(placingAngle)
{
    _state = 0;
}

void Sensor::updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity)
{
    Point rangeBeg(_robot->getCenter());
    float sensorAngle = _robot->getDirectionAngle() - _placingAngle;
    rangeBeg.translate((_robot->getRadius() + 1) * cos(sensorAngle), (_robot->getRadius() + 1) * sin(sensorAngle));
    const int ends = 3;
    Point rangeEnds[3];
    for (int i = 0; i < ends; i++)
        rangeEnds[i].setCoords(rangeBeg);
    rangeEnds[0].translate(_range * cos(sensorAngle + _rangeAngle / 2), _range * sin(sensorAngle + _rangeAngle / 2));
    rangeEnds[1].translate(_range * cos(sensorAngle), _range * sin(sensorAngle));
    rangeEnds[2].translate(_range * cos(sensorAngle - _rangeAngle / 2), _range * sin(sensorAngle - _rangeAngle / 2));

    /*std::cout << "-START-\n";
    std::cout << "robot: " << _robot->getX() << ", " << _robot->getY() << std::endl;
    std::cout << "beg: " << rangeBeg.getX() << ", " << rangeBeg.getY() << std::endl;
    std::cout << "end0: " << rangeEnds[0].getX() << ", " << rangeEnds[0].getY() << std::endl;
    std::cout << "end1: " << rangeEnds[1].getX() << ", " << rangeEnds[1].getY() << std::endl;
    std::cout << "end2: " << rangeEnds[2].getX() << ", " << rangeEnds[2].getY() << std::endl;*/

    double minDetection = _range; // no detection
    for (SimEntMap::const_iterator it = firstEntity; it != lastEntity; it++)
    {
        //std::cout << "-------------------------------id: " << it->second->getID() << std::endl;
        for (int i = 0; i < ends; i++)
        {
            int shape = it->second->getShapeID();
            if (it->second != _robot && shape == SimEnt::KHEPERA_ROBOT || shape == SimEnt::CIRCLE)
            {
                //std::cout << "############ " << i << std::endl;
                double detection = _range;
                CircularEnt* entity = dynamic_cast<CircularEnt*>(it->second);
                Point& center = entity->getCenter();
                double radius = entity->getRadius();
                bool belongs;
                Point orth_proj = orthogonalProjection(center, rangeBeg, rangeEnds[i], &belongs);
                //std::cout << "orth_proj: " << orth_proj.getX() << ", " << orth_proj.getY() << std::endl;
                double dist_from_line = orth_proj.getDistance(center);
                //std::cout << "dist_to_line: " << dist_from_line << std::endl;
                if (dist_from_line == radius && orth_proj.isBetween(rangeBeg, rangeEnds[i]))
                    detection = rangeBeg.getDistance(orth_proj);
                else if (dist_from_line < EPS)
                    detection = rangeBeg.getDistance(orth_proj) - radius;
                else if (dist_from_line < radius)
                {
                    double k = radius / dist_from_line;
                    float touchAngle = acos(dist_from_line / radius);
                    //std::cout << "touchAngle: " << touchAngle << std::endl;
                    Point projOnCircle(center.getX() + orth_proj.getXDiff(center) * k,
                        center.getY() + orth_proj.getYDiff(center) * k);
                    //std::cout << "projOnCircle: " << projOnCircle.getX() << ", " << projOnCircle.getY() << std::endl;
                    float directionAngle = acos(projOnCircle.getXDiff(center) / radius) 
                        * sign(projOnCircle.getYDiff(center));
                    //std::cout << "cos: " << projOnCircle.getXDiff(center) / radius << std::endl;
                    //std::cout << "cos angle: " << directionAngle * 180.0 / M_PI << std::endl;
                    //std::cout << "sin: " << projOnCircle.getYDiff(center) / radius << std::endl;
                    //std::cout << "sin angle: " << asin(projOnCircle.getYDiff(center) / radius) * 180.0 / M_PI << std::endl;
                    //std::cout << "projAgain: " << projAgain.getX() << ", " << projAgain.getY() << std::endl;
                    Point left(center);
                    left.translate(radius * cos(directionAngle + touchAngle), radius * sin(directionAngle + touchAngle));
                    Point right(center);
                    right.translate(radius * cos(directionAngle - touchAngle), radius * sin(directionAngle - touchAngle));
                    //std::cout << "left: " << left.getX() << ", " << left.getY() << std::endl;
                    //std::cout << "right: " << right.getX() << ", " << right.getY() << std::endl;
                    //std::cout << rangeBeg.getDistance(projOnCircle) << " :: " <<
                    //    rangeBeg.getDistance(left) << " :: " << rangeBeg.getDistance(right) << std::endl;
                    if (left.isBetween(rangeBeg, rangeEnds[i]))
                        detection = min(detection, rangeBeg.getDistance(left));
                    if (right.isBetween(rangeBeg, rangeEnds[i]))
                        detection = min(detection, rangeBeg.getDistance(right));
                }
                //std::cout << "detection: " << detection << std::endl;
                minDetection = min(minDetection, detection);
            }
        }
    }
    _state = 1 - minDetection / _range;
    //std::cout << "minDet: " << minDetection << ", sensor state: " << _state << std::endl;
}