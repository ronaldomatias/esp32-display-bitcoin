#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* ssid = "Marisa";
const char* password = "10092018";

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.setCursor(0, 0);
  lcd.backlight();
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    lcd.print("Conectando wi-fi!");
    delay(2000);
  }

  lcd.clear();
  lcd.print("Conectado!");
}

void loop() {

  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient httpClient;

    httpClient.begin("https://www.mercadobitcoin.net/api/BTC/ticker/");
    int httpCode = httpClient.GET();

    if (httpCode > 0) {
      String payload = httpClient.getString();

      char json[500];
      payload.replace(" ", "");
      payload.replace("\n", "");
      payload.trim();
      payload.remove(0, 1);
      payload.toCharArray(json, 500);

      StaticJsonDocument doc(200);
      deserializeJson(doc, json);

      Serial.print(doc);
    } else {
      lcd.clear();
      lcd.print("Requisicao erro");
    }

    httpClient.end();
  }

  delay(5000);
}
