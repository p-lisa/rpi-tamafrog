#pragma once 

#include <Arduino.h>

class LED{
    public:
    LED(int p, float phase = 0.0f){ // phaseoffset is used so that the fire patterns are indep for each led
        pin = p;
        phaseOffset = phase;
    }
    void init(){
        pinMode(pin, OUTPUT);
    }
    void on(){
        digitalWrite(pin, true);
    }
    void off(){
        digitalWrite(pin, false);
    }
    void fire(){
        float t = millis() * 0.02 + phaseOffset;;   // controls speed
        float n = noise1D(t);         // -1 to 1
        float brightness = mapNoise(n, 50, 255);

        analogWrite(pin, (int)brightness);
    }

    private:
    int pin;
    float phaseOffset;

    float hash(int x) {
        x = (x << 13) ^ x;
        return 1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
    }

    float smoothstep(float t) {
        return t * t * (3 - 2 * t);
    }

    float noise1D(float x) {
        int x0 = (int)floor(x);
        int x1 = x0 + 1;

        float t = x - x0;
        float fade = smoothstep(t);

        float n0 = hash(x0);
        float n1 = hash(x1);

        return n0 * (1 - fade) + n1 * fade;
    }

    float mapNoise(float n, float minVal, float maxVal) {
        return (n * 0.5 + 0.5) * (maxVal - minVal) + minVal;
    }
};