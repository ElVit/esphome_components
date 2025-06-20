# ESPHome Maidesite Desk Component

[ESPHome](https://esphome.io/) component for controlling Maidesite desk controllers via their serial protocol.

## Usage

### What you need

* ESPHome compatible microcontroller (e.g. ESP8266, ESP32, ESP32-S2, ESP32-C3, ...)
* RJ12 cable (phone cable, RJ11 may also work)

### Wiring

#### RJ12 connector pinning

Please double check this for your specific model!

pin | function
----|---------
 1  | NC (pulled up)
 2  | GND
 3  | TX
 4  | VCC
 5  | RX
 6  | NC (pulled up)

#### Microcontroller pinning

ESP    | desk
-------|-----
GND    | GND
5V     | VCC
GPIO3  | TX
GPIO4  | RX

![IMG_1256](https://github.com/user-attachments/assets/229f52bb-5cdd-454c-b72b-e84979ee5976)

Sometimes the desk controller does not supply enough current, then simply disconnet the VCC pin and plug in a micro-usb/usb-c cable into your ESP board.  

### Example ESPHome yaml code

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/ElVit/esphome_components/
    components: [ maidesite_desk ]

uart:
  - id: uart_bus
    tx_pin: GPIO3
    rx_pin: GPIO4
    baud_rate: 9600

maidesite_desk:
  id: my_desk

sensor:
  - platform: maidesite_desk
    height_abs:
      name: "Height"

number:
  - platform: maidesite_desk
    height_abs:
      name: "Height"
      mode: SLIDER

button:
  - platform: maidesite_desk
    stop:
      name: "Stop"
    step_up:
      name: "Step up"
    step_down:
      name: "Step down"
    goto_max:
      name: "Move to MAX"
    goto_min:
      name: "Move to MIN"
    goto_m1:
      name: "Move to M1"
    goto_m2:
      name: "Move to M2"
    goto_m3:
      name: "Move to M3"
    goto_m4:
      name: "Move to M4"
```

### Examples

* basic, sufficient for most people: [example_basic.yaml](example_basic.yaml)
* full with all features in use: [example_full.yaml](example_full.yaml)

### Features

#### sensors entities

entity       | description
-------------|----------------------------
height_abs   | current height of the desk
height_pct   | height in percent
height_min   | minimal height
height_max   | maximal height
position_m1  | 1st stored height
position_m2  | 2nd stored height
position_m3  | 3rd stored height
position_m4  | 4th stored height

#### number entities

entity      | description
------------|----------------------------
height_abs  | current height of the desk
height_pct  | height in percent

#### buttons entities

entity     | description
-----------|---------------------------
step_up    | move up desk by one step (~14mm)
step_down  | move down desk by one step (~14mm)
goto_max   | move up until stopped
goto_min   | move down until stopped
stop       | stop movement of desk
goto_m1    | move to 1st stored height
goto_m2    | move to 2nd stored height
goto_m3    | move to 3rd stored height
goto_m4    | move to 4th stored height
save_m1    | save current height to 1th position
save_m2    | save current height to 2th position
save_m3    | save current height to 3th position
save_m4    | save current height to 4th position

#### lambda methods

method                                | description
--------------------------------------|---------------------------
`id(my_desk).step_up()`               | step_up desk by one step (~14mm)
`id(my_desk).step_down()`             | step_down desk by one step (~14mm)
`id(my_desk).goto_max_position()`     | move up
`id(my_desk).goto_min_position()`     | move down
`id(my_desk).stop()`                  | stop movement of desk
`id(my_desk).goto_mem_position(pos)`  | move to stored height `pos`
`id(my_desk).save_mem_position(pos)`  | save current height to position `pos`
`id(my_desk).goto_height(height)`     | move to specified height

## Dev Notes

[Jarvis reverse engineering notes](https://docs.google.com/spreadsheets/d/1GKZfDFljVX4eQBMawq0-Rc8t0x8V6gjQ5BgAYngPYTo/edit?pli=1&gid=1438530487#gid=1438530487)

To see and send custom UART messages the following esphome code can be used:

``` yaml
esphome:
  ...
  includes: components/common_includes.h

uart:
  - id: uart_bus
    tx_pin: GPIO3
    rx_pin: GPIO4
    baud_rate: 9600
    debug:
      direction: BOTH
      after:
        bytes: 9
      sequence:
        - lambda: UARTDebug::log_hex(direction, bytes, ',');

text:
  - platform: template
    name: "UART message"
    id: uart_msg
    optimistic: true
    min_length: 0
    max_length: 100
    mode: text
    initial_value: "f1,f1,01,00,01,7e"

button:
  - platform: template
    name: "Send UART message"
    on_press:
      - uart.write: !lambda |-
          auto str = id(uart_msg).state;
          str = std::regex_replace(str, std::regex("0x"), "");
          str = std::regex_replace(str, std::regex(",| "), "");
          std::vector<uint8_t> out;
          auto ret = parse_hex(str.c_str(), out, str.length() / 2);
          return out;
```

To be able to use the regex command the file "common_includes.h" must be created in the directory "components".  
The content of the "common_includes.h" must be:

``` c++
#include <iostream>
#include <regex>
#include <string.h>
```

## Sources

:heart: A big THANKS to [Rocka84](https://github.com/Rocka84/esphome_components) for doing the main work, I've just forked this repo and modified some parts.  
:heart: A big THANKS  to [shades66](https://github.com/shades66/Maidesite-standing-desk/tree/main) for the minimal and easy to setup esphome config.  
:heart: A big THANKS  to [phord/Jarvis](https://github.com/phord/Jarvis) for reverse engineering the UART interface and most control messages!  
:heart: A big THANKS  to [OkhammahkO](https://github.com/OkhammahkO) for collecting the scattered information at [pimp-my-desk/desk-control](https://gitlab.com/pimp-my-desk/desk-control)!  
:heart: Thanks to the whole home assistant community for sharing their knowlege and helping me to create this ESPHome component!  
