#ifndef HE_SENSORS_H
#define HE_SENSORS_H

struct HEData
{
    int16_t thumb;
    int16_t index;
    int16_t middle;
};

void initHESensors();
HEData readHESensors();

#endif
