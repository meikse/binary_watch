# binary_watch
small and minimal binary wrist watch build with an AVR controller in C  

## software
1. program is written normal C 
2. chip is atmega328p, the .hex-file (in the software folder) has to be flashed on the chip
3. there is a various variety of programmers who can do the job, but easiest ist to hook the chip on an arduino and use it ans an ISP
3. flashed with avrdude (subs whatever programmer you use):\
`avrdude -c usbtiny -p m328 -U flash:w:main.hex`

## hardware
crystal is an 32768Hz clock crytsal with 10p capacity. C1=10p; Pullup resistor = 10k; other resistor: depends on C1
button is a normal smd button

IMPORTANT: hour-LEDs have to chosen bright (~23500mcd), minuts-LEDs have to be chosen normal brightness (~8600mcd), because Port C power supply fluctuates with the supply on port Binäruhr

PCB is desgined in Eagle. libaries in the folder have to be included! Manufacturer is JLCPCB. (changes are done in the gcode afterwards. to get a clean contact for the Battery)

Case and and sticks are manufactured by hand (besides basement -> 3d-printed) with filing and drilling

V2A (stainless steel) is used to avoid corrusion
steel shaft rings not recommended, because if good drilled not necessary)

## order list
|amount|item|
|-|-|
1 | chip (atmega328p)
1 | button
2 | resistors
1 | Capacitor
1 | crystal (32.768kHz, 10pF)
6 | LEDs (medium brightness)
4 | LEDs (high brightness)
1 | V2A sheet (0.5mm) 							<- sleeve is also forged from it 
4 | V2A countersunk head screws (M2x12)
4 | V2A nuts (M2)
2 | V2A steel shaft (2mm)
4 | V2A steel shaft rings 
1 | watch strap
1 | stud
1 | battery 2032 3V

## Note

since i had no experience at this time with coding at all, the code is all over the place but works properly.
