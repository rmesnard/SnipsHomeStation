#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include <SerialCommands.h>
#include "Adafruit_CCS811.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    256
#define LED_ROW 32
#define LED_COL 8

#define CMDBUFSIZE 32   // buffer size for receiving serial commands
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  10

CRGB leds[NUM_LEDS];
Adafruit_CCS811 ccs;

char serial_command_buffer_[CMDBUFSIZE];
SerialCommands serial_commands_(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

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
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

uint8_t gCurrentPatternNumber = 0; 

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


void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("Unrecognized command [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("]");
}

void cmd_show(SerialCommands* sender)
{
  FastLED.show();
  sender->GetSerial()->println("OK - SHOW");
}

void cmd_clear(SerialCommands* sender)
{
  gCurrentPatternNumber=0;
  FastLED.clear();
  sender->GetSerial()->println("OK - CLEAR");
}

void cmd_clean(SerialCommands* sender)
{
  gCurrentPatternNumber=0;
  FastLED.clear();
  FastLED.show();
  sender->GetSerial()->println("OK - CLEAN");
}

void cmd_play_anim(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str == NULL)
  {
    sender->GetSerial()->println("ERROR NO_PARAM");
    return;
  }
  FastLED.clear();
  gCurrentPatternNumber = atoi(param_str);

  sender->GetSerial()->print("OK - SET ANIM ");
  sender->GetSerial()->println(gCurrentPatternNumber);
}

void cmd_pixel_draw(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str == NULL)
  {
    sender->GetSerial()->println("ERROR NO_PARAM");
    return;
  }
  gCurrentPatternNumber = 0;
  int pixelnum = atoi(param_str);

  if ((param_str = sender->Next()) != NULL)
  {
    currentcolorR = atoi(param_str);
    if ((param_str = sender->Next()) != NULL)
      currentcolorG = atoi(param_str);
    if ((param_str = sender->Next()) != NULL)
      currentcolorB = atoi(param_str);
  }

  leds[pixelnum].setRGB(currentcolorR, currentcolorG, currentcolorB);
  
  sender->GetSerial()->print("OK - SET PIXEL ");
  sender->GetSerial()->println(pixelnum);
}

void cmd_set_color(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str == NULL)
  {
    sender->GetSerial()->println("ERROR NO_PARAM");
    return;
  }
  currentcolorR = atoi(param_str);

  if ((param_str = sender->Next()) != NULL)
  {
    currentcolorG = atoi(param_str);
    if ((param_str = sender->Next()) != NULL)
      currentcolorB = atoi(param_str);
  }
  
  sender->GetSerial()->println("OK - SET COLOR ");
}

void cmd_draw_line(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str == NULL)
  {
    sender->GetSerial()->println("ERROR NO_PARAM");
    return;
  }
  int colnum = atoi(param_str);

  draw_column(colnum);
  
  sender->GetSerial()->println("OK - DRAW LINE");
}

void cmd_eyes_move(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
    int eyL =atoi(param_str);
    if ((param_str = sender->Next()) != NULL)
      {
      int eyR = atoi(param_str);
      moveEyes(eyL,eyR,50);
      }
  }
  else
  {
      eyexL = LED_ROW / 4;
      eyexR = eyexL + 10;
      eyeSk = 0;
      currentX = 2;
      currentY = 2;
      displayEyes(2,2);
  }
  
  sender->GetSerial()->println("OK - EYES");
}

void cmd_eyes_pupil(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
      int pux =atoi(param_str);
      if ((param_str = sender->Next()) != NULL)
      {
      int puy = atoi(param_str);
      movePupil(pux,puy,50);
      }
  }
  else
  {
      movePupil(2,2,50);
  }
  
  sender->GetSerial()->println("OK - PUPILS");
}

void cmd_eyes_anim(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
      int animC =atoi(param_str);

      if (animC == 3)     // louche    
        louche();

      if ((param_str = sender->Next()) != NULL)
      {
        
      int animP = atoi(param_str);

      if (animC == 1)     // round spin eye    
          roundSpin(animP);

      if (animC == 2)    {
          // change skin     
          eyeSk =  animP;          
          displayEyes(currentX,currentY);
        }

    }
  }
 
  sender->GetSerial()->println("OK - EYES ANIM");
}

void cmd_clock(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
      FastLED.clear();
      rendertxt(param_str[0],0,false,false);
      rendertxt(param_str[1],7,false,false);
      rendertxt(param_str[2],13,false,false);
      rendertxt(param_str[3],18,false,false);
      rendertxt(param_str[4],25,false,false);
      FastLED.show();
  }
 
  sender->GetSerial()->println("OK - CLOCK");
}

void cmd_text_print(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
      int colT = 0;
      for ( int idx =0; idx < strlen(param_str);idx++)
      {
      rendertxt(param_str[idx],colT,false,false);
      colT += 8;
      }  
  }
 
  sender->GetSerial()->println("OK - TEXT");
}


void cmd_text_scroll(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
      for ( int idx =0; idx < strlen(param_str);idx++)
        scrollTxt(param_str[idx],50);
  }
 
  sender->GetSerial()->println("OK - SCROLL");
}

void cmd_rotate(SerialCommands* sender)
{
  char* param_str = sender->Next();
  if (param_str != NULL)
  {
    int rotx = atoi(param_str);
    if ( rotx > 0 )
      rotate_right(rotx,50);
    else
      rotate_left(abs(rotx),50);
  }
 
  sender->GetSerial()->println("OK - SCROLL");
}

void cmd_readccs(SerialCommands* sender)
{
  if(ccs.available()){
      if(!ccs.readData()){
        sender->GetSerial()->print("CO2:");
        sender->GetSerial()->print(ccs.geteCO2());
        sender->GetSerial()->print(",TVOC:");
        sender->GetSerial()->print(ccs.getTVOC());
        sender->GetSerial()->print(",TEMP:");
        float temp = ccs.calculateTemperature();
        sender->GetSerial()->println(temp);
      }
      else{
        sender->GetSerial()->println("ERROR - CCS NO DATA");
      }
    }
   else
    {
      sender->GetSerial()->println("ERROR - CCS UNAVAILABLE");
    }
 
}

SerialCommand cmd_show_("SHOW", cmd_show);
SerialCommand cmd_clear_("CLEAR", cmd_clear);
SerialCommand cmd_clean_("CLEAN", cmd_clean);
SerialCommand cmd_play_anim_("PLAY", cmd_play_anim);
SerialCommand cmd_pixel_draw_("PIXEL", cmd_pixel_draw);
SerialCommand cmd_set_color_("COLOR", cmd_set_color);
SerialCommand cmd_draw_line_("LINE", cmd_draw_line);
SerialCommand cmd_eyes_move_("EYEM", cmd_eyes_move);
SerialCommand cmd_eyes_pupil_("EYEP", cmd_eyes_pupil);
SerialCommand cmd_eyes_anim_("EYEA", cmd_eyes_anim);
SerialCommand cmd_clock_("CLOCK", cmd_clock);
SerialCommand cmd_text_print_("TEXT", cmd_text_print);
SerialCommand cmd_text_scroll_("SCROLL", cmd_text_scroll);
SerialCommand cmd_rotate_("ROT", cmd_rotate);
SerialCommand cmd_readccs_("CCS", cmd_readccs);


void setup() {

  Serial.begin(57600);
 
  delay(1000); // 1 second delay for recovery

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
  Serial.println("CCS Started.");
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(); 
  FastLED.show(); 

  serial_commands_.SetDefaultHandler(cmd_unrecognized);
  serial_commands_.AddCommand(&cmd_show_);
  serial_commands_.AddCommand(&cmd_clear_);
  serial_commands_.AddCommand(&cmd_clean_);
  serial_commands_.AddCommand(&cmd_play_anim_);
  serial_commands_.AddCommand(&cmd_pixel_draw_);
  serial_commands_.AddCommand(&cmd_set_color_);
  serial_commands_.AddCommand(&cmd_draw_line_);
  serial_commands_.AddCommand(&cmd_eyes_move_);
  serial_commands_.AddCommand(&cmd_eyes_pupil_);
  serial_commands_.AddCommand(&cmd_eyes_anim_);
  serial_commands_.AddCommand(&cmd_clock_);
  serial_commands_.AddCommand(&cmd_text_print_);
  serial_commands_.AddCommand(&cmd_text_scroll_);
  serial_commands_.AddCommand(&cmd_rotate_);
  serial_commands_.AddCommand(&cmd_readccs_);
  
  Serial.println("Ready!");
  
}


void loop()
{

  serial_commands_.ReadSerial();

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { 
    
    gHue++; 

  if ( gCurrentPatternNumber == 1 )
    confetti();
  if ( gCurrentPatternNumber == 2 )
    rainbow();
  if ( gCurrentPatternNumber == 3 )
    rainbowWithGlitter();
  
  if ( gCurrentPatternNumber != 0 )
    FastLED.show();  
   
  } // slowly cycle the "base color" through the rainbow
  
}


void setPixelRGB(int pixelnum,int red,int green,int blue)
{
  if(( pixelnum >-1 ) & ( pixelnum < NUM_LEDS ))
    leds[pixelnum].setRGB(red,green,blue);
}

void copyPixel(int inpx,int outpx)
{
  if(( inpx >-1 ) & ( inpx < NUM_LEDS ) &( outpx >-1 ) & ( outpx < NUM_LEDS ))
     leds[outpx] = leds[inpx];
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
    setPixelRGB(pixelnum++, currentcolorR, currentcolorG, currentcolorB);
  }

}


void rotate_left(int nbstep,int wait) {

  int x,y;
  int pixelnum;
  
  CRGB memoled[LED_COL];
  
   for (int idx = 0 ; idx < nbstep ; idx++ )
   { 
      pixelnum = 0;
  
      for (y=0; y < LED_COL; y++) {
        memoled[y] = leds[y];
      }
    
      for (x=0; x < (LED_ROW - 1); x++) {
        for (y=0; y < LED_COL; y++) {
          leds[pixelnum] = leds[pixelnum + LED_COL];
          pixelnum++;
        }
      }
    
     for (y=0; y < LED_COL; y++) {
        leds[pixelnum++] = memoled[y];
      }
      FastLED.show();
      delay(wait);
   }
   
}

void rotate_right(int nbstep,int wait) {

  int x,y;
  int pixelnum;

  CRGB memoled[LED_COL];

   for (int idx = 0 ; idx < nbstep ; idx++ )
   { 
      pixelnum = NUM_LEDS - LED_COL;
      for (y=0; y < LED_COL; y++) {
        memoled[y] = leds[y + pixelnum];
      }
    
      for  (x= (LED_ROW - 1) ; x > 0; x--) {
        for (y=0; y < LED_COL; y++) {
          leds[pixelnum] = leds[pixelnum - LED_COL];
          pixelnum++;
        }
        pixelnum=pixelnum-( LED_COL * 2);
      }
    
     for (y=0; y < LED_COL; y++) {
        leds[y] = memoled[y];
      }
      FastLED.show();
      delay(wait);
   }
 
}


void rendertxt(int charnum , int column,bool overwrite,bool invert) {
  
    int x,y;
    int set;
    int thebit;
    int pixelnum;
 

    for (x=0; x < 8; x++) {
        for (y=0; y < 8; y++) {
            pixelnum = ( y * 8 ) + ( 7 - x ) + ( column * LED_COL );
            thebit = pgm_read_byte_near(font8x8_basic + ( charnum * 8 ) + x );
            set = thebit & 1 << y;
            if ( set != 0 )
            {
              if (!invert)
                setPixelRGB(pixelnum, currentcolorR, currentcolorG, currentcolorB);
            }
            else
            {
              if ((overwrite) | (invert))
                setPixelRGB(pixelnum, 0, 0, 0);
            }
        }

    }
}


void scrollTxt(int charnum,int wait) {

  
 for ( int colnum = LED_ROW -1 ; colnum > LED_ROW - 8 ; colnum-- )
 {
  rotate_left(1,10);
  rendertxt(charnum,colnum,true,false);
  FastLED.show();
  delay(wait);   
 }
}


void displayEye(int starcol, int offsetX, int offsetY,int skin) 
{

  if ( skin == 0 )
  {
    int storeR,storeG,storeB;
    storeR = currentcolorR;
    storeG = currentcolorG;
    storeB = currentcolorB;
    setCurrentColor(100, 100, 100);
    rendertxt(0x01,starcol,false,false);  
    for ( int x= 0; x < 4; x++ )
      for ( int y= 0; y < 4; y++ )
         setPixelRGB( ( offsetX + x ) * LED_COL + offsetY + (starcol*8) + y, storeR, storeG, storeB);

    setCurrentColor(storeR, storeG, storeB);
  }
  else if ( skin == 1 )    
  {
    //  skin tired eyes
    setCurrentColor(100, 100, 100);
    rendertxt(0x01,starcol,false,false);  
    setCurrentColor(255, 128, 128);
    rendertxt(0x02,starcol,false,false);  
    setCurrentColor(255, 0, 0);
    rendertxt(0x03,starcol,false,false);  
  }
  else if ( skin == 2 )    
  {
    //  skin gradient sauron eye
    setCurrentColor(136, 0, 21);
    rendertxt(0x06,starcol,false,false);  
    setCurrentColor(237, 28, 36);
    rendertxt(0x07,starcol,false,false);  
    setCurrentColor(255, 127, 39);
    rendertxt(0x08,starcol,false,false);  
    setCurrentColor(255, 201, 14);
    rendertxt(0x09,starcol,false,false);  
  }
  

  setPixelRGB( ( offsetX +1 ) * LED_COL + offsetY + (starcol*8) + 1, 0, 0, 0);
  setPixelRGB( ( offsetX + 2 ) * LED_COL + offsetY + (starcol*8) + 1, 0, 0, 0);
  setPixelRGB( ( offsetX + 1 ) * LED_COL + offsetY + (starcol*8) + 2 , 0, 0, 0);
  setPixelRGB( ( offsetX + 2)  * LED_COL + offsetY + (starcol*8) + 2, 0, 0, 0);

   
    rendertxt(0x01,starcol,false,true);  

}

void displayEyes(int offsetX, int offsetY) 
{
  FastLED.clear();

  if ( eyeSk == 4 )
  {
    setCurrentColor(255, 201, 14);
    displayEye(eyexL,offsetX,offsetY,3);
    setCurrentColor(0, 255, 0);
    displayEye(eyexR,offsetX,offsetY,3);
  }
  else    
  {
    displayEye(eyexL,offsetX,offsetY,eyeSk);
    displayEye(eyexR,offsetX,offsetY,eyeSk);
  }

  currentX = offsetX;
  currentY =  offsetY;
  FastLED.show();
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

void louche()
{
  
  movePupil(2, 2, 40);
  delay(100);
  
for ( int x = 2 ; x > 0 ; x-- )
{
  FastLED.clear();
  if ( eyeSk == 5 )
  {
    // default skin current color used
    displayEye(eyexL,4-x,2,7);
    displayEye(eyexR,x,2,8);
  }
  else 
  {
    //  skin tired eye
    displayEye(eyexL,4-x,2,eyeSk);
    displayEye(eyexR,x,2,eyeSk);
  }
  FastLED.show();
  delay(100);
}

delay(600);


for ( int x = 0 ; x < 3 ; x++ )
{
  FastLED.clear();
  if ( eyeSk == 5 )
  {
    // default skin current color used
    displayEye(eyexL,4-x,2,7);
    displayEye(eyexR,x,2,8);
  }
  else 
  {
    //  skin tired eye
    displayEye(eyexL,4-x,2,eyeSk);
    displayEye(eyexR,x,2,eyeSk);
  }
  FastLED.show();
  delay(200);
}

  currentX = 2;
  currentY =  2;
  
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
