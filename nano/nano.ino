// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <SimpleDHT.h>  

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN_PIXEL        2 // On Trinket or Gemma, suggest changing this to 1

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

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

int incomingByte = 0;
int runcommand = 0;
int indexp = 0;

int pin_push = LOW;
int pin_key = LOW;
int pin_box = LOW;

int motifR[NUMPIXELS];
int motifG[NUMPIXELS];
int motifB[NUMPIXELS];

bool isshow = false;
bool waiteffect = false;

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

char hexb[3];

// default light intensity 50 %
int intensity = 50;
int scrollx =0;
char scroll = 'L';
int fps=10;

unsigned long currentMillis;
unsigned long previousMillis;

#if defined( PIN_DTH22 )
  SimpleDHT22 dht22(PIN_DTH22);
#endif


void setup() {
  Serial.begin(57600);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
    // END of Trinket-specific code.
  
  #if defined( PIN_PUSH )
    pinMode(PIN_PUSH,INPUT);
  #endif
  
  #if defined( PIN_KEYLOCK )
    pinMode(PIN_KEYLOCK,INPUT);
  #endif
  
  #if defined( PIN_BOXOPEN )
    pinMode(PIN_BOXOPEN,INPUT);
  #endif
  
  #if defined( PIN_RELAY )
    pinMode(PIN_RELAY,OUTPUT);
  #endif


  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();

  showtstart();
  
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
  Serial.print("T.");
  Serial.print((float)temperature); Serial.print(".H.");
  Serial.print((float)humidity); Serial.println(".");
  }
 
}
#endif


void getstatus()
{
  int pinstate = LOW;
#if defined( PIN_DTH22 )
  checkDTH22();
#endif

#if defined( PIN_PUSH )
  pinstate = digitalRead(PIN_PUSH);
  Serial.print("B");
  if (pinstate == HIGH )
    Serial.print("1.");
  else
    Serial.println("0.");
#endif

#if defined( PIN_KEYLOCK )
  pin_key = digitalRead(PIN_KEYLOCK);
  Serial.print("K");
  if ( pin_key == HIGH )
    Serial.print("1.");
  else
    Serial.println("0.");
#endif

#if defined( PIN_BOXOPEN )
  pin_box = digitalRead(PIN_BOXOPEN);
  Serial.print("L");
  if ( pin_box == HIGH )
    Serial.print("1.");
  else
    Serial.println("0.");
#endif


}

void checkstatus()
{
int pinstate = LOW;

#if defined( PIN_PUSH )
  pinstate = digitalRead(PIN_PUSH);
  if ( pin_push != pinstate )
  {  
  pin_push = pinstate;
  Serial.print("B");
  if ( pin_push == LOW )
    Serial.print("1.");
  else
    Serial.println("0.");
  }
  
#endif

#if defined( PIN_KEYLOCK )
  pinstate = digitalRead(PIN_KEYLOCK);
  if ( pin_key != pinstate )
  {  
  pin_key = pinstate;
  Serial.print("K");
  if ( pin_key == LOW )
    Serial.print("1.");
  else
    Serial.println("0.");
  }
  
#endif

#if defined( PIN_BOXOPEN )
  pinstate = digitalRead(PIN_BOXOPEN);
  if ( pin_box != pinstate )
  {  
  pin_box = pinstate;
  Serial.print("B");
  if ( pin_box == LOW )
    Serial.print("1.");
  else
    Serial.println("0.");
  }
  
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

void scrollleft()
{
      // store left line
      int memoR[NUMVERTICAL];
      int memoG[NUMVERTICAL];
      int memoB[NUMVERTICAL];
      
      for(int i=0; i<NUMVERTICAL; i++) 
      { 
        memoR[i] = motifR[i];
        memoG[i] = motifG[i];
        memoB[i] = motifB[i];
      }      
  
      for(int i=0; i<NUMPIXELS-NUMVERTICAL; i++) 
      { 
        motifR[i] = motifR[i+NUMVERTICAL];
        motifG[i] = motifG[i+NUMVERTICAL];
        motifB[i] = motifB[i+NUMVERTICAL];
        pixels.setPixelColor(i, pixels.Color(pixIt(motifR[i]), pixIt(motifG[i]), pixIt(motifB[i])));
      }

      for(int i=0; i<NUMVERTICAL; i++) 
      { 
        motifR[i+NUMPIXELS-NUMVERTICAL] = memoR[i];
        motifG[i+NUMPIXELS-NUMVERTICAL] = memoG[i];
        motifB[i+NUMPIXELS-NUMVERTICAL] = memoB[i];
        pixels.setPixelColor(i+NUMPIXELS-NUMVERTICAL, pixels.Color(pixIt(memoR[i]),pixIt(memoG[i]),pixIt(memoB[i])));
      }
      
     
     pixels.show();   // Send the updated pixel colors to the hardware.
}

int pixIt(int icolor)
{
   int res = (int) (icolor * intensity ) / 100;
   return res;
}

void scrollright()
{
      // store right line
      int memoR[NUMVERTICAL];
      int memoG[NUMVERTICAL];
      int memoB[NUMVERTICAL];
      
      for(int i=0; i<NUMVERTICAL; i++) 
      { 
        memoR[i] = motifR[i+NUMPIXELS-NUMVERTICAL];
        memoG[i] = motifG[i+NUMPIXELS-NUMVERTICAL];
        memoB[i] = motifB[i+NUMPIXELS-NUMVERTICAL];
      }      
  
      for(int i=NUMPIXELS-NUMVERTICAL-1; i>=0; i--) 
      { 
        motifR[i+NUMVERTICAL] = motifR[i];
        motifG[i+NUMVERTICAL] = motifG[i];
        motifB[i+NUMVERTICAL] = motifB[i];
        
        pixels.setPixelColor(i+NUMVERTICAL, pixels.Color(pixIt(motifR[i]),pixIt(motifG[i]),pixIt(motifB[i])));
      }

      for(int i=0; i<NUMVERTICAL; i++) 
      { 
        motifR[i] = memoR[i];
        motifG[i] = memoG[i];
        motifB[i] = memoB[i];
        pixels.setPixelColor(i, pixels.Color(pixIt(memoR[i]), pixIt(memoG[i]),pixIt(memoB[i])));
      }
      
     
     pixels.show();   // Send the updated pixel colors to the hardware.
}

int getHex()
{
  int hexvalue =0;
  hexb[0] = Serial.read();
  hexb[1] = Serial.read();
  hexb[2] = 0;
   
  hexvalue = strtol(hexb, NULL, 16);
  return (hexvalue);
}

bool waitnbytes(int nbbytes)
{
    int timeout =0;
    while ((Serial.available() < nbbytes) && (timeout < 100 )) {
      delay(10);
      timeout++;
    }
    if ( timeout < 100 )
       return true;

    return false;
}

void loop() {
  //pixels.clear(); // Set all pixel colors to 'off'

    if ((Serial.available() > 0) && ( ! waiteffect )){
          // read the incoming byte:
          incomingByte = Serial.read();

          if (incomingByte == 'W') 
          {
            // wait end of effect ( not for infinite scrolling )
            if ( scrollx != 255 ) 
              waiteffect = true;
          }

          if (incomingByte == 'G') 
          {
            // Get Status
            getstatus();
          }

          if (incomingByte == 'O') 
          {
            if ( waitnbytes(2) )
            {
              int newrelay = getHex();
              if ( newrelay == 0 )
                digitalWrite(PIN_RELAY,LOW);
              else
                digitalWrite(PIN_RELAY,HIGH);
            }          
          }

          if (incomingByte == 'K') 
          {
            showtstart();
            isshow=false;
          }

          if (incomingByte == 'C') 
          {
            // Clear Display
            clearscreen(true);
            scrollx=0;
          }

          if (incomingByte == 'E') 
          {
            // Erase dislay ( erase buffer but not the display )
            clearscreen(false);
            scrollx=0;
          }

          if (incomingByte == 'S') 
          {
            // show image
            displaymotif();
          }
          if (incomingByte == 'L')
          { 
            // scroll left xx steps yy fps
            scroll='L';
            if ( waitnbytes(4) )
            {
              scrollx = getHex();
              fps = getHex();
            }
          }

          if (incomingByte == 'R') 
          { 
            // scroll right xx steps yy fps
            scroll='R';
            if ( waitnbytes(4) )
            {
              scrollx = getHex();
              fps = getHex();
            }
          }

          if (incomingByte == 'I') 
          {
            // Set Intensity
            if ( waitnbytes(2) )
            {
              intensity = getHex();
              if (intensity > 100 )
                intensity = 100;
            }
            if (isshow)
              displaymotif();
          }
          
          if (incomingByte == 'P') 
          {
            // Define Pixel color  xx pix id  rr gg bb color
            if ( waitnbytes(8) )
            {
              indexp = getHex();
              if (( indexp > -1 ) & ( indexp < NUMPIXELS ))
              {
              motifR[indexp] = getHex();
              motifG[indexp] = getHex();
              motifB[indexp] = getHex();
              }
            }
          } 
  }



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

checkstatus();
  
}
