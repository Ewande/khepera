#include "ProximitySensor.h"
#include "../Math/MathLib.h"
#include "../Entities/LinearEnt.h"
//#include <iostream>

void ProximitySensor::updateState(SimEntMap::const_iterator& firstEntity, SimEntMap::const_iterator& lastEntity)
{
    Point rangeBeg(_robot->getCenter());
    float sensorAngle = _robot->getDirectionAngle() - _placingAngle;
    rangeBeg.translate((_robot->getRadius()) * cos(sensorAngle), (_robot->getRadius()) * sin(sensorAngle));
    std::vector<Point> rangeEnds(_beams, Point(rangeBeg));
    for (int i = 0; i < _beams; i++)
        rangeEnds[i].translate(_range * cos(sensorAngle + _rangeAngle / 2 - i * _rangeAngle / (_beams - 1)),
        _range * sin(sensorAngle + _rangeAngle / 2 - i * _rangeAngle / (_beams - 1)));

    double minDetection = _range; // no detection
    for (SimEntMap::const_iterator it = firstEntity; it != lastEntity; it++)
    {
        if (it->second->getID() != _robot->getID())
        {
            for (int i = 0; i < _beams; i++)
            {
                int shape = it->second->getShapeID();
                if (shape == SimEnt::KHEPERA_ROBOT || shape == SimEnt::CIRCLE)
                {
                    CircularEnt* entity = dynamic_cast<CircularEnt*>(it->second);
                    Point& center = entity->getCenter();
                    double radius = entity->getRadius();

                    Point orth_proj = orthogonalProjection(center, rangeBeg, rangeEnds[i]);
                    double dist_from_line = orth_proj.getDistance(center);

                    if (dist_from_line == radius && orth_proj.isBetween(rangeBeg, rangeEnds[i]))
                        minDetection = min(minDetection, rangeBeg.getDistance(orth_proj));
                    else if (dist_from_line < EPS)
                        minDetection = min(minDetection, rangeBeg.getDistance(orth_proj) - radius);
                    else if (dist_from_line < radius)
                    {
                        double k = radius / dist_from_line;
                        float touchAngle = (float)acos(dist_from_line / radius);
                        Point projOnCircle(center.getX() + orth_proj.getXDiff(center) * k,
                            center.getY() + orth_proj.getYDiff(center) * k);
                        float directionAngle = (float)acos(projOnCircle.getXDiff(center) / radius)
                            * sign(projOnCircle.getYDiff(center));
                        Point left(center);
                        left.translate(radius * cos(directionAngle + touchAngle),
                            radius * sin(directionAngle + touchAngle));
                        Point right(center);
                        right.translate(radius * cos(directionAngle - touchAngle),
                            radius * sin(directionAngle - touchAngle));
                        if (left.isBetween(rangeBeg, rangeEnds[i]))
                            minDetection = min(minDetection, rangeBeg.getDistance(left));
                        if (right.isBetween(rangeBeg, rangeEnds[i]))
                            minDetection = min(minDetection, rangeBeg.getDistance(right));
                    }
                }
                else if (shape == SimEnt::LINE)
                {
                    LinearEnt* line = dynamic_cast<LinearEnt*>(it->second);

                    // check if ends of linear entity are between ends of current beam
                    double beg_cross = (line->getBeg() - rangeBeg).cross(rangeEnds[i] - rangeBeg);
                    double end_cross = (line->getEnd() - rangeBeg).cross(rangeEnds[i] - rangeBeg);
                    if (beg_cross && end_cross && sign(beg_cross) != sign(end_cross))
                    {
                        // check if ends of current beam are between ends of linear ent
                        double beg2_cross = (rangeBeg - line->getBeg()).cross(line->getEnd() - line->getBeg());
                        double end2_cross = (rangeEnds[i] - line->getBeg()).cross(line->getEnd() - line->getBeg());
                        if (beg2_cross && end2_cross && sign(beg2_cross) != sign(end2_cross))
                            minDetection = min(minDetection, _range * (end2_cross / (end2_cross - beg2_cross)));

                    }
                }
            }
        }
    }
    _state = (float)(1 - minDetection / _range);
    //std::cout << "minDet: " << minDetection << ", sensor state: " << _state << std::endl;
}