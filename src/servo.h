#pragma once

#include <Arduino.h>
#include <Wire.h> // the I2C com library
#include <Adafruit_PWMServoDriver.h> // provides setPWM() for eg

const int SERVO_MIN = 120;   // pulse count for ~0°, min pulse length to send. Not in microseconds but in PCA countar value within one PWM cycle
const int SERVO_MAX = 600;   // pulse count for ~180°
const int SERVO_CHANNELS = 16; // nb channels on pca9685

class PCAServo{
    public:
    PCAServo(int sda, int scl){
        pin_sda = sda;
        pin_scl = scl;
        pwm = new Adafruit_PWMServoDriver(0x40, Wire1);
        for (int i = 0; i < SERVO_CHANNELS; i++) {
            current_angle[i] = 90; // assumed val by default
            }
        }

    ~PCAServo(){ // fct called when object go out of scope ou est delete
        delete pwm;
    }

    void init(){
        Wire1.setSDA(pin_sda); // data line for I2C
        Wire1.setSCL(pin_scl); // clock line for I2C
        Wire1.begin(); // initialize the I2C bus

        delay(200);

        pwm->begin(); // initializes the PCA9685 chip
        pwm->setPWMFreq(50);   // standard servo frequency: 50 Hz. sets the PWM repetition frequency of the PCA9685 outputs. This is for the com btw pca and servos (whereas SCL is btw pico and pca)

        delay(200);
    }

    void set_angle(int channel, int angle) {
        // for absolute move
        angle = constrain(angle, 0, 360); // force angle to stay btw 0 and 180 degrees to avoid invalid values. it caps at max/min.
        current_angle[channel] = angle; // save the current angle

        int pulse = _angle_to_pulse(angle);
        pwm->setPWM(channel, 0, pulse); // sends the pwm signal. Syntax is: setPWM(channel, on, off) (=at what count the signal turns on/off) The PCA9685 uses a counter from 0 to 4095 during each PWM cycle. That creates a pulse whose width is pulse counts long.
    }

    void add_angle(int channel, int delta) {
        // for relative move
        int new_angle = current_angle[channel] + delta;
        set_angle(channel, new_angle);
    }

    void add_angle_loop(int channel, int delta) {
        int new_angle = current_angle[channel] + delta;

        if (new_angle >= 180) {
            new_angle = 0;
        }

        if (new_angle < 0) {
            new_angle = 180;
        }

        set_angle(channel, new_angle);
    }

    int get_angle(int channel) {
        return current_angle[channel];
    }

    private:
    int pin_sda;
    int pin_scl;
    Adafruit_PWMServoDriver* pwm;
    int current_angle[SERVO_CHANNELS];

    int _angle_to_pulse(int angle) { // fct to conver an angle to a pwm value
        return map(angle, 0, 180, SERVO_MIN, SERVO_MAX); 
    }
};