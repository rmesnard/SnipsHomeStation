// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 128 // Popular NeoPixel ring size
#define NUMVERTICAL  8
#define NUMHORIZON  64

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int incomingByte = 0;
int runcommand = 0;
int indexp = 0;

int motifR[NUMPIXELS];
int motifG[NUMPIXELS];
int motifB[NUMPIXELS];

bool isshow = false;

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels

char hexb[3];

// default light intensity 50 %
int intensity = 50;
int scrollx =0;
char scroll = 'L';
int fps=10;

unsigned long currentMillis;
unsigned long previousMillis;


void setup() {
  Serial.begin(9600);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  pixels.clear();
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

void clearscreen()
{
      for(int i=0; i<NUMPIXELS; i++) 
      { 
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            motifR[i] =0;
            motifG[i] = 0;
            motifB[i] = 0;      
      }
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
  
      for(int i=NUMPIXELS-NUMVERTICAL; i>0; i--) 
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

    if (Serial.available() > 0) {
          // read the incoming byte:
          incomingByte = Serial.read();

          if (incomingByte == 'C') 
            clearscreen();
            
          if (incomingByte == 'S') 
            displaymotif();

          if (incomingByte == 'L')
          { 
            scroll='L';
            if ( waitnbytes(4) )
            {
              scrollx = getHex();
              fps = getHex();
            }
          }

          if (incomingByte == 'R') 
          { 
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
            // Define Pixel color
            if ( waitnbytes(6) )
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
}
