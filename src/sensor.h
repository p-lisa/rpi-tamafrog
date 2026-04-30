#include <Arduino.h>

struct SensorData {
    // int32_t distance_raw;   // distance * 1000
    float distance_m;        // distance in meters

    uint16_t strength;       // signal strength, 2 bytes

    uint8_t status;          // 0 = valid

    bool signal;
    bool saturation;
    bool ambiant;
    bool temperature;
    bool validity;
};

class Sensor{
    public:

    Sensor(){
    };

    void init(uint32_t baudrate = 921600){
        Serial2.begin(baudrate);
    }
    
    bool read(SensorData &data) { // could have added static in the begin of the line to signify it belong to the class itself and not to an instance. this would allow to not have to create an obj
        if (Serial2.available() < 16) { // return int renvoie des données buffers dans ma boite mail, en bytes
        return false;
        }

        if (Serial2.read() != 0x57) {
        return false;
        }

        uint8_t buffer[15]; // 15 et pas 16 parce que la première est marked as read

        if (Serial2.readBytes(buffer, 15) != 15) { // "vide la boite mail et la fout dans le buffer" 
        return false;
        }

        int32_t rawDistance = // or: si tu mélanges des bytes et tu veux qu'ils s'écrasent entre eux
        ((int32_t)buffer[7]) |
        ((int32_t)buffer[8] << 8) |
        ((int32_t)buffer[9] << 16);

        // sign-extend 24-bit value
        if (rawDistance & 0x800000) {
        rawDistance |= 0xFF000000;
        }

        data.distance_m = rawDistance / 1000.0f;

        data.status = buffer[10]; // Bit masking (& 0x01) only makes sense if the protocol defines flags, which this one does not
        data.validity = (data.status == 0);

        data.strength = // often high str is low reliability because of saturation/multipath reflections
        (uint16_t)buffer[11] |
        ((uint16_t)buffer[12] << 8);

        // optional bit extraction (debug only)
        data.signal      = data.status & 0x02;
        data.saturation  = data.status & 0x04;
        data.ambiant     = data.status & 0x08;
        data.temperature = data.status & 0x10;

        return true; // as bool to signal thata frame was read and parsed
    }

    bool is_near(SensorData &data, float dist_m) {
        if (!read(data)) return false;

        return data.validity &&
            data.strength > 1 &&
            data.strength < 70 &&
            data.distance_m < dist_m;
    }

    bool is_far(SensorData &data, float dist_m) {
        if (!read(data)) return false;          // no new info → don't trigger
        if (!data.validity) return false;       // ignore invalid frames

        return data.distance_m >= dist_m;
    }
};
