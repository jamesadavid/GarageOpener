// This #include statement was automatically added by the Particle IDE.
#include "RelayShield.h"

// Create an instance of the RelayShield library, so we have something to talk to
RelayShield myRelays;

// Create prototypes of the Spark.functions we'll declare in setup()
int relayOn(String command);
int relayOff(String command);
int checkStatus(String command);
int toggleRelay(String command);

//pin initializations
const int pinReedSensorMiddleDoor = D5;

// Initialize a generic message in case we don't get a real status lower.
const char* STATUS_MESSAGE_UNAVAILABLE = "Status unavailable.";
const char* STATUS_MESSAGE_CLOSED = "Closed";
const char* STATUS_MESSAGE_OPEN = "Open";

void setup() {

    //.begin() sets up a couple of things and is necessary to use the rest of the functions
    myRelays.begin(2);
    pinMode(pinReedSensorMiddleDoor, INPUT_PULLUP);
    // Register Spark.functions and assign them names
    Particle.function("relayOn", relayOn);
    Particle.function("relayOff", relayOff);
    Particle.function("toggleRelay", toggleRelay);
    Particle.function("checkStatus", checkStatus);

    // door reed sensor - this is visible from an IFTTT check if you wish

    Particle.variable("doorStatus", STATUS_MESSAGE_UNAVAILABLE, STRING);

}
//sense if door is open or not in this loop.
void loop() {
    // loop over Reed switch status to see if doors are open or closed.

    const char* message = STATUS_MESSAGE_UNAVAILABLE;

    if(digitalRead(pinReedSensorMiddleDoor) ==1){
        // Left door closed (removed code for right to simplify)
        message = STATUS_MESSAGE_CLOSED;
    }

    if(digitalRead(pinReedSensorMiddleDoor) ==0){
        // door open
        message = STATUS_MESSAGE_OPEN;
    }

    // this constantly sets the variable for dooStatus that can be seen by IFTTT
    Particle.variable("doorStatus", message, STRING);
    delay(500);
}

// function to check door status that we can use from DO Button app.
//  Same as in loop, but only checks when called.
//  The DO Button calls this, and raises a Spark.publish("status") variable which IFTTT listens for.
//  This is what allows you to call the function via your iPhone or Apple Watch, and in about a second, get an answer.
int checkStatus(String command){
    // Ritual incantation to convert String into Int
    char inputStr[64];
    command.toCharArray(inputStr,64);

    const char* message = STATUS_MESSAGE_UNAVAILABLE;

    if(digitalRead(pinReedSensorMiddleDoor) ==1){
        // Left door closed
        message = STATUS_MESSAGE_CLOSED;
    }

    if(digitalRead(pinReedSensorMiddleDoor) ==0){
        // door open
        message = STATUS_MESSAGE_OPEN;
    }

    Particle.publish("status", message);
    return 1;
}

int relayOn(String command){
    // Ritual incantation to convert String into Int
    char inputStr[64];
    command.toCharArray(inputStr,64);
    int i = atoi(inputStr);

    // Turn the desired relay on.
    // From your Do Button config, call the relay, and pass in 1 through 4 for the Relay to turn on.
    myRelays.on(i);

    // Respond
    return 1;
}

// Momentarily turn on, then off the relay you want.  This simulates a button press when you hook the
// physical button to the normally open (two left) leads on the relay.  Again, pass in the number of the relay to control.
// In IFTTT Do Button app, call the Photon, then point the function to toggleRelay, and in the arguments field, jut put a 1 for example.
int toggleRelay(String command){
    // Ritual incantation to convert String into Int
    char inputStr[64];
    command.toCharArray(inputStr,64);
    int i = atoi(inputStr);

    // Turn the desired relay on
    //myRelays.on(i);
    relayOn(command);
    delay(500);
    relayOff(command);
    // Respond
    return 1;
}

//  This is used in toggleRelay, however, it can also be used stand alone if you have a normally closed relay and want
//  to temporarily open it.
int relayOff(String command){
    // Ritual incantation to convert String into Int
    char inputStr[64];
    command.toCharArray(inputStr,64);
    int i = atoi(inputStr);

    // Turn the desired relay off
    myRelays.off(i);

    // Respond
    return 1;
}

// Log message to cloud, message is a printf-formatted string
void debug(String message, int value) {
    char msg [50];
    sprintf(msg, message.c_str(), value);
    Particle.publish("DEBUG", msg);
}
