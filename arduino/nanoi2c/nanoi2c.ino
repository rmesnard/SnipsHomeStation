#include <Adafruit_NeoPixel.h>
#include <SimpleDHT.h>  
#include <Wire.h>

// pin on the Arduino is connected to the WS2811 
#define PIN_PIXEL        2 

// I2C Slave Adress
#define SLAVE_ADDRESS 0x12

#define NUMPIXELS 128 
#define NUMVERTICAL  8
#define NUMHORIZON  16

// Temp and humidity
#define PIN_DTH22  7

// Push Button
#define PIN_PUSH  5

// RELAY
#define PIN_RELAY  8

// Contact Open Box & Key
// #define PIN_KEYLOCK 10
// #define PIN_BOXOPEN 11
// #define PIN_PIR 12

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

int dataReceived = 0;

byte runcommand = 0;
byte indexp = 0;

byte motifR[NUMPIXELS];
byte motifG[NUMPIXELS];
byte motifB[NUMPIXELS];

int atemp = 0;
int btemp = 0;
int ahum = 0;
int bhum = 0;
int pushstate = LOW;
int keystate = LOW;
int boxstate = LOW;
int pirstate = LOW;
int relaystate = 0;

bool isshow = false;
bool waiteffect = false;

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

// default light intensity 50 %
byte intensity = 50;
byte scrollx =0;
char scroll = 'L';
byte fps=10;
byte roll=0;

unsigned long currentMillis;
unsigned long previousMillis;

#if defined( PIN_DTH22 )
  SimpleDHT22 dht22(PIN_DTH22);
#endif


void setup() {
  Serial.begin(57600);

  #if defined( PIN_PUSH )
    pinMode(PIN_PUSH,INPUT);
  #endif
  
  #if defined( PIN_KEYLOCK )
    pinMode(PIN_KEYLOCK,INPUT);
  #endif
  
  #if defined( PIN_BOXOPEN )
    pinMode(PIN_BOXOPEN,INPUT);
  #endif

  #if defined( PIN_PIR )
    pinMode(PIN_PIR,INPUT);
  #endif
  
  #if defined( PIN_RELAY )
    pinMode(PIN_RELAY,OUTPUT);
  #endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendState);

  showtstart();
  
}

void receiveData(int byteCount){
    while(Wire.available()) {
        dataReceived = Wire.read();
//        Serial.print("Donnee recue : ");
//        Serial.println(dataReceived);

          // Q Query temp and Hum
          if (dataReceived == 0x51) 
          {
            #if defined( PIN_DTH22 )
              checkDTH22();
            #endif
            sendData();
          }


          // W wait 
          if (dataReceived == 0x57) 
          {
            // wait end of effect ( not for infinite scrolling )
            if ( scrollx != 255 ) 
              waiteffect = true;
          }

          // O Relay On Off
          if (dataReceived == 0x4F) 
          {
            relaystate = Wire.read();
            digitalWrite(PIN_RELAY,relaystate);
          }

          // K Reset display 
          if (dataReceived == 0x4B) 
          {
            showtstart();
            isshow=false;
          }

          // C Clear display
          if (dataReceived == 0x43) 
          {
            clearscreen(true);
            scrollx=0;
          }

          // E  Erase dislay ( erase buffer but not the display )
          if (dataReceived == 0x45) 
          {
            clearscreen(false);
            scrollx=0;
          }

          // S  Show buffer
          if (dataReceived == 0x53) 
          {
            displaymotif();
          }

          // L scroll left x steps y fps
          if (dataReceived == 0x4C)
          { 
              scroll='L';
              scrollx = Wire.read();
              fps = Wire.read();
              roll = Wire.read();
          }

          // R scroll right x steps y fps
          if (dataReceived == 0x52) 
          { 
            scroll='R';
              scrollx = Wire.read();
              fps = Wire.read();
              roll = Wire.read();              
          }

          // I  Set Intensity
          if (dataReceived == 0x49 ) 
          {
              intensity = Wire.read();
              if (intensity > 100 )
                intensity = 100;
            if (isshow)
              displaymotif();
          }

          // P Define Pixel color  x pix id  r g b color
          if (dataReceived == 0x50) 
          {
              indexp =  Wire.read();
              int r =   Wire.read();
              int g =  Wire.read();
              int b =  Wire.read();
              if (( indexp > -1 ) & ( indexp < NUMPIXELS ))
              {
              motifR[indexp] = r;
              motifG[indexp] = g;
              motifB[indexp] = b;
              }
          } 

    }
}

void sendData(){
  // return temperature and humidity
    Wire.write(atemp);
    Wire.write(btemp);
    Wire.write(ahum);
    Wire.write(bhum);
}


void sendState(){

    getstatus();
  
    Wire.write(pushstate);
    Wire.write(keystate);
    Wire.write(boxstate);
    Wire.write(pirstate);
    Wire.write(relaystate);
}

#if defined( PIN_DTH22 )

void checkDTH22()
{
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  err = dht22.read2(&temperature, &humidity, NULL);
  if ( err == SimpleDHTErrSuccess )
  {
    atemp = (int)temperature;
    btemp = (temperature - atemp) * 100;
    ahum = (int)humidity;
    bhum = (humidity - ahum) * 100;
  }
}
#endif


void getstatus()
{
 

#if defined( PIN_PUSH )
  pushstate = digitalRead(PIN_PUSH);
#endif

#if defined( PIN_KEYLOCK )
  keystate = digitalRead(PIN_KEYLOCK);
#endif

#if defined( PIN_BOXOPEN )
  boxstate = digitalRead(PIN_BOXOPEN);
#endif

#if defined( PIN_PIR )
  pirstate = digitalRead(PIN_PIR);
#endif
}


void showtstart()
{
  pixels.setPixelColor(0, pixels.Color(128,128,128));
  pixels.setPixelColor(NUMVERTICAL-1, pixels.Color(128,128,128));
  pixels.setPixelColor(NUMPIXELS-NUMVERTICAL, pixels.Color(128,128,128));
  pixels.setPixelColor(NUMPIXELS-1, pixels.Color(128,128,128));
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
}

void displaymotif()
{
      for(int i=0; i<NUMPIXELS; i++) 
      { 
      pixels.setPixelColor(i, pixels.Color(pixIt(motifR[i]), pixIt(motifG[i]), pixIt(motifB[i])));
      }
     pixels.show();   // Send the updated pixel colors to the hardware.

     isshow=true;
}


void clearscreen(bool showit)
{
   pixels.clear();
   for(int i=0; i<NUMPIXELS; i++) 
   { 
        //pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            motifR[i] =0;
            motifG[i] = 0;
            motifB[i] = 0;      
   }
   if ( showit )
     pixels.show();   // Send the updated pixel colors to the hardware.
   isshow=false;
}

int pixIt(int icolor)
{
   int res = (int) (icolor * intensity ) / 100;
   return res;
}

void scrollleft()
{
      byte memoR[NUMVERTICAL];
      byte memoG[NUMVERTICAL];
      byte memoB[NUMVERTICAL];

      if ( roll == 1 )
      {
        // store left line
        for(int i=0; i<NUMVERTICAL; i++) 
        { 
          memoR[i] = motifR[i];
          memoG[i] = motifG[i];
          memoB[i] = motifB[i];
        }      
      }

      for(int i=0; i<NUMPIXELS-NUMVERTICAL; i++) 
      { 
        motifR[i] = motifR[i+NUMVERTICAL];
        motifG[i] = motifG[i+NUMVERTICAL];
        motifB[i] = motifB[i+NUMVERTICAL];
        pixels.setPixelColor(i, pixels.Color(pixIt(motifR[i]), pixIt(motifG[i]), pixIt(motifB[i])));
      }

      if ( roll == 1 )
      {
        for(int i=0; i<NUMVERTICAL; i++) 
        { 
          motifR[i+NUMPIXELS-NUMVERTICAL] = memoR[i];
          motifG[i+NUMPIXELS-NUMVERTICAL] = memoG[i];
          motifB[i+NUMPIXELS-NUMVERTICAL] = memoB[i];
          pixels.setPixelColor(i+NUMPIXELS-NUMVERTICAL, pixels.Color(pixIt(memoR[i]),pixIt(memoG[i]),pixIt(memoB[i])));
        }
      }      
     
     pixels.show();   // Send the updated pixel colors to the hardware.
}


void scrollright()
{
      byte memoR[NUMVERTICAL];
      byte memoG[NUMVERTICAL];
      byte memoB[NUMVERTICAL];
      
      // store right line
      if ( roll == 1 )
      {
        for(int i=0; i<NUMVERTICAL; i++) 
        { 
          memoR[i] = motifR[i+NUMPIXELS-NUMVERTICAL];
          memoG[i] = motifG[i+NUMPIXELS-NUMVERTICAL];
          memoB[i] = motifB[i+NUMPIXELS-NUMVERTICAL];
        }      
      }
  
      for(int i=NUMPIXELS-NUMVERTICAL-1; i>=0; i--) 
      { 
        motifR[i+NUMVERTICAL] = motifR[i];
        motifG[i+NUMVERTICAL] = motifG[i];
        motifB[i+NUMVERTICAL] = motifB[i];
        
        pixels.setPixelColor(i+NUMVERTICAL, pixels.Color(pixIt(motifR[i]),pixIt(motifG[i]),pixIt(motifB[i])));
      }

      if ( roll == 1 )
      {
        for(int i=0; i<NUMVERTICAL; i++) 
        { 
          motifR[i] = memoR[i];
          motifG[i] = memoG[i];
          motifB[i] = memoB[i];
          pixels.setPixelColor(i, pixels.Color(pixIt(memoR[i]), pixIt(memoG[i]),pixIt(memoB[i])));
        }
      }     
      
     pixels.show();   // Send the updated pixel colors to the hardware.
}


void loop() {

 currentMillis = millis();
 long millisSpend = currentMillis - previousMillis;
 
  if ( millisSpend > ( 1000 / fps ) )  
  {
    previousMillis = currentMillis;
    if ( scrollx > 0 )
      {
        if ( scrollx != 255 )
          scrollx--;
        if ( scroll=='L' )
          scrollleft();
        else
          scrollright();
      }         
  }

if ( scrollx <= 0)
  waiteffect = false;

// Check Input
//getstatus();

}
