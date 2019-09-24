// Serial remote command for Neopixel ( WS2812 )
// 
// 
//    #include <Adafruit_GFX.h>
//    #include <Adafruit_NeoMatrix.h>
    #include <Adafruit_NeoPixel.h>


const PROGMEM char font8x8_basic[] = {
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0020 (space)
     0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00,   // U+0021 (!)
     0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0022 (")
     0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00,   // U+0023 (#)
     0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00,   // U+0024 ($)
     0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00,   // U+0025 (%)
     0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00,   // U+0026 (&)
     0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0027 (')
     0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00,   // U+0028 (()
     0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00,   // U+0029 ())
     0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00,   // U+002A (*)
     0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00,   // U+002B (+)
     0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06,   // U+002C (,)
     0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00,   // U+002D (-)
     0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00,   // U+002E (.)
     0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00,   // U+002F (/)
     0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00,   // U+0030 (0)
     0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00,   // U+0031 (1)
     0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00,   // U+0032 (2)
     0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00,   // U+0033 (3)
     0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00,   // U+0034 (4)
     0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00,   // U+0035 (5)
     0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00,   // U+0036 (6)
     0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00,   // U+0037 (7)
     0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00,   // U+0038 (8)
     0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00,   // U+0039 (9)
     0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00,   // U+003A (:)
     0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06,   // U+003B (;)
     0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00,   // U+003C (<)
     0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00,   // U+003D (=)
     0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00,   // U+003E (>)
     0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00,   // U+003F (?)
     0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00,   // U+0040 (@)
     0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00,   // U+0041 (A)
     0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00,   // U+0042 (B)
     0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00,   // U+0043 (C)
     0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00,   // U+0044 (D)
     0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00,   // U+0045 (E)
     0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00,   // U+0046 (F)
     0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00,   // U+0047 (G)
     0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00,   // U+0048 (H)
     0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0049 (I)
     0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00,   // U+004A (J)
     0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00,   // U+004B (K)
     0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00,   // U+004C (L)
     0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00,   // U+004D (M)
     0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00,   // U+004E (N)
     0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00,   // U+004F (O)
     0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00,   // U+0050 (P)
     0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00,   // U+0051 (Q)
     0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00,   // U+0052 (R)
     0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00,   // U+0053 (S)
     0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0054 (T)
     0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00,   // U+0055 (U)
     0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00,   // U+0056 (V)
     0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00,   // U+0057 (W)
     0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00,   // U+0058 (X)
     0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00,   // U+0059 (Y)
     0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00,   // U+005A (Z)
     0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00,   // U+005B ([)
     0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00,   // U+005C (\)
     0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00,   // U+005D (])
     0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00,   // U+005E (^)
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,   // U+005F (_)
     0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0060 (`)
     0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00,   // U+0061 (a)
     0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00,   // U+0062 (b)
     0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00,   // U+0063 (c)
     0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00,   // U+0064 (d)
     0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00,   // U+0065 (e)
     0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00,   // U+0066 (f)
     0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F,   // U+0067 (g)
     0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00,   // U+0068 (h)
     0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+0069 (i)
     0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E,   // U+006A (j)
     0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00,   // U+006B (k)
     0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00,   // U+006C (l)
     0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00,   // U+006D (m)
     0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00,   // U+006E (n)
     0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00,   // U+006F (o)
     0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F,   // U+0070 (p)
     0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78,   // U+0071 (q)
     0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00,   // U+0072 (r)
     0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00,   // U+0073 (s)
     0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00,   // U+0074 (t)
     0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00,   // U+0075 (u)
     0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00,   // U+0076 (v)
     0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00,   // U+0077 (w)
     0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00,   // U+0078 (x)
     0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F,   // U+0079 (y)
     0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00,   // U+007A (z)
     0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00,   // U+007B ()
     0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,   // U+007C (|)
     0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00,   // U+007D (})
     0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+007E (~)
     0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C    // U+007F (eye)
};

#ifdef __AVR__
 #include <avr/power.h> 
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 256
#define LED_ROW 32
#define LED_COL 8

#define CMDBUFSIZE 32   // buffer size for receiving serial commands

char cmdBuffer[CMDBUFSIZE];
int animation = 0;
uint32_t currentcolor;

// default eye on center
int currentX = 0;
int currentY = 0;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.


 

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  currentcolor = strip.Color(100, 100, 100);
  Serial.begin(57600);
   Serial.println("Started");
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {


  // Read Serial Command  
  // Format :
  //  X=nn

  if (Serial.available())
  {
    int charsRead;
    charsRead = Serial.readBytesUntil('\n', cmdBuffer, sizeof(cmdBuffer) - 1);  //read entire line
    cmdBuffer[charsRead] = '\0';       // Make it a C string
   
    
    if(cmdBuffer[1] != '=' || strlen(cmdBuffer)<3)
    {
      Serial.println("KO");
    }
    else
    {
       Serial.print(">"); Serial.println(cmdBuffer);

      if (cmdBuffer[0] == 'C')
      {
        cmdBuffer[5]= '\0';
        cmdBuffer[9]= '\0';
        int colorR =  atoi(&cmdBuffer[2]);
        int colorG =  atoi(&cmdBuffer[6]);
        int colorB =  atoi(&cmdBuffer[10]);
        currentcolor = strip.Color(colorR, colorG, colorB);
      }
        
      if (cmdBuffer[0] == 'T')
      {
         Serial.println("OK");
      }
                
      if (cmdBuffer[0] == 'A')
      {
        int animidx=  atoi(&cmdBuffer[2]);
        if ( animidx == 1 )
          theaterChase(currentcolor,50);
        if ( animidx == 2 )
          theaterChaseRainbow(50);
        if ( animidx == 3 )
          rainbow(10);
        if ( animidx == 4 )
          flash(currentcolor,100);
        if ( animidx == 5 )
          CylonBounce(2,20,20);
      }

      if (cmdBuffer[0] == 'P')
      {
        
        int pixelnum = atoi(&cmdBuffer[2]);
        strip.setPixelColor(pixelnum, currentcolor);
      }

      if (cmdBuffer[0] == 'S')
        strip.show();

      if (cmdBuffer[0] == 'W')
        colorWipe(currentcolor, 1 );
        
      if (cmdBuffer[0] == 'E')
        strip.clear();

      if (cmdBuffer[0] == 'D') {
        int colnum =  atoi(&cmdBuffer[2]);
        draw_column(colnum);
      }

      if (cmdBuffer[0] == 'F') {
        strip.fill(currentcolor);
      }

      if (cmdBuffer[0] == 'K') {
        rendertxt(0x3A,15);
      }

      if (cmdBuffer[0] == 'Y') {
        displayEyes(0,0);
      }

      
    }
  }
  

}

void draw_column(int indxcolumn ) {

  int y;
  int pixelnum = indxcolumn*8;

  for (y=0; y < 8; y++) {
    strip.setPixelColor(pixelnum++, currentcolor);
  }

}


void rotate_left() {

  int x,y;
  int pixelnum;
  uint32_t pixelcol[LED_COL];

  for (y=0; y < LED_COL; y++) {
    pixelcol[y] = strip.getPixelColor(y);
  }

  for (x=0; x < (LED_ROW - 1); x++) {
    for (y=0; y < LED_COL; y++) {
      strip.setPixelColor(pixelnum, strip.getPixelColor(pixelnum + LED_COL));
      pixelnum++;
    }
  }

 for (y=0; y < LED_COL; y++) {
    strip.setPixelColor(pixelnum++, pixelcol[y]);
  }

}

void rotate_right() {

  int x,y;
  int pixelnum = LED_COUNT - LED_COL;
  uint32_t pixelcol[LED_COL];

  for (y=0; y < LED_COL; y++) {
    pixelcol[y] = strip.getPixelColor(y + pixelnum);
  }

  for  (x= (LED_ROW - 1) ; x > 0; x--) {
    for (y=0; y < LED_COL; y++) {
      strip.setPixelColor(pixelnum, strip.getPixelColor(pixelnum - LED_COL));
      pixelnum++;
    }
    pixelnum=pixelnum-( LED_COL * 2);
  }

 for (y=0; y < LED_COL; y++) {
    strip.setPixelColor(y, pixelcol[y]);
  }



}


void rendertxt(int charnum , int column) {
  
    int x,y;
    int set;
    int thebit;
    int pixelnum;
    charnum = charnum - 0x20;

    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            pixelnum = ( x * 8 ) + y + ( column * LED_COL );
            thebit = pgm_read_byte_near(font8x8_basic + ( charnum * 8 ) + x );
            set = thebit & 1 << y;
            if ( set != 0 )
              strip.setPixelColor(pixelnum, currentcolor);

        }

    }
}

// Some functions of our own for creating animated effects -----------------


void flash(uint32_t color,int wait) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    }
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    strip.clear();
    strip.show();                          //  Update strip to match
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 1 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

// CylonBounce

void CylonBounce(int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < LED_ROW - EyeSize-2; i++) {
    strip.clear();
    strip.setPixelColor(i * LED_COL +4, strip.Color(50, 50, 50));
    for(int j = 1; j <= EyeSize; j++) {
      strip.setPixelColor( (i+j) * LED_COL + 4, strip.Color(50, 50, 50));
      strip.setPixelColor( (i+j) * LED_COL + 4, strip.Color(50, 50, 50));
      strip.setPixelColor( (i+j) * LED_COL + 4, strip.Color(50, 50, 50));
    }
    strip.setPixelColor((i+EyeSize+1)*LED_COL + 4, strip.Color(50, 50, 50));
    strip.show();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = LED_ROW - EyeSize-2; i > 0; i--) {
    strip.clear();
    strip.setPixelColor(i  * LED_COL +4, strip.Color(50, 50, 50));
    for(int j = 1; j <= EyeSize; j++) {
      strip.setPixelColor((i+j) * LED_COL + 4, strip.Color(50, 50, 50));
    }
    strip.setPixelColor((i+EyeSize+1)*LED_COL + 4, strip.Color(50, 50, 50));
    strip.show();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

// 

void displayEye(int eyenum, int offsetX, int offsetY) 
{
  int starcol = 8*eyenum;
  rendertxt(0x7F,starcol);  

Serial.println(( offsetX + 3)  * LED_COL  + offsetY + starcol );

  strip.setPixelColor( ( offsetX + 3)  * LED_COL  + offsetY + (starcol*8), strip.Color(0, 0, 0));
  strip.setPixelColor( ( offsetX + 4)  * LED_COL  + offsetY + (starcol*8), strip.Color(0, 0, 0));
  strip.setPixelColor( ( offsetX + 3)  * LED_COL + offsetY + (starcol*8) + 1, strip.Color(0, 0, 0));
  strip.setPixelColor( ( offsetX + 4)  * LED_COL + offsetY + (starcol*8) + 1, strip.Color(0, 0, 0));

}

void displayEyes(int offsetX, int offsetY) 
{
  strip.clear();
  displayEye(1,offsetX,offsetY);
  displayEye(2,offsetX,offsetY);

  currentX = offsetX;
  currentY =  offsetY;
  strip.show();
}

void moveEyes(int newX, int newY, int stepDelay)
{
  // set current position as start position
  int startX = currentX;
  int startY = currentY;
 
  // eval steps
  int stepsX = abs(currentX - newX);
  int stepsY = abs(currentY - newY);

  // need to change at least one position
  if ((stepsX == 0) && (stepsY == 0))
    return;
   
  // eval direction of movement, # of steps, change per X Y step, perform move
  int dirX = (newX >= currentX) ? 1 : -1;
  int dirY = (newY >= currentY) ? 1 : -1;
  int steps = (stepsX > stepsY) ? stepsX : stepsY;
  int intX, intY;
  float changeX = (float)stepsX / (float)steps;
  float changeY = (float)stepsY / (float)steps;
  for (int i=1; i<=steps; i++)
  {
    intX = startX + round(changeX * i * dirX);
    intY = startY + round(changeY * i * dirY);
    displayEyes(intX, intY);
    delay(stepDelay);
  }
}

void roundSpin(int times)
{
  
  moveEyes(2, 0, 50);
  delay(500);
  
  for (int i=0; i<times; i++)
  {
    displayEyes(2, -1); delay(40); if (i==0) delay(40);
    displayEyes(1, -2); delay(40); if (i==0) delay(30);
    displayEyes(0, -2); delay(40); if (i==0) delay(20);
    displayEyes(-1, -2); delay(40);if (i==0) delay(10);
    displayEyes(-2, -1); delay(40);
    displayEyes(-2, 0); delay(40);
    displayEyes(-2, 1); delay(40);if (i==(times-1)) delay(10);
    displayEyes(-1, 2); delay(40);if (i==(times-1)) delay(20);
    displayEyes(0, 2); delay(40); if (i==(times-1)) delay(30);
    displayEyes(1, 2); delay(40); if (i==(times-1)) delay(40);
    displayEyes(2, 1); delay(40); if (i==(times-1)) delay(50);
    displayEyes(2, 0); delay(40);
  }
}
