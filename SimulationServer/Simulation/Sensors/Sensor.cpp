#include "Sensor.h"
#include "../Math/MathLib.h"

Sensor::Sensor(uint8_t type, double range, float rangeAngle, float placingAngle)
    : _range(range), _rangeAngle(rangeAngle), _placingAngle(placingAngle)
{
    _type = type;
    _state = 0;
    _beams = 2 + 6 * (int) (_rangeAngle / M_PI);
}

Sensor::Sensor(std::ifstream& file, bool readBinary, uint8_t type) : _type(type)
{
    if (readBinary)
    {
        file.read(reinterpret_cast<char*>(&_range), sizeof(_range));
        file.read(reinterpret_cast<char*>(&_rangeAngle), sizeof(_rangeAngle));
        file.read(reinterpret_cast<char*>(&_placingAngle), sizeof(_placingAngle));
        file.read(reinterpret_cast<char*>(&_state), sizeof(_state));
    }
    else
        file >> _range >> _rangeAngle >> _placingAngle >> _state;
}

void Sensor::serialize(Buffer& buffer) const
{
    buffer.pack(_type);
    buffer.pack(_range);
    buffer.pack(_rangeAngle);
    buffer.pack(_placingAngle);
    buffer.pack(_state);
}
void Sensor::serialize(std::ofstream& file) const
{
    file.write(reinterpret_cast<const char*>(&_type), sizeof(_type));
    file.write(reinterpret_cast<const char*>(&_range), sizeof(_range));
    file.write(reinterpret_cast<const char*>(&_rangeAngle), sizeof(_rangeAngle));
    file.write(reinterpret_cast<const char*>(&_placingAngle), sizeof(_placingAngle));
    file.write(reinterpret_cast<const char*>(&_state), sizeof(_state));
}