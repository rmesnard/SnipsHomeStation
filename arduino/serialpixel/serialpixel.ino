// Serial remote command for Neopixel ( WS2812 )
// 

    #include <Adafruit_NeoPixel.h>


const PROGMEM char font8x8_basic[] = {
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0000 (nul)
     0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C,   // U+0001  (eye ball full )
     0x2C, 0x40, 0x02, 0x81, 0x81, 0x01, 0x20, 0x34,   // U+0002   ( red eye pink )
     0x10, 0x02, 0xC1, 0x00, 0x00, 0x82, 0x42, 0x08,   // U+0003   ( red eye red )
     0x3C, 0x42, 0xBD, 0xA5, 0xA5, 0xBD, 0x42, 0x3C,   // U+0004   ( hypno eye 1)
     0x00, 0x3C, 0x42, 0x5A, 0x5A, 0x42, 0x3C, 0x00,   // U+0005   ( hypno eye 2)
     0x24, 0x42, 0x81, 0x00, 0x00, 0x81, 0x42, 0x24,	// Char 006  ( Gradient eye 1 )
	   0x18, 0x24, 0x42, 0x81, 0x81, 0x42, 0x24, 0x18,	// Char 007  ( Gradient eye 2 )
	   0x00, 0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00,	// Char 008  ( Gradient eye 3 )
   	 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,	// Char 009  ( Gradient eye 4 )
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000A
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000B
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000C
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000D
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000E
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+000F
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0010
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0011
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0012
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0013
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0014
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0015
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0016
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0017
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0018
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+0019
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001A
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001B
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001C
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001D
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001E
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // U+001F  
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
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    // U+007F 
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

#define CMDBUFSIZE 16   // buffer size for receiving serial commands

char cmdBuffer[CMDBUFSIZE];
byte animation = 0;

byte currentcolorR = 100;
byte currentcolorG = 100;
byte currentcolorB = 100;


// default eye on center
byte currentX = 3;
byte currentY = 3;
byte eyexL = LED_ROW / 4;
byte eyexR = eyexL + 10;
byte eyeSk = 0;



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
        currentcolorR =  atoi(&cmdBuffer[2]);
        currentcolorG =  atoi(&cmdBuffer[6]);
        currentcolorB =  atoi(&cmdBuffer[10]);
        
      }
        
      if (cmdBuffer[0] == 'O')
      {
         Serial.println("OK");
      }
                
      if (cmdBuffer[0] == 'A')
      {
        int animidx=  atoi(&cmdBuffer[2]);
        if ( animidx == 1 )
          theaterChase(50);
        if ( animidx == 2 )
          theaterChaseRainbow(50);
        if ( animidx == 3 )
          rainbow(10);
        if ( animidx == 4 )
          flash(100);
        if ( animidx == 5 )
          CylonBounce(2,20,20);
      }

      if (cmdBuffer[0] == 'P')
      {
        int pixelnum = atoi(&cmdBuffer[2]);
        strip.setPixelColor(pixelnum, strip.Color(currentcolorR, currentcolorG, currentcolorB));
      }

      if (cmdBuffer[0] == 'S')
        strip.show();

      if (cmdBuffer[0] == 'W')
        colorWipe( 1 );
        
      if (cmdBuffer[0] == 'E')
      {
        if (cmdBuffer[2] == '0') {
          strip.clear();
        }
        else
        {
          strip.clear();
          strip.show();
        }
      }


      if (cmdBuffer[0] == 'R')
      {
        int nbstep =  atoi(&cmdBuffer[3]);
        if (cmdBuffer[2] == '0') 
          rotate_left(nbstep,50);
        if (cmdBuffer[2] == '1') 
          rotate_right(nbstep,50);
      }      
      
      if (cmdBuffer[0] == 'D') {
        int colnum =  atoi(&cmdBuffer[2]);
        draw_column(colnum);
      }

      if (cmdBuffer[0] == 'F') {
        strip.fill();
      }

      if (cmdBuffer[0] == 'K') {
        rendertxt(cmdBuffer[2],0);
        rendertxt(cmdBuffer[3],7);
        rendertxt(0x3A,12);
        rendertxt(cmdBuffer[5],17);
        rendertxt(cmdBuffer[6],24);
      }

      if (cmdBuffer[0] == 'T') {
        int colT = 0;
        for ( int idx =2; idx < strlen(cmdBuffer);idx++)
        {
        rendertxt(cmdBuffer[idx],colT);
        colT += 8;
        }      
      }

      if (cmdBuffer[0] == 'U') {
        for ( int idx =2; idx < strlen(cmdBuffer);idx++)
          scrollTxt(cmdBuffer[idx],50);
      }

      if (cmdBuffer[0] == 'Y') {
        if (cmdBuffer[2] == '1') {
          // Display default eye   Y=1
          eyexL = LED_ROW / 4;
          eyexR = eyexL + 10;
          currentX = 2;
          currentY = 2;
          displayEyes(2,2);
        }
        if (cmdBuffer[2] == '2') {
          // Move pupil           Y=2.X.Y
          cmdBuffer[5]= '\0';
          cmdBuffer[7]= '\0';          
          int pupx =  atoi(&cmdBuffer[4]);          
          int pupy =  atoi(&cmdBuffer[6]);
          movePupil(pupx,pupy,50);
        }
        if (cmdBuffer[2] == '3') {
          // Move eyes           Y=3.AA.BB
          cmdBuffer[6]= '\0';
          cmdBuffer[9]= '\0';
          int neweyexL =  atoi(&cmdBuffer[4]);          
          int neweyexR =  atoi(&cmdBuffer[7]);          
          moveEyes(neweyexL,neweyexR,10);
        }
        if (cmdBuffer[2] == '4') {
          // round spin eye      Y=4.nn
          int rdtime =  atoi(&cmdBuffer[4]);          
          roundSpin(rdtime);
        }
        if (cmdBuffer[2] == '5') {
          // change skin      Y=5.nn
          eyeSk =  atoi(&cmdBuffer[4]);          
          displayEyes(currentX,currentY);
        }

      }
      
    }
  }
  

}

void setPixel(int pixelnum,uint32_t pixelcol)
{
  if(( pixelnum >-1 ) & ( pixelnum < LED_COUNT ))
    strip.setPixelColor(pixelnum, pixelcol);
}

uint32_t getPixel(int pixelnum)
{
  uint32_t pixelcol;
  if(( pixelnum >-1 ) & ( pixelnum < LED_COUNT ))
    pixelcol = strip.getPixelColor(pixelnum);
  else
    pixelcol = strip.Color(0, 0, 0);

  return (pixelcol);
}

void setCurrentColor(int red,int green,int blue)
{
    currentcolorR=red;
    currentcolorG=green;
    currentcolorB=blue;
}


void draw_column(int indxcolumn ) {

  int y;
  int pixelnum = indxcolumn*8;

  for (y=0; y < 8; y++) {
    setPixel(pixelnum++, strip.Color(currentcolorR, currentcolorG, currentcolorB));
  }

}


void rotate_left(int nbstep,int wait) {

  int x,y;
  int pixelnum;
  uint32_t pixelcol[LED_COL];
  uint32_t copypixel;
  
   for (int idx = 0 ; idx < nbstep ; idx++ )
   { 
      pixelnum = 0;
  
      for (y=0; y < LED_COL; y++) {
        pixelcol[y] = getPixel(y);
      }
    
      for (x=0; x < (LED_ROW - 1); x++) {
        for (y=0; y < LED_COL; y++) {
          copypixel = getPixel(pixelnum + LED_COL);
          setPixel(pixelnum, copypixel);
          pixelnum++;
        }
      }
    
     for (y=0; y < LED_COL; y++) {
        setPixel(pixelnum++, pixelcol[y]);
      }
      strip.show();
      delay(wait);
   }
}

void rotate_right(int nbstep,int wait) {

  int x,y;
  int pixelnum;
  uint32_t pixelcol[LED_COL];

   for (int idx = 0 ; idx < nbstep ; idx++ )
   { 
      pixelnum = LED_COUNT - LED_COL;
      for (y=0; y < LED_COL; y++) {
        pixelcol[y] = getPixel(y + pixelnum);
      }
    
      for  (x= (LED_ROW - 1) ; x > 0; x--) {
        for (y=0; y < LED_COL; y++) {
          setPixel(pixelnum, getPixel(pixelnum - LED_COL));
          pixelnum++;
        }
        pixelnum=pixelnum-( LED_COL * 2);
      }
    
     for (y=0; y < LED_COL; y++) {
        setPixel(y, pixelcol[y]);
      }
      strip.show();
      delay(wait);
   }
 
}


void rendertxt(int charnum , int column) {
  
    int x,y;
    int set;
    int thebit;
    int pixelnum;
   
    uint32_t currentcolor = strip.Color(currentcolorR, currentcolorG, currentcolorB);
    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            pixelnum = ( y * 8 ) + ( 7 - x ) + ( column * LED_COL );
            thebit = pgm_read_byte_near(font8x8_basic + ( charnum * 8 ) + x );
            set = thebit & 1 << y;
            if ( set != 0 )
              setPixel(pixelnum, currentcolor);

        }

    }
}


void scrollTxt(int charnum,int wait) {

  
 for ( int colnum = LED_ROW -1 ; colnum > LED_ROW - 8 ; colnum-- )
 {
  rotate_left(1,5);
  rendertxt(charnum,colnum);
  strip.show();
  delay(wait);   
 }
}

// Some functions of our own for creating animated effects -----------------


void flash(int wait) {
    uint32_t currentcolor = strip.Color(currentcolorR, currentcolorG, currentcolorB);
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      setPixel(i, currentcolor);         //  Set pixel's color (in RAM)
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
void colorWipe( int wait) {
  uint32_t currentcolor = strip.Color(currentcolorR, currentcolorG, currentcolorB);
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    setPixel(i, currentcolor);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase( int wait) {
  uint32_t currentcolor = strip.Color(currentcolorR, currentcolorG, currentcolorB);
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        setPixel(c, currentcolor); // Set pixel 'c' to value 'color'
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
      setPixel(i, strip.gamma32(strip.ColorHSV(pixelHue)));
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
        setPixel(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

// CylonBounce

void CylonBounce(int EyeSize, int SpeedDelay, int ReturnDelay){

uint32_t currentcolor = strip.Color(currentcolorR, currentcolorG, currentcolorB);

  for(int i = 0; i < LED_ROW - EyeSize-2; i++) {
    strip.clear();
    setPixel(i * LED_COL +4, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    for(int j = 1; j <= EyeSize; j++) {
  //    strip.setPixelColor( (i+j) * LED_COL + 3, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
      setPixel( (i+j) * LED_COL + 4, currentcolor);
  //    strip.setPixelColor( (i+j) * LED_COL + 5, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    }
    setPixel((i+EyeSize+1)*LED_COL + 4, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    strip.show();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = LED_ROW - EyeSize-2; i > 0; i--) {
    strip.clear();
    setPixel(i  * LED_COL +4, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    for(int j = 1; j <= EyeSize; j++) {
    //  strip.setPixelColor( (i+j) * LED_COL + 3, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
      setPixel((i+j) * LED_COL + 4, currentcolor);
    //  strip.setPixelColor( (i+j) * LED_COL + 5, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    }
    setPixel((i+EyeSize+1)*LED_COL + 4, strip.Color(currentcolorR/30, currentcolorG/30, currentcolorB/30));
    strip.show();
    delay(SpeedDelay);
  }
 
  strip.clear();
  strip.show();
}

// 

void displayEye(int starcol, int offsetX, int offsetY,int skin) 
{

  if ( skin == 0 )
  {
    // default skin current color used
    rendertxt(0x01,starcol);  
  }
  else if ( skin == 1 )    
  {
    //  skin tired eyes
    setCurrentColor(100, 100, 100);
    rendertxt(0x01,starcol);  
    setCurrentColor(255, 128, 128);
    rendertxt(0x02,starcol);  
    setCurrentColor(255, 0, 0);
    rendertxt(0x03,starcol);  
  }
  else if ( skin == 2 )    
  {
    //  skin hypno eye step 1
    setCurrentColor(200, 200, 0);
    rendertxt(0x04,starcol);  
  //  currentcolor = strip.Color(200,200, 0);
  //  rendertxt(0x05,starcol);  
  }
    else if ( skin == 3 )    
  {
    //  skin hypno eye step 2
    setCurrentColor(200, 200, 0);
    rendertxt(0x05,starcol);  
  //  currentcolor = strip.Color(200, 200, 200);
  //  rendertxt(0x05,starcol);  
  }
    else if ( skin == 4 )    
  {
    //  skin gradient sauron eye
    setCurrentColor(136, 0, 21);
    rendertxt(0x06,starcol);  
    setCurrentColor(237, 28, 36);
    rendertxt(0x07,starcol);  
    setCurrentColor(255, 127, 39);
    rendertxt(0x08,starcol);  
    setCurrentColor(255, 201, 14);
    rendertxt(0x09,starcol);  
  }
    else if ( skin == 5 )    
  {
    //  skin gradient blue eye
    setCurrentColor(230, 230,230);
    rendertxt(0x01,starcol);  
    setCurrentColor(153, 217, 234);
    rendertxt(0x08,starcol);  
    setCurrentColor(0, 162, 232);
    rendertxt(0x09,starcol);  
  }
    else if ( skin == 6 )    
  {
    //  skin gradient green eye
    setCurrentColor(230, 230,230);
    rendertxt(0x01,starcol);  
    setCurrentColor(181, 230, 29);
    rendertxt(0x08,starcol);  
    setCurrentColor(34, 117, 76);
    rendertxt(0x09,starcol);  
  }
    else if ( skin == 7 )    
  {
    //  skin gradient green eye
    setCurrentColor(230, 230,230);
    rendertxt(0x01,starcol);  
    setCurrentColor(185, 122, 27);
    rendertxt(0x08,starcol);  
    setCurrentColor(136, 0, 21);
    rendertxt(0x09,starcol);  
  }

  setPixel( ( offsetX +1 ) * LED_COL + offsetY + (starcol*8) + 1, strip.Color(0, 0, 0));
  setPixel( ( offsetX + 2 ) * LED_COL + offsetY + (starcol*8) + 1, strip.Color(0, 0, 0));
  setPixel( ( offsetX + 1 ) * LED_COL + offsetY + (starcol*8) + 2 , strip.Color(0, 0, 0));
  setPixel( ( offsetX + 2)  * LED_COL + offsetY + (starcol*8) + 2, strip.Color(0, 0, 0));

}

void displayEyes(int offsetX, int offsetY) 
{
  strip.clear();

  if ( eyeSk == 0 )
  {
    // default skin current color used
    displayEye(eyexL,offsetX,offsetY,0);
    displayEye(eyexR,offsetX,offsetY,0);
  }
  else if ( eyeSk == 1 )    
  {
    //  skin tired eye
    displayEye(eyexL,offsetX,offsetY,1);
    displayEye(eyexR,offsetX,offsetY,1);
  }
  else if ( eyeSk == 2 )    
  {
    // sauron eye
    displayEye(eyexL,offsetX,offsetY,4);
    displayEye(eyexR,offsetX,offsetY,4);
  }
  else if ( eyeSk == 3 )    
  {
    // blue eye
    displayEye(eyexL,offsetX,offsetY,5);
    displayEye(eyexR,offsetX,offsetY,5);
  }
  else if ( eyeSk == 4 )    
  {
    // green eye
    displayEye(eyexL,offsetX,offsetY,6);
    displayEye(eyexR,offsetX,offsetY,6);
  }
  else if ( eyeSk == 5 )    
  {
    // veron eye
    displayEye(eyexL,offsetX,offsetY,7);
    displayEye(eyexR,offsetX,offsetY,5);
  }


  currentX = offsetX;
  currentY =  offsetY;
  strip.show();
}

void movePupil(int newX, int newY, int stepDelay)
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

void moveEyes(int newColL,int newColR, int stepDelay)
{
  // set current position as start position
  int starteyexL = eyexL;
  int starteyexR = eyexR;
 
  // eval steps
  int stepsL = abs(starteyexL - newColL);
  int stepsR = abs(starteyexR - newColR);

  // need to change at least one position
  if ((stepsL == 0) && (stepsR == 0))
    return;
   
  // eval direction of movement, # of steps, change per step, perform move
  int dirL = (newColL >= starteyexL) ? 1 : -1;
  int dirR = (newColR >= starteyexR) ? 1 : -1;
  int steps = (stepsL > stepsR) ? stepsL : stepsR;
  int intX, intY;
  float changeL = (float)stepsL / (float)steps;
  float changeR = (float)stepsR / (float)steps;
  for (int i=1; i<=steps; i++)
  {
    eyexL = starteyexL + round(changeL * i * dirL);
    eyexR = starteyexR + round(changeR * i * dirR);
    displayEyes(currentX, currentY);
    delay(stepDelay);
  }
}


void roundSpin(int times)
{
  
  movePupil(1, 4, 40);
  delay(40);
  
  for (int i=0; i<times; i++)
  {
    displayEyes(0, 3); delay(40); if (i==0) delay(40);
    displayEyes(0, 2); delay(40); if (i==0) delay(30);
    displayEyes(0, 1); delay(40); if (i==0) delay(20);
    displayEyes(1, 0); delay(40);if (i==0) delay(10);
    displayEyes(2, 0); delay(40);
    displayEyes(3, 0); delay(40);
    displayEyes(4, 1); delay(40);if (i==(times-1)) delay(10);
    displayEyes(4, 2); delay(40);if (i==(times-1)) delay(20);
    displayEyes(4, 3); delay(40); if (i==(times-1)) delay(30);
    displayEyes(3, 4); delay(40); if (i==(times-1)) delay(40);
    displayEyes(2, 4); delay(40); if (i==(times-1)) delay(50);
    displayEyes(1, 4); delay(40);
  }
}
