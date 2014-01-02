Showcase Backlights
====
Showcase Backlights are a Trinket & LPD8806 backlighting solution.
## Dependecies
### Software
Building and flashing the project requires a few less than ordinary tools.
1. Adafruit's modified [Arduino IDE](http://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide).
2. The Python INO project. I recommend using virtualenv as well. `pip isntall ino` or `easy_install ino`
3. AvrDude with the modification described [here](http://learn.adafruit.com/introducing-trinket/programming-with-avrdude).

### Hardware
1. Adafruits Trinket 5v. (Modification will allow this to work on an ordinary ATtiny85)
2. A photoresistor & a 10k resistor
3. A strip of 10 LPD8806 LED's
4. A 1 amp 5v power supply

## Building
`cd` into the project directory and `ino build`.
The details are contained in the .ini file.

## Flashing
`./upload`.
Note: I use zsh and you will probably have to change the shebang line to point to bash or whichever shell you are using.