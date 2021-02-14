#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_adc_cal.h>
#include <ESP32Ping.h>          // https://github.com/marian-craciunescu/ESP32Ping
#include <WiFiClientSecure.h>

#include <config.h>


#define ADC_EN              14  //ADC_EN is the ADC detection enable port
#define ADC_PIN             34
#define BUTTON_1            GPIO_NUM_0
#define BUTTON_2            GPIO_NUM_35

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library


void IRAM_ATTR isr() {

  esp_sleep_enable_ext0_wakeup(BUTTON_1, 0);
  delay(200);
  esp_deep_sleep_start();

}

void markWiFiState () {

  if (WiFi.status() != WL_CONNECTED)
    tft.fillRect(0, 0, tft.width(), 35,TFT_RED);
  else
    tft.fillRect(0, 0, tft.width(), 35,TFT_GREEN);
  
  tft.setTextColor(TFT_BLACK);
  tft.drawString("WiFi", tft.width() / 2, (35 / 2) );

}

void markPingState () {

  float avg_time;

  for (byte i = 0; i < PINGHOSTS; i++) {
    if(Ping.ping(hosts[i].host_ip, 2)) {
      avg_time = Ping.averageTime();
      if (avg_time > hosts[i].avg_delay)  
        tft.fillRect(0, (2+35) * hosts[i].position, tft.width(), 35, TFT_YELLOW);
      else
        tft.fillRect(0, (2+35) * hosts[i].position, tft.width(), 35, TFT_GREEN);
    } else {
        tft.fillRect(0, (2+35) * hosts[i].position, tft.width(), 35, TFT_RED);
    }
    tft.setTextColor(TFT_BLACK);
    tft.drawString(hosts[i].name, tft.width() / 2, (35 / 2) + (2+35) * hosts[i].position );
  }

}

void markSiteState () {

  WiFiClientSecure client;
  bool issiteok = false;

  for (byte i = 0; i < WEBSETIES; i ++) {
    client.setCACert( websites[i].crt );

    if (!client.connect(websites[i].host , 443)) {
      tft.fillRect(0, (2+35) * websites[i].position, tft.width(), 35, TFT_RED);
    } else {

      client.println( websites[i].get_string );
      client.println( websites[i].hst_string );
      client.println("Connection: close");
      client.println();

      issiteok = false;

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "HTTP/1.1 200 OK\r") issiteok = true;
        if (line == "\r") break;
      }

      client.stop();
      if (issiteok)
        tft.fillRect(0, (2+35) * websites[i].position, tft.width(), 35, TFT_GREEN);
      else 
        tft.fillRect(0, (2+35) * websites[i].position, tft.width(), 35, TFT_YELLOW);

    }

    tft.setTextColor(TFT_BLACK);
    tft.drawString( websites[i].name, tft.width() / 2, (35 / 2) + (35 + 2) * websites[i].position );
  }

}

void showVoltage()
{  
  const int vref = 1100;
  uint16_t v = analogRead(ADC_PIN);
  float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
  String voltage = String(battery_voltage) + "V";

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString( voltage, tft.width() / 2, (35 / 2) + (35 + 2) * 5 );
}


void setup() {
    Serial.begin(115200);

    /*
    ADC_EN is the ADC detection enable port
    If the USB port is used for power supply, it is turned on by default.
    If it is powered by battery, it needs to be set to high level
    */
    pinMode(ADC_EN, OUTPUT);
    digitalWrite(ADC_EN, HIGH);

    pinMode(BUTTON_2, INPUT_PULLUP);
    attachInterrupt(BUTTON_2, isr, FALLING);

    tft.init();
    tft.setRotation(0);

    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);

    tft.drawString("Hello", tft.width() / 2, tft.height() / 2 );

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
}

void loop() {
  delay(1000);
  
  markWiFiState ();
  markPingState ();
  markSiteState();

  showVoltage();
}