#define BLYNK_TEMPLATE_ID           "TMPL6hALKXOpy"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "6KPtBEF8-ULH5Wq7jg_Dc07M3pEWBBVn"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "OSINT";
char pass[] = "p09oi87u";

#define relay D1      //D1
#define button D3     //D3
#define sensor D7     //D7
#define autoBtn D5    //D5
WidgetLED led(V0);
BlynkTimer timer;
boolean buttonState = HIGH;
boolean autoState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);         //D1
  pinMode(button, INPUT_PULLUP);  //D3
  pinMode(sensor, INPUT_PULLUP);  //D7
  pinMode(autoBtn, INPUT_PULLUP); //D5
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, blinkLedWidget);
  timer.setInterval(200L, run);
}

void loop() {
  Blynk.run();
  timer.run();
}
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1, V2);
}

void run() {
  if (digitalRead(autoBtn) == LOW)
    autoState = !autoState;
  Blynk.virtualWrite(V2, autoState);
  if (autoState == LOW) {  
    if (digitalRead(button) == LOW) {
      if (buttonState != LOW) {
        digitalWrite(relay, !digitalRead(relay));
        Blynk.virtualWrite(V1, digitalRead(relay));
      }
      buttonState = LOW;
    } else {
      buttonState = HIGH;
    }
  } else {
    digitalWrite(relay, digitalRead(sensor));
    Blynk.virtualWrite(V1, digitalRead(sensor));
  }
}

void blinkLedWidget() {
  if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }
}
BLYNK_WRITE(V1) {
  if (autoState == LOW) {
    digitalWrite(relay, boolean(param.asInt()));
  }
}

BLYNK_WRITE(V2) {
  autoState = boolean(param.asInt());
}
