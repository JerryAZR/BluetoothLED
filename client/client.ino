/**
 * @file master.ino
 * @author Zerui An (anzerui@126.com / jerryazr@gmail.com)
 * @brief Use an ESP-12F (wired to a rotary encoder) as an LED controller.
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * To be added later
 */

#include <AiEsp32RotaryEncoder.h>
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "client.h"

// Not the best practise, I know.
// But I'm too lazy to type all the variables explicitly
using namespace constants;

// helper functions
bool inline active(int pin);

// variables
volatile int dc = 0;
int dc_old = -1;

//depending on your encoder - try 1,2 or 4 to get expected behaviour
#define ROTARY_ENCODER_STEPS 4

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder encoder = AiEsp32RotaryEncoder(
  CLK_PIN, DT_PIN, SW_PIN, -1, ROTARY_ENCODER_STEPS);

IPAddress server(192,168,10,100);       // the fix IP address of the server
WiFiClient client;


void IRAM_ATTR readEncoderISR()
{
  encoder.readEncoder_ISR();
}

void rotary_onButtonClick()
{
  static unsigned long lastTimePressed = 0;
  //ignore multiple press in that time milliseconds
  if (millis() - lastTimePressed < 500)
  {
    return;
  }
  lastTimePressed = millis();
  Serial.print("button pressed ");
  Serial.print(millis());
  Serial.println(" milliseconds after restart");
}

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());

  // configure the button pin as input
  encoder.begin();
  encoder.setup(readEncoderISR);
  bool circleValues = false;
	encoder.setBoundaries(0, 255, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

	/*Rotary acceleration introduced 25.2.2021.
   * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
   * without accelerateion you need long time to get to that number
   * Using acceleration, faster you turn, faster will the value raise.
   * For fine tuning slow down.
   */
	encoder.setAcceleration(100); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
}

void loop() {
  //don't do anything unless value changed
	if (encoder.encoderChanged())
	{
		dc = encoder.readEncoder();
    analogWrite(LED_BUILTIN, dc);
    dc_old = dc;
    Serial.print("Value: ");
    Serial.println(dc);

    client.connect(server, 80);   // Connection to the server
    client.println(String(dc) + "\r");  // sends the message to the server
    String answer = client.readStringUntil('\r');   // receives the answer from the sever
    Serial.println("from server: " + answer);
    client.flush();
    
	}
	if (encoder.isEncoderButtonClicked())
	{
		rotary_onButtonClick();
	}
}


bool inline active(int pin) {
  // active low
  return (digitalRead(pin) == ACTIVE);
}
