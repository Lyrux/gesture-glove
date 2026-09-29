#ifndef HE_SENSORS_H
#define HE_SENSORS_H

struct HEData
{
    int thumb;
    int index;
    int middle;
};

void initHESensors();
HEData readHESensors();

#endif