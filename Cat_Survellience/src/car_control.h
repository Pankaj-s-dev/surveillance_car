#include <Arduino.h>

// https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/

// =============================
// L298N Motor Driver Pin Map
// =============================

// L298N input channels
#define IN1   2   
#define IN2   14   
#define IN3   15  
#define IN4   13 
#define EN_A_B 12 

// PWM for motor speed
#define PWM_CHANNEL   0
#define PWM_FREQUENCY 1000
#define PWM_RESOLUTION 8
int current_speed = 180;   // default



void initBatteryAdc() {
    Serial.println("INIT : Battery INIT Done");  
}

int getBatteryLevel() {
    int pct = 5;
    return pct;
}


void initCarPins() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    // Setup PWM on EN_A_B (GPIO12)
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(EN_A_B, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, current_speed);   // initial speed

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("INIT : Car Pin INIT Done");  
}

void pin_info(){
    Serial.print("CAR_LEFT_GPIO : ");
    Serial.println(digitalRead(IN1));
    Serial.print("CAR_RIGHT_GPIO : ");
    Serial.println(digitalRead(IN2));
    Serial.print("CAR_FORWARD_GPIO : ");
    Serial.println(digitalRead(IN3));
    Serial.print("CAR_BACKWARD_GPIO : ");
    Serial.println(digitalRead(IN4));
}

void car_stop() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    pin_info();
}

void car_forward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    pin_info();
}

void car_backward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    pin_info();
}

void car_left() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    pin_info();
}

void car_right() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    pin_info();
}

// static esp_err_t car_handler(httpd_req_t *req)
// {
//     char query[100];
//     if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
//         char dir[20];

//         if (httpd_query_key_value(query, "dir", dir, sizeof(dir)) == ESP_OK) {

//             if (!strcmp(dir, "forward")) car_forward();
//             else if (!strcmp(dir, "backward")) car_backward();
//             else if (!strcmp(dir, "left")) car_left();
//             else if (!strcmp(dir, "right")) car_right();
//             else if (!strcmp(dir, "stop")) car_stop();
//         }
//     }

//     httpd_resp_sendstr(req, "OK");
//     return ESP_OK;
// }

static esp_err_t car_handler(httpd_req_t *req)
{
    char query[100];

    // Read full query string: ?dir=forward&speed=180
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {

        char dir[20];
        char speed_str[10];

        // Read direction
        if (httpd_query_key_value(query, "dir", dir, sizeof(dir)) == ESP_OK) {

            // Read speed parameter if present
            if (httpd_query_key_value(query, "speed", speed_str, sizeof(speed_str)) == ESP_OK) {
                current_speed = atoi(speed_str);
                current_speed = constrain(current_speed, 0, 255);
                ledcWrite(PWM_CHANNEL, current_speed);
                Serial.printf("Speed set to: %d\n", current_speed);
            }

            // Handle direction movement
            if (!strcmp(dir, "forward"))      car_forward();
            else if (!strcmp(dir, "backward")) car_backward();
            else if (!strcmp(dir, "left"))     car_left();
            else if (!strcmp(dir, "right"))    car_right();
            else if (!strcmp(dir, "stop"))     car_stop();
        }
    }

    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}



static esp_err_t battery_handler(httpd_req_t *req) {
    char json[64];
    int level = getBatteryLevel();  // <-- your ADC reading function
    Serial.print("battery level : ");
    Serial.print(level);
    snprintf(json, sizeof(json), "{\"level\": %d}", level);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json, strlen(json));
    return ESP_OK;
}

