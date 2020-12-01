#include <FastLED.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SoftwareSerial.h>
CRGB leds[6];

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

volatile boolean hasSync;
unsigned int RPM;
long longRPM;
int mapADC;
long MAP; //Has to be a long for PID calcs (Boost control)
int baro; //Barometric pressure is simply the inital MAP reading, taken before the engine is running
byte TPS; //The current TPS reading (0% - 100%)
byte TPSlast; //The previous TPS reading
unsigned long TPS_time; //The time the TPS sample was taken
unsigned long TPSlast_time; //The time the previous TPS sample was taken
byte tpsADC; //0-255 byte representation of the TPS
byte tpsDOT;
int rpmDOT;
byte VE;
byte O2;
byte O2_2;
int coolant;
int cltADC;
int i;
int IAT;
int iatADC;
int batADC;
int O2ADC;
int O2_2ADC;
int dwell;
byte dwellCorrection; //The amount of correction being applied to the dwell time.
byte battery10; //The current BRV in volts (multiplied by 10. Eg 12.5V = 125)
byte advance;
byte corrections;
byte TAEamount; //The amount of accleration enrichment currently being applied
byte egoCorrection; //The amount of closed loop AFR enrichment currently being applied
byte wueCorrection; //The amount of warmup enrichment currently being applied
byte batCorrection; //The amount of battery voltage enrichment currently being applied
byte iatCorrection; //The amount of inlet air temperature adjustment currently being applied
byte launchCorrection; //The amount of correction being applied if launch control is active
byte afrTarget;
byte idleDuty;
byte flex; //Ethanol reading (if enabled). 0 = No ethanol, 100 = pure ethanol. Eg E85 = 85. 
unsigned long TAEEndTime; //The target end time used whenever TAE is turned on
volatile byte squirt;
volatile byte spark;
byte engine;
unsigned int PW; //In uS
volatile byte runSecs; //Counter of seconds since cranking commenced (overflows at 255 obviously)
volatile byte secl; //Continous 
volatile int loopsPerSecond;
boolean launchingSoft; //True when in launch control soft limit mode
boolean launchingHard; //True when in launch control hard limit mode
int freeRAM;
String response;

SoftwareSerial mySerial(8, 9); // RX, TX


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, A0, GRB>(leds, 6);
  FastLED.setBrightness(32);
  display.begin();
  display.setBias(display.getBias() + 1);
  display.setContrast(display.getContrast() + 1);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.display();
  pinMode(A3, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  mySerial.begin(115200);
}

void buildArray(){
      for (i = 0; i <73; i++){
      response[i] = mySerial.read();
    }
}



void loop() {
  mySerial.write("A");
  buildArray();
  RPM = ((response [15] << 8) | (response [14]));
  advance = response[22];
  battery10 = response[9]; //battery voltage
  dwell = response[3];
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  leds[2] = CRGB::Black;
  leds[3] = CRGB::Black;
  leds[4] = CRGB::Black;
  leds[5] = CRGB::Green;
  FastLED.show();
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print(RPM);
  display.display();
  delay(40);

};
