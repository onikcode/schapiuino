### Server Servo Motor Example

The basic example allows to handle a servo motor, indicating the angle to which it should turns. To this, the serveur expose the following API

### /servomotor/angle
- POST
    Within the body, you should specify the angle to which the servo motor should turn.
- GET
    Returns a JSON containing the current angle on the servo motor.

### /servomotor/reset
- POST
    It forces the servo motor to turn to angle = 0
    
The circuit to build uses :
- A servo motor
- A 100 µF capacitor
- Digital Port 9 to write pin on servo motor (white cable) 

![dsc_0034](https://cloud.githubusercontent.com/assets/7602475/13553848/25e86cc8-e397-11e5-9ed8-3bf3a66248f6.JPG)

You can open a REST client and try:

POST 192.168.1.178, with body 90 to make servo turn and the POST /servomotor/reset to return to the original position.

