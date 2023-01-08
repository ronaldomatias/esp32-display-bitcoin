#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const char* ssid = "Marisa";
const char* password = "10092018";
const String urlAPI = "http://api.coindesk.com/v1/bpi/currentprice/BTC.json";


void setup() {

  initializeLed();
  connectWifi();
  writeInDisplay("Conectado");
}

void loop() {
  makeRequestAndGetBitcoinCurrentPrice();
  delay(300000);
}

void connectWifi() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    lcd.print("Conectando wi-fi!");
    delay(2000);
  }
}

void initializeLed() {

  lcd.init();
  lcd.backlight();
}

void writeInDisplay(const char* message) {

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print(message);
}

void makeRequestAndGetBitcoinCurrentPrice() {

  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient httpClient;

    httpClient.begin(urlAPI);
    int httpCode = httpClient.GET();

    if (httpCode > 0) {
      String payload = httpClient.getString();

      StaticJsonDocument<768> doc;
      deserializeJson(doc, payload);

      JsonObject bpi = doc["bpi"];
      const char* bitcoinCurrentPrice = bpi["USD"]["rate"];

      writeInDisplay(bitcoinCurrentPrice);
      doc.clear();

    } else {
      writeInDisplay("Erro requisição");
    }

    httpClient.end();
  }
}
