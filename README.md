# Tamafrog

An Arduino-based Raspberry Pi Pico W project that controls an interactive frog character using a TFT LCD screen (ST7735S 128x128), buttons, a distance sensor, an LED, and an SG90 servo driven through a PCA9685 PWM controller.

The project uses a simple state machine to switch between frog states such as 'sleeping', 'active', 'eating', 'looting', and 'executing'.

## Features

- Raspberry Pi Pico W firmware written in C++ using the Arduino framework
- TFT LCD display for frog state images
- Distance-based wake/sleep behavior using a laser distance sensor
- Three physical buttons for user actions
- LED feedback during the loot action
- SG90 servo control through a PCA9685 PWM driver
- State-machine-based behavior flow

## Hardware

| Component | Purpose |
| --- | --- |
| Raspberry Pi Pico W | Main microcontroller |
| TFT LCD screen (ST7735S 128x128) | Displays frog state images |
| Nooploop TOF Sense Laser Range Sensor 18301 (EEHD-5PL7) | Detects whether something is near or far |
| PCA9685 servo driver | Controls the SG90 servo through I2C |
| SG90 servo | Executes a physical motion |
| Push button on GP21 | Eat action |
| Push button on GP18 | Loot action |
| Push button on GP16 | Execute action |
| LED on GP20 | Loot action feedback |
| External 5V power supply | Powers the servo output rail on the PCA9685 |

## Pinout and Wiring

### Buttons

| Function | Raspberry Pi Pico W Pin |
| --- | --- |
| Eat button | GP21 |
| Loot button | GP18 |
| Execute button | GP16 |

### LED

| Component | Raspberry Pi Pico W Pin |
| --- | --- |
| LED | GP20 |

### LCD Screen

| LCD Pin | Raspberry Pi Pico W Connection |
| --- | --- |
| VCC | 3V3 |
| SCL / SCK / CLK | GP2, SPI0 SCK |
| SDA / MOSI / DIN | GP3, SPI0 TX |
| RES / Reset | GP5 |
| DC / Data Command | GP4 |
| CS / Chip Select | GP1 |

### PCA9685 Servo Driver

| PCA9685 Pin | Raspberry Pi Pico W Connection |
| --- | --- |
| SDA | GP6, I2C1 SDA / Wire1 |
| SCL | GP7, I2C1 SCL / Wire1 |
| OE | GND |
| VCC | 3V3 OUT |
| V+ / servo power rail | External 5V supply |

The SG90 servo is connected to channel 4 of the PCA9685.

### Laser Distance Sensor

| Sensor Pin | Raspberry Pi Pico W Connection |
| --- | --- |
| TX | GP9, UART1 RX |
| RX | GP8, UART1 TX |
| VCC | VBUS |


## Software Behavior

The firmware starts by initializing all hardware modules:

- Serial output at 9600 baud
- LED
- LCD screen
- Distance sensor
- PCA9685 servo driver
- Buttons

The servo is initialized to `0°`.

After initialization, the application runs continuously inside `app_run()` and updates the state machine.

## State Machine

The project uses the following frog states:

| State | Behavior | Transition |
| --- | --- | --- |
| `SLEEP` | Displays the sleeping image | Goes to `ACTIVE` when the sensor detects an object is near |
| `ACTIVE` | Displays the active image | Goes back to `SLEEP` when the sensor detects an object is far |
| `EAT` | Displays the eating image for set duration | Returns to `ACTIVE` |
| `LOOT` | Displays the loot image and fires the LED for set duration | Turns the LED off and returns to `ACTIVE` |
| `EXECUTE` | Displays the execute image and moves the servo to 90° | Returns to `ACTIVE` after 2 seconds |

Button behavior while active:

| Button | State Triggered |
| --- | --- |
| Eat button | `EAT` |
| Loot button | `LOOT` |
| Execute button | `EXECUTE` |

## Project Structure

The visible application entry points are:

```text
main.cpp
app.cpp
```

Expected supporting modules include:

```text
app.h
buttons.h / buttons.cpp
led.h / led.cpp
lcd.h / lcd.cpp
sensor.h / sensor.cpp
servo.h / servo.cpp
state_machine.h
assets/assets.h
```

The `assets/assets.h` file is expected to provide image assets such as:

```cpp
sleep_img
active_img
eat_img
loot_img
execute_img
```

## Main Application Flow

`main.cpp` starts serial communication and then runs the application:

```cpp
void setup() {
  Serial.begin(9600);
  app_run();
}

void loop() {
}
```

`app_run()` initializes the hardware once and then repeatedly calls `app_update()`:

```cpp
void app_run() {
    app_init();
    while (true) {
        app_update();
    }
}
```

The current application runs an infinite loop inside app_run(), so Arduino's loop() is intentionally unused.

## Power Notes

The PCA9685 logic side is powered from the Pico W `3V3 OUT` pin, while the servo power rail is powered by an external 5V supply.

## Build and Upload

This project is built using **PlatformIO**.

### platformio.ini (example)

```ini
[env:pico]
platform = raspberrypi
board = pico_w
framework = arduino

lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ST7735 and ST7789 Library
    adafruit/Adafruit PWM Servo Driver Library
```

### Build and Upload

```bash
pio run
pio run -t upload
```

### Serial Monitor

```bash
pio device monitor -b 9600
```