#include <splash.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <FastLED.h>

CRGB leds[6];
 
static uint32_t oldtime = millis();
uint8_t speedyResponse[100]; //The data buffer for the Serial data. This is longer than needed, just in case
uint8_t byteNumber[2];  // pointer to which uint8_t number we are reading currently
uint8_t readclt; // clt doesn't need to be updated very ofter so
int clt;   // to store coolant temp
unsigned int rpm;  //rpm and PW from speeduino
float afr;
float state;
float mapData;
int8_t psi;
float afrConv;
uint8_t cmdAdata[40] ; 
uint8_t test;
 
/*
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
*/
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//SoftwareSerial mySerial(8, 9); // RX, TX

void setup() {
    display.begin();
    display.setBias(display.getBias() + 1);
    display.setContrast(display.getContrast() + 1);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);            // Draw 2X-scale text
    display.display();
    //mySerial.begin(115200);
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, A0, GRB>(leds, 6);
    FastLED.setBrightness(32);
}


#define BYTES_TO_READ 74
#define SERIAL_TIMEOUT 300
float rps;
boolean sent = false;
boolean received = false;
uint32_t sendTimestamp;

void loop () {
  requestData();
  if(received) {
 //   displayData();
    drawData();
    received = false;
  }
}


void requestData() {
  if(sent && Serial.available()) {
    if(Serial.read() == 'A') {
      uint8_t bytesRead = Serial.readBytes(speedyResponse, BYTES_TO_READ);
      if(bytesRead != BYTES_TO_READ) {
        processData();
        for(uint8_t i = 0; i < bytesRead; i++) {
        }
        received = true;
        clearRX();
      } else {
        processData();
        received = true;
        rps = 1000.0/(millis() - sendTimestamp);
      }
      sent = false;
    } else Serial.read();
  } else if(!sent) {
    Serial.write('A');
    sent = true;
    sendTimestamp = millis();
  } else if(sent && millis() - sendTimestamp > SERIAL_TIMEOUT) {
    sent = false;
  }
}

void clearRX() {
  while(Serial.available()) Serial.read();
}
 

 
void drawData() { //write data to LEDs based on RPM
  if(rpm < 1000) {
    leds[0] = CRGB::Black; //off
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
   
  }
    if(200 < rpm && rpm < 600) {
    leds[0] = CRGB::Green; //cranking
    leds[1] = CRGB::Green;
    leds[2] = CRGB::Green;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Green;
    leds[5] = CRGB::Green;
   
  }
 
  if(rpm > 600){
    leds[0] = CRGB::Black; //idle above 600 rpm below 1000
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Green;        
  }
    
  if(rpm > 1500){
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Yellow;
  }
  
  if(rpm > 2000) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Green;
    leds[5] = CRGB::Yellow;
   
  }
  
  if(rpm > 2500){
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Yellow;
    leds[5] = CRGB::Yellow;
  }
    
  if(rpm > 3000){
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Yellow;
    leds[5] = CRGB::Yellow;
  }
  
  if(rpm > 3500) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Yellow;
    leds[5] = CRGB::Red;
   
  }

  if(rpm > 4000) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Green;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Yellow;
    leds[5] = CRGB::Red;
   
  }

  if(rpm > 4500) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Yellow;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
   
  }
  
  if(rpm > 4500) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Green;
    leds[2] = CRGB::Yellow;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
   
  }
  
  if(rpm > 5000) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Green;
    leds[2] = CRGB::Yellow;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
   
  }
  
  if(rpm > 5500) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Yellow;
    leds[2] = CRGB::Yellow;
    leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
   
  }
  
  if(rpm > 6000) { 
    leds[0] = CRGB::Green;
    leds[1] = CRGB::Yellow;
    leds[2] = CRGB::Yellow;
    leds[3] = CRGB::Red;
    leds[4] = CRGB::Red;
    leds[5] = CRGB::Red;
   
  }

  /*Serial.println("RPM:" + String(rpm));
  Serial.println("RAWLOW" + String(speedyResponse[14]));
  Serial.println("RAWHIGH" + String(speedyResponse[15]));
   */ 
  display.clearDisplay(); //display RPM
  display.setTextSize(1);           
  display.setCursor(0,0);
  display.print("rpm");
  display.setTextSize(2);
  display.setCursor(23,0);
  display.print(rpm, 1);

  display.setTextSize(1);            //display status
  display.setCursor(0,15);
  display.print("stat");
  display.setTextSize(2);
  display.setCursor(25,15);
  display.print(state, 1);
  
  display.setTextSize(1);            //display coolant temp
  display.setCursor(0,25);
  display.print("clt");
  display.setTextSize(2);
  display.setCursor(25,28);
  display.print(clt, 1);
 /* display.setCursor(69,0);
  display.print("|");
  display.setCursor(104,2);
  display.setTextSize(1);
  display.print("-map");
  display.setTextSize(2);
  display.setCursor(80,0);
  display.print(mapData);*/
  //delay(25);  // this delay was placed in order for the screen to not populate at a speed that is inhumanly readable
  FastLED.show(); //set leds
  display.display(); //set display
}
void processData() {  // necessary conversion for the data before sending to screen
 
  rpm = ((speedyResponse [15] << 8) | (speedyResponse [14])); // rpm low & high (Int) TBD: probaply no need to split high and low bytes etc. this could be all simpler
  afr = speedyResponse[10];
  state = speedyResponse[2];
  mapData = ((speedyResponse [5] << 8) | (speedyResponse [4]));
  psi = (mapData / 6.895);
  clt = speedyResponse[7];
  afrConv = afr/10;
  
}
/* //code from a project i don't remember the link to, keeping it here as a frame of reference for communication
void buildArray(){
      for (i = 0; i <73; i++){
      response[i] = mySerial.read();
    }
}

void loop() {
display.clearDisplay();
// run over and over 
mySerial.write("A");// Schreibe zum Seriellen Port
//  delay(10);
buildArray();// Read Block of Data
// Make usable Values
RPM = ((response [15] << 8) | (response [14]));
advance = response[22];
battery10 = response[9]; //battery voltage
dwell = response[3]; //Dwell in ms * 10
//rpmDOT = ((response [34] << 8) | (response [33]));
//launchingSoft= response[72];// Wert nicht klar
//String thisString = String(secl);
display.print(RPM);
Serial.println(String(response));
Serial.println(RPM);
display.setCursor(0,0);
display.print(advance);
display.setCursor(0,10);
display.print(dwell);
display.setCursor(0,20);
display.println(battery10);
display.display();
delay(150);
}
*/
/*
secl = response[0]; //secl is simply a counter that increments each second. Used to track unexpected resets (Which will reset this count to 0)
squirt = response[1]; //Squirt Bitfield
engine = response[2]; //Engine Status Bitfield
dwell = response[3]; //Dwell in ms * 10
MAP = ((response [5] << 8) | (response [4])) ; //map value is divided by 2
IAT= response[6] ; //mat
coolant = response[7]; //Coolant ADC
tpsADC = response[8]; //TPS (Raw 0-255)
battery10 = response[9]; //battery voltage
O2 = response[10]; //O2
egoCorrection = response[11]; //Exhaust gas correction (%)
iatCorrection = response[12]; //Air temperature Correction (%)
wueCorrection = response[13]; //Warmup enrichment (%)
RPM = ((response [15] << 8) | (response [14])); //Junto todo en una variable
TAEamount = response[16]; //acceleration enrichment (%)
// 0x00 = response[17]; //Barometer correction (%)
corrections = response[18]; //Total GammaE (%)
VE = response[20]; //Current VE 1 (%)
afrTarget = response[21];
PW = response[22]; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
tpsDOT = response[23]; //TPS DOT
advance = response[22];
TPS = response[23]; // TPS (0% to 100%)
loopsPerSecond = ((response [25] << 8) | (response [24])); //Junto todo en una variable
freeRAM = ((response [27] << 8) | (response [26])); //Junto todo en una variable

batCorrection = response[30]; //Battery voltage correction (%)
spark = response[31]; //Spark related bitfield
O2_2 = response[32]; //O2

rpmDOT = ((response [34] << 8) | (response [33])); //Junto todo en una variable

flex = response[35]; //Flex sensor value (or 0 if not used)
*/
