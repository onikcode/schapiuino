### LCD Handler and shapiuino Example

For this example, you need to add LCDHandler library [v0.1.0](https://github.com/jomaora/LCDHandler/releases/tag/v0.1.0).

Materials:
	* Arduino UNO
	* Arduino Ethernet Shield
	* press button
	* 10KOhm resistance
	* 220 Ohm resistance
	* LCD

### LCD circuit:

The Ethernet shield uses ports 4, 10, 11, 12 and 13 to its communications. Some of these ports are used also to setup LCD. We just need to change them to other free ports. Connect :

* (pin  1 on LCD) **Vss** to GROUND
* (pin  2 on LCD) **Vcc** to 5V 
* (pin  3 on LCD) **V0** to GROUND to has max bright 
* (pin  4 on LCD) **RS** to Digital 8 
* (pin  5 on LCD) **R/W** to GROUND 
* (pin  6 on LCD) **E** to Digital 9 
* (pin 11 on LCD) **D4** to Digital 5
* (pin 12 on LCD) **D5** to Digital 7
* (pin 13 on LCD) **D6** to Digital 3
* (pin 14 on LCD) **D7** to Digital 2
* (pin 15 on LCD) **LED+** to 220 Ohm resistor, to 5V
* (pin 16 on LCD) **LED-** to GROUND
* (pins 7, 8, 9, 10 on LCD) **D0, D1, D2, D3** no connexion used

Having this, LCD init will seem like

```
LiquidCrystal lcd(8, 9, 5, 7, 3, 2);
```

![dsc_0030](https://cloud.githubusercontent.com/assets/7602475/13378926/ccc83126-de16-11e5-8493-bcf8833cfca9.JPG)

### Press button cirtuit

Basic circuit with a push button, a resistance of 10 K-Ohms and it uses digital port 6 to indicates when the request has to be sent.

### Load example on the board

* A "Press to request" message will be shown.
* Press the button
* After catching server's response, it will be shown on LCD 
