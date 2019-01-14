/*======================================================== 
 WiFi Web Server and testing Nokia LCD
 
 Created by:    Peiman Dadkhah
 Date:          2019/01/06  
 ========================================================= */

#include <SPI.h>
#include <WiFi.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//HardwareSerial *LCD = &Adafruit_PCD8544;
//***************************LCD Pin Set Up**************************
// ESP8266 Software SPI (slower updates, more flexible pin options):
// pin 14 - Serial clock out (SCLK)
// pin 13 - Serial data out (DIN)
// pin 12 - Data/Command select (D/C)
// pin 5 - LCD chip select (CS)
// pin 4 - LCD reset (RST)
#define SCLK  14
#define DIN   13
#define DC    12
#define CS    5
#define RST   4
Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);
//***************************DHT Sensor Pin Set Up****************************
#define DHTTYPE DHT11   // DHT 11
const int dhtPin = 9;
DHTesp dht;

//***************************Digital Temp Pin Set Up**************************

//***************************Wifi Set Up**************************************
const char* ssid     = "HomeSweetHome";
const char* password = "Maniais#1";

//***************************Pin Set Up***************************************

// =========================================================
void setup() {
  // Serial Init
  Serial.begin(115200);

  //Display Init
  display.begin();
  // you can change the contrast around to adapt the display
  display.setContrast(50);   // for the best viewing choose 50!
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("HOME Sweet Home!!!");
  display.display(); 
  delay (2000);
//  display.setTextColor(WHITE, BLACK); // 'inverted' text
//  display.println(3.141592);
  display.setTextSize(1);
  display.setTextColor(BLACK);
//  display.println("0x"); 
//  display.println(0xDEADBEEF, HEX);
//  display.display(); 
//  delay(2000);
//  display.clearDisplay();
//  display.display();
  
  // invert the display
  // display.invertDisplay(true); // no invert put "false"
  
  // DHT temp Init
  dht.setup(dhtPin, DHTesp::DHT11);
  Serial.println("DHT initiated");

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
}

// =========================================================

void loop() {
  // Just to show the program is alive...
  static int counter = 0;
  
  float T = dht.getTemperature();
  float H = dht.getHumidity ();
  Serial.printf(" % Temp: %f", T);
  Serial.printf("\tHumid: %f", H);
  
//  display.setCursor(0,0);
  display.print("Temp: ");
  display.println(T);
  display.print("Humid: ");
  display.println(H);
  display.display();
  delay(1500);
  Serial.printf("\n");
  display.clearDisplay();
  display.display();
}
