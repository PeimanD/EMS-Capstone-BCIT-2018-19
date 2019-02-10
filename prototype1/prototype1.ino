
/*======================================================== 
 WiFi Web Server and testing Nokia LCD
 
 Created by:    Peiman Dadkhah
 Date:          2019/01/06

 ========================================================= */

#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//HardwareSerial *LCD = &Adafruit_PCD8544;
//***************************LCD Pin Set Up**************************
// ESP8266 Software SPI (slower updates, more flexible pin options):
// pin 14 - Serial clock out (SCLK)
// pin 12 - Serial data out (DIN)
// pin 13 - Data/Command select (D/C)
// pin 15 - LCD reset (RST)
// pin 2 - LCD chip select (CS)

#define LIGHT 27
#define SCLK  14
#define DIN   12
#define DC    13
#define RST   15
#define CS    2
Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);
//********************Temp Sensor Pin Set Up************************
#define SDA 18
#define SCL 19
// SI7050 I2C address is 0x40(64)
#define Addr 0x40
unsigned int data[2];
 
#define RELAY   0       //pin for relay signal

unsigned short set_temp = 24;
unsigned short temp_var = 1;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
unsigned int dutyCycle = 40;
//*****************Digital Temp Pin Set Up**********************

//*********************Wifi Set Up******************************
const char* ssid     = "HomeSweetHome";
const char* password = "Maniais#1";
WiFiServer server(80);
// Client variables 
char linebuf[80];
int charcount=0;

//*********************Pin Set Up*******************************

// ==========================================================
void setup() {
  // Serial Init
  Serial.begin(115200);
  
  pinMode(RELAY, OUTPUT);

  // temp sensor Setup
  Wire.begin(SDA,SCL);
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
  
  //Display Init
  display.begin();
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(LIGHT, ledChannel);
  delay(50);
  ledcWrite(ledChannel, dutyCycle);

  // you can change the contrast around to adapt the display
  display.setContrast(60);   // for the best viewing choose 50!
  display.display(); // show splashscreen
  delay(2000);
  
  digitalWrite(LIGHT, HIGH);
  display.clearDisplay();   // clears the screen and buffer
  // text display tests
  display.setTextSize(1);
  display.setTextColor(BLACK);
  // invert the display
  //display.invertDisplay(true); delay(100);
  // revert back to no rotation
   display.setRotation(2); delay(100); //Rotate the screen
  // display.setCursor(0,0);
  display.println("**Welcome**\n");
  display.println("HOME Sweet Home!!!");
  display.display();
  delay (1200);
  
  display.setTextSize(1);
  display.setTextColor(BLACK);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  display.clearDisplay();
  display.println("Connecting to: "); display.print(ssid);
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print("."); display.display();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.println("WiFi Connected");
  display.println("IP address:");
  display.println(WiFi.localIP());
  display.display(); 
  delay(3000);
  display.clearDisplay();  display.display();
}

// =========================================================

void loop() {

/*// ================Web Server Connection==================
    // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline character) 
        // and the line is blank, the http request has ended, so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

            // Check if any reads failed and exit early (to try again).

            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println();
            client.println("<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta http-equiv=\"refresh\" content=\"30\"></head>");
            client.println("<body><div style=\"font-size: 3.5rem;\"><p>ESP32 - DHT</p><p>");
            if(atoi(celsiusTemp)>=25){
              client.println("<div style=\"color: #930000;\">");
            }
            else if(atoi(celsiusTemp)<25 && atoi(celsiusTemp)>=5){
              client.println("<div style=\"color: #006601;\">");
            }
            else if(atoi(celsiusTemp)<5){
              client.println("<div style=\"color: #009191;\">");
            }
            client.println(celsiusTemp);
            client.println("*C</p><p>");
            client.println(fahrenheitTemp);
            client.println("*F</p></div><p>");
            client.println(humidityTemp);
            client.println("%</p></div>");
            client.println("</body></html>");     
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
            memset(linebuf,0,sizeof(linebuf));
            charcount=0;
          } else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
  
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
// =========================================================*/
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send temperature measurement command, NO HOLD MASTER
  Wire.write(0xF3);
  
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // temp msb, temp lsb
  if(Wire.available() == 2){
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  
  // Convert the data
  float temp = ((data[0] * 256.0) + data[1]);
  float ctemp = ((175.72 * temp) / 65536.0) - 46.85;
  float ftemp = ctemp * 1.8 + 32;
  if (!digitalRead(RELAY)){
    digitalWrite(RELAY,HIGH);
  }else{
    digitalWrite(RELAY,LOW);
  }
  // Output data to serial monitor
  Serial.print("Temperature: ");
  Serial.print(ctemp); Serial.print(" C\n");
  delay(500);
  //******************************************************
  display.clearDisplay();
  display.print("Temp: ");  display.println(ctemp);
  display.println("");
  //  display.print("Humid: ");  display.println(H);
  display.display();
  delay(500);
}
