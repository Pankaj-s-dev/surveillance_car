#include <Arduino.h>

#define forward_right 11
#define backward_right 12
#define forward_left 13
#define backward_left 9
#define speed_right 10
#define speed_left 8

#define battery_measurment A0

String input = "";

void move_forward();
void move_backward();
void move_left_extreme();
void move_right_extreme();
void stop();
int readBattery();
void driveCar(String d, int pwm);

void setup() {
    Serial.begin(57600);

    pinMode(forward_right, OUTPUT);
    pinMode(backward_right, OUTPUT);
    pinMode(forward_left, OUTPUT);
    pinMode(backward_left, OUTPUT);
    pinMode(speed_right, OUTPUT);
    pinMode(speed_left, OUTPUT);

    digitalWrite(speed_right, HIGH);
    digitalWrite(speed_left, HIGH);

}

void processCommand(String cmd) {

    if (cmd.startsWith("CMD:MOVE")) {
        // Format example: CMD:MOVE,forward,180
        int firstComma = cmd.indexOf(',');
        int secondComma = cmd.indexOf(',', firstComma + 1);

        String dir = cmd.substring(firstComma + 1, secondComma);
        int speed = cmd.substring(secondComma + 1).toInt();

        driveCar(dir, speed);
        return;
    }

    if (cmd == "CMD:BAT?") {
        int battery = readBattery();
        Serial.print("BAT:");
        Serial.println(battery);
        return;
    }
}

void driveCar(String d, int pwm)
{
    if (d == "forward") {
        move_forward();
    }
    else if (d == "backward") {
        move_backward();
    }
    else if (d == "left") {
        move_left_extreme();
    }
    else if (d == "right") {
        move_right_extreme();
    }
    else if (d == "stop") {
        stop();
    }
}

int readBattery()
{
    int raw = analogRead(battery_measurment);

    float voltage = (raw / 1023.0) * 3.3;

    // Example: Li-ion 4.2–3.3 V
    int percent = map(voltage * 100, 330, 420, 0, 100);

    percent = constrain(percent, 0, 100);

    return percent;
}

void move_forward(){
    digitalWrite(forward_right, HIGH);
    digitalWrite(forward_left, HIGH);
    digitalWrite(backward_right, LOW);
    digitalWrite(backward_left, LOW);
}

void move_backward(){
    digitalWrite(forward_right, LOW);
    digitalWrite(forward_left, LOW);
    digitalWrite(backward_right, HIGH);
    digitalWrite(backward_left, HIGH);
}

void move_left_extreme(){
    digitalWrite(forward_right, HIGH);
    digitalWrite(forward_left, LOW);
    digitalWrite(backward_right, LOW);
    digitalWrite(backward_left, HIGH);
}

void move_right_extreme(){
    digitalWrite(forward_right, LOW);
    digitalWrite(forward_left, HIGH);
    digitalWrite(backward_right, HIGH);
    digitalWrite(backward_left, LOW);
}

void stop(){
    digitalWrite(forward_right, LOW);
    digitalWrite(forward_left, LOW);
    digitalWrite(backward_right, LOW);
    digitalWrite(backward_left, LOW);
}

void loop() {
    Serial.print("Goood");
    delay(1000);
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            processCommand(input);
            input = "";
        } else {
            input += c;
        }
    }
}