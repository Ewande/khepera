#include "ProximitySensor.h"
#include "../Math/MathLib.h"

void ProximitySensor::updateState(const SimEntMap::const_iterator& firstEntity, 
    const SimEntMap::const_iterator& lastEntity)
{
    Point rangeBeg(_robot->getCenter());
    float sensorAngle = _robot->getDirectionAngle() - _placingAngle;
    rangeBeg.translate(_robot->getRadius() * cos(sensorAngle), _robot->getRadius() * sin(sensorAngle));
    std::vector<Point> rangeEnds(_beams, Point(rangeBeg));
    for (int i = 0; i < _beams; i++)
    {
        double angle = sensorAngle + _rangeAngle / 2 - i * _rangeAngle / (_beams - 1);
        rangeEnds[i].translate(_range * cos(angle), _range * sin(angle));
    }

    double minDetection = _range; // no detection
    for (SimEntMap::const_iterator it = firstEntity; it != lastEntity; it++)
    {
        if (it->second->getID() != _robot->getID())
        {
            int shape = it->second->getShapeID();
            for (int i = 0; i < _beams; i++)
            {
                if (shape == SimEnt::KHEPERA_ROBOT || shape == SimEnt::CIRCLE)
                {
                    CircularEnt* entity = dynamic_cast<CircularEnt*>(it->second);
                    minDetection = min(minDetection, detectCircle(*entity, rangeBeg, rangeEnds[i]));
                }
                else if (shape == SimEnt::LINE)
                {
                    LinearEnt* entity = dynamic_cast<LinearEnt*>(it->second);
                    minDetection = min(minDetection, detectLine(*entity, rangeBeg, rangeEnds[i]));
                }
                else if (shape == SimEnt::RECTANGLE)
                {
                    RectangularEnt* entity = dynamic_cast<RectangularEnt*>(it->second);
                    minDetection = min(minDetection, detectRectange(*entity, rangeBeg, rangeEnds[i]));
                }
            }
        }
    }
    _state = (float) (1 - minDetection / _range);
    //std::cout << "minDet: " << minDetection << ", sensor state: " << _state << std::endl;
}

double ProximitySensor::detectCircle(CircularEnt& entity, Point& sensor_beg, Point& sensor_end)
{
    double minDetection = INF_COLLISION;

    Point& center = entity.getCenter();
    double radius = entity.getRadius();
    Point orth_proj = orthogonalProjection(center, sensor_beg, sensor_end);
    double dist_from_line = orth_proj.getDistance(center);

    if (dist_from_line == radius && orth_proj.isBetween(sensor_beg, sensor_end))
        return sensor_beg.getDistance(orth_proj);
    else if (dist_from_line < EPS)
        return sensor_beg.getDistance(orth_proj) - radius;
    else if (dist_from_line < radius)
    {
        double k = radius / dist_from_line;
        float touchAngle = (float) acos(dist_from_line / radius);
        Point projOnCircle(center.getX() + orth_proj.getXDiff(center) * k,
            center.getY() + orth_proj.getYDiff(center) * k);
        float directionAngle = (float) acos(projOnCircle.getXDiff(center) / radius)
            * sign(projOnCircle.getYDiff(center));
        Point left(center);
        left.translate(radius * cos(directionAngle + touchAngle),
            radius * sin(directionAngle + touchAngle));
        Point right(center);
        right.translate(radius * cos(directionAngle - touchAngle),
            radius * sin(directionAngle - touchAngle));
        if (left.isBetween(sensor_beg, sensor_end))
            minDetection = min(minDetection, sensor_beg.getDistance(left));
        if (right.isBetween(sensor_beg, sensor_end))
            minDetection = min(minDetection, sensor_beg.getDistance(right));
    }
    return minDetection;
}

double ProximitySensor::detectLine(LinearEnt& entity, Point& sensor_beg, Point& sensor_end)
{
    // check if ends of linear entity are between ends of current beam
    Point temp = sensor_end - sensor_beg;
    double beg_cross = (entity.getBeg() - sensor_beg).cross(temp);
    double end_cross = (entity.getEnd() - sensor_beg).cross(temp);
    if (beg_cross && end_cross && sign(beg_cross) != sign(end_cross))
    {
        // check if ends of current beam are between ends of linear ent
        Point temp2 = entity.getEnd() - entity.getBeg();
        double beg2_cross = (sensor_beg - entity.getBeg()).cross(temp2);
        double end2_cross = (sensor_end - entity.getBeg()).cross(temp2);
        if (beg2_cross && end2_cross && sign(beg2_cross) != sign(end2_cross))
            return _range * (beg2_cross / (beg2_cross - end2_cross));
    }
    return INF_COLLISION;
}

double ProximitySensor::detectRectange(RectangularEnt& entity, Point& sensor_beg, Point& sensor_end)
{
    return INF_COLLISION;
}