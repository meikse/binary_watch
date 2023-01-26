# binary_watch :watch:
small, minimalistic binary wrist watch built with an AVR microcontroller

| demo                       | frontal view                        |
|----------------------------|-------------------------------------|
| ![demo](./assets/demo.gif) | ![demo](./assets/frontal.png =500x) |

<!-- their is the project still in cloud -->
<!-- https://myhub.autodesk360.com/ -->

## contents

## software 

As the computing unit for this project an [atmega328p]() is used as a microcontroller.
Therefore the microcontroller must be [flashed](#flash) with the `main.hex` already provided in the [software](./software/) folder.
But since small changes due to different hardware configuration will likely occur, [re-compiling](#compile) of the `main.c` code  often provides a remedy for this issue.
Since this porject is hardware orientated, a simple circuit for flashing must be [setup](#setup).

### compile

To compile the code (if necessary after changes) the [AVR](https://www.nongnu.org/avr-libc/)-toolchain is required. 
This can be downloaded and install for Windows with [WINavr](https://winavr.sourceforge.net/) as well for Linux 
```sh
sudo apt install gcc-avr avr-libc # for debian
```
or Mac
```sh
brew tap osx-cross/avr
brew install avr-gcc avr-libc
```
if `gcc-avr` is set up correctly and lies in path. Go to the software folder and build the `.hex` file
```sh
cd ./software
make 
```
this will generate the `main.hex` but does nothing to the hardware yet, if the hardware is already set up.

### flash

There are various ways of flashing the `.hex` files on an **atmega328p**. A simple solution can be easily wired with an [arduino](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP) where the arduino itself acts as a programmer.

(Hint: the used microcontroller comes in an AU-package which is rather small. Since the flashing requires no long time, the connections between the wires and the pins can be achieved holding them via jumper cables by hand for this short duration of time)

After hooking up the cables and components in the correct manner, the utility software [avrdude](https://www.nongnu.org/avrdude/) for programming the hardware via USB is needed as well. Note, if you have already arduino-IDE installed `avrdude` should already exists on you machine.
It exists for [Windows](https://github.com/avrdudes/avrdude/releases) and can be downloaded and installed for Linux
```sh
sudo apt install avrdude
```
as well for Mac
```sh
brew install avrdude
```
As an alternative to the arduino there exist many pre-built flashing sticks for the same purposes. Make sure `avrdude` recognizes the device for flashing.

In the software folder, the previous compiled `main.hex` can be either manually programmed from the command line
```sh
# subs arduino for different device
avrdude -c arduino -p m328 -U flash:w:main.hex 
```
or simply 
```sh
make program
```
which will compile the code and flash it instantly on the microcontroller.

(Note, depending which hardware you use, the first 4 lines of the [Makefile](./Software/Makefile) might be changed)

## circuit

![sheet](./hardware/circuit/sheet.svg =1000x)

The LEDs representing the *hours* are connected to port C and the LEDs representing the minutes are connected to PORT D.
Thus both ports are configured to be outputs.
Usually the LEDs should have a resistor in series to limit the current. Due to a limit current supply of PORT C and D the maximum current is restricted.
The button represents the only [input](#principle).
The button itself is pulled to ground and debounced via hardware as well as software.
One additional resistor is needed to pull the RESET pin high.
As a frequency reference an external crystal is needed without any capacitors.

component specification:
* R1, R2 = 1000k
* C1 = 10pF
* Q1 = 32768Hz, 10pF

Note, the LEDs on the different ports needs to be different, since current differs between both ports.
The LEDs on port C have to be chosen very bright (~23500 mcd).
The LEDs on port D have to be chosen more darker (~8600 mcd).

The PCB requires (0805) SMD components which shall not further specified here.
All electrical components are listed at the [order list](#order_list).
The [gcode](./hardware/circuit) files for the PCB are stored in the hardware folder as well

## construction

The construction of the watch is built up like a sandwich. There 6 comopenents needed:
1. [base](./hardware/construction/base.obj)
2. [frame](./hardware/construction/frame.obj)
3. screen
4. PCB
5. plate
6. battery

Base and frame should be 3D printed.
The PCB can be manufactured externally.
Screen is simply sone by cutting a transparent plastic plate into shape and drill the holes for the screws. 
The plate under the base must be stainless steal to avoid corroding but still conduction of current. It has the same shape as the screen but does not have an outlet for the button.
The battery is just plugged into the base before assembly. 
Thus the Vcc pads which are attached beneath the PCB are directly connected to the Vcc of the battery.
The sandwich is tighten due to 4 screws which are counter with nuts.

**Important**, since their is not ground connection between battery and PCB yet, a connector/sleeve must be inserted which is a small pipe that is placed on the lateral surface of a screw hole in the base.
This screw hole in the base is at the top-left screw position in the [demo.gif](./asserts/demo.gif).
(The connector/sleeve can also be done by the steel sheet from the [order list](#order_list))

If the sandwich is carefully tighten the connection should be established. The microcontroller needs a few seconds sync with the external crystal.
If the watch prints 01:00 all steps have been done succesfully

cut view at the symmetry line of the watch:
```python
        ┌─────────────────────────────────────────────────────────────────────────────┐
        │                                    screen                                   │
        ├─────────────────────────────────────────────────────────────────────────────┤
        │                                                                             │
        │                                    frame                                    │
        │                                                                             │
        │                                                                             │
        ├─────────────────────────────────────────────────────────────────────────────┤
        │┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼ PCB ┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼│
┌───────┴──────┬──────────────────────────────────────────────────────────────┬───────┴───────┐
│ base         │                                +                             │         base  │
│              │                             battery                          │               │
│              │                                -                             │               │
└───────┬──────┴──────────────────────────────────────────────────────────────┴───────┬───────┘
        │                                  plate (ND)                                 │ 
        └─────────────────────────────────────────────────────────────────────────────┘

                                          "middel cut"
```

The shafts for the armband are manufactured by hand as well.
The shaft does not need to be rigidly fixed on its axis, if the 3D printing is done precisely because the shaft should be stiff in them.

### order list
|amount|item|
|-|-|
1 | chip (atmega328p)
1 | button
2 | resistors
1 | capacitor
1 | crystal (32.768kHz, 10pF)
6 | LEDs (medium brightness)
4 | LEDs (high brightness)
1 | V2A sheet (0.5mm) 		
4 | V2A countersunk head screws (M2x12)
4 | V2A nuts (M2)
2 | V2A steel shaft (2mm)
4 | V2A steel shaft rings 
1 | watch strap
1 | connector/sleeve
1 | battery 2032 3V

### functionality

By default, when touching the button the time is printed on the LEDs (`normal-mode`).

To set the time correctly, one has to push the button until all LEDs blink a few times simultaneously and the watch switches in the `set-time-mode`.
If this happens the time can be increment by 5 min when the button is once pushed.
Be careful, the `set-time-mode` finishes after a few seconds if the time will not be incremented anymore.
If this happens, the watch switches in the `await-mode`.
The time is still not running, but if one touches the button now again, the time runs again with the previous set time as the initial value.
Thus `normal-mode` is reached again.

## principle


top view (screw holes are not included for simplification):
```python
                           MSB                           LSB                  
                           2^5   2^4   2^3   2^2   2^1   2^0                 
                            |     |     |     |     |     |               
                            ▼     ▼     ▼     ▼     ▼     ▼  
                        ┌─────────────────────────────────────────────┐
                        │             ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌─────┐ │
                        │             │LED│ │LED│ │LED│ │LED│ │  C  │ │ ◄─ hours
                        │             └───┘ └───┘ └───┘ └───┘ │  R  │ │
                        │ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ │  Y  │ │
                        │ │LED│ │LED│ │LED│ │LED│ │LED│ │LED│ │  S  │ │ ◄─ minutes
                        │ └───┘ └───┘ └───┘ └───┘ └───┘ └───┘ │  T  │ │
                        │                                     │  A  │ │
                        │  ┌┬──────┬┐  ┌┬──────┬┐  ┌┬──────┬┐ │  L  │ │
                        │  ││  R1  ││  ││  C1  ││  ││  R2  ││ └┬───┬┘ │
                        │  └┴──────┴┘  └┴──────┴┘  └┴──────┴┘  │   │  │
                        │                                             │
                        │                     ┌────────────────────┐  │
                        │                     │                    │  │
                        │   ┌────────────┐    │                    │  │
                        │   │            │    │                    │  │
                        │   │   BUTTON   │    │         µc         │  │
                        │   │            │    │                    │  │
                        │   │            │    │                    │  │
                        │   └────────────┘    │                    │  │
                        │ PCB                 └────────────────────┘  │
                        └─────────────────────────────────────────────┘

                                        "top view PCB"

```
The time on the watch is read from right to left. 
The top row represents the hours, the bottom row represents the minutes
The maximum hour value is 12:00.

Following presentation holds as an example for clarification. An `X` represents an on LED. An empty box represents an off LED.
```
            ┌───┐ ┌───┐ ┌───┐ ┌───┐ 
            │   │ │ X │ │   │ │ X │ = 4 + 1 = 5
            └───┘ └───┘ └───┘ └───┘                                  ─► 5:45
┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ 
│ X │ │   │ │ X │ │ X │ │   │ │ X │ = 32 + 8 + 4 + 1 = 45
└───┘ └───┘ └───┘ └───┘ └───┘ └───┘ 
```
The shifting LEDs from left to right in the beginning of the [demo.gif](./asserts/demo.gif) do no contain any information.
This is programmed because of aesthetic reasons.

## TODO

"never change a running system" but maybe rework the `main.c` file for sleepless nights
