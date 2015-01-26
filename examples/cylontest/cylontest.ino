#include <Adafruit_NeoPixel.h>

static uint32_t kOutputPIN = 2;
static uint32_t kNumberOfPixels = 150;

static uint32_t kWipeDelay = 2;

static uint32_t kSlowCylonTime = 2000;
static uint32_t kFastCylonTime = 1000;

static int32_t kCylonWidth = 40;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(kNumberOfPixels, kOutputPIN, NEO_GRB + NEO_KHZ800);

//-------------------------------------------------------------
//             MODES
//-------------------------------------------------------------
//assuming 32bit per color RGB
#define RED  0xFF0000
#define ORANGE 0xFF5500
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define AQUA 0x00FFFF
#define BLUE 0x0000FF
//#define INDIGO 0x3300FF
#define VIOLET 0xFF00FF
#define WHITE  0xFFFFFF
#define BLACK 0

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Yo yo yo!");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
#if 1
  Serial.println("  colorWipe(RED, kWipeDelay);");
  colorWipe(RED, kWipeDelay);
  Serial.println("  colorWipe(ORANGE, kWipeDelay);");
  colorWipe(ORANGE, kWipeDelay);
  Serial.println("  colorWipe(YELLOW, kWipeDelay);");
  colorWipe(YELLOW, kWipeDelay);
  Serial.println("  colorWipe(GREEN, kWipeDelay);");
  colorWipe(GREEN, kWipeDelay);
  Serial.println("  colorWipe(AQUA, kWipeDelay);");
  colorWipe(AQUA, kWipeDelay);
  Serial.println("  colorWipe(BLUE, kWipeDelay);");
  colorWipe(BLUE, kWipeDelay);
  Serial.println("  colorWipe(VIOLET, kWipeDelay);");
  colorWipe(VIOLET, kWipeDelay);
  Serial.println("  colorWipe(BLACK, kWipeDelay);");
  colorWipe(BLACK, kWipeDelay);
  Serial.println("  rainbow(kWipeDelay, 6);");
  rainbow(kWipeDelay, 6);
  Serial.println("  cylon(0, kFastCylonTime, 6, false);");
  cylon(0, kFastCylonTime, 6, false);
  Serial.println("  cylon(1, kFastCylonTime, 6, false);");
  cylon(1, kFastCylonTime, 6, false);
  Serial.println("  cylon(2, kFastCylonTime, 6, false);");
  cylon(2, kFastCylonTime, 6, false);
  Serial.println("  cylon(3, kSlowCylonTime, 6, false);");
  cylon(3, kSlowCylonTime, 6, false);
  Serial.println("  cylon(4, kSlowCylonTime, 6, false);");
  cylon(4, kSlowCylonTime, 6, false);
  Serial.println("  cylon(5, kSlowCylonTime, 6, false);");
  cylon(5, kSlowCylonTime, 6, false);
  Serial.println("  cylon(0, kFastCylonTime, 6, true);");
  cylon(0, kFastCylonTime, 6, true);
  Serial.println("  cylon(1, kFastCylonTime, 6, true);");
  cylon(1, kFastCylonTime, 6, true);
  Serial.println("  cylon(2, kFastCylonTime, 6, true);");
  cylon(2, kFastCylonTime, 6, true);
  Serial.println("  cylon(3, kSlowCylonTime, 6, true);");
  cylon(3, kSlowCylonTime, 6, true);
  Serial.println("  cylon(4, kSlowCylonTime, 6, true);");
  cylon(4, kSlowCylonTime, 6, true);
  Serial.println("  cylon(5, kSlowCylonTime, 6, true);");
  cylon(5, kSlowCylonTime, 6, true);
  Serial.println("  colorWipe(BLACK, kWipeDelay);");
  colorWipe(BLACK, kWipeDelay);
  Serial.println("  rainbowCycle(kWipeDelay, 6);");
  rainbowCycle(kWipeDelay, 6);
#else
  cylon(5, kSlowCylonTime, 6, false);
#endif
}

//#############################################################
// This code is working
//#############################################################

// cylon back and forth in a given mode at loop speed.  mode is what the cylon
// will overlay on top of set numberofpasses to 0 for infinite runs
void cylon(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses, boolean invert) {
  int32_t delaytime =  loopspeed / (strip.numPixels() * 2);
  uint32_t basecolor = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint8_t update = 0;
  uint16_t pass = 0;
  int16_t leadpos = 0;

  boolean affects_whole_string = mode > 2 && invert;

  // static color modes
  switch (mode) {
    case 0 :
      basecolor = RED;  //red
      break;
    case 1 :
      basecolor = GREEN;  //green
      break;
    case 2 :
      basecolor = BLUE;  //blue
      break;
    default:
      break;
  }

  if (invert) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, basecolor);
    }
    strip.show();
  }

  while ( !numberofpasses || pass <= (numberofpasses - 1) ) {
    uint32_t start_time = millis();

    dir ? leadpos++ : leadpos--;

    if (leadpos==0) {
      dir = 1;
      pass++;
    } else if ( leadpos==(strip.numPixels()-1) ) {
      dir = 0;
    }

    //counter for dynamic rainbows
    j = (j + 1) % 256;

    int16_t start_index = affects_whole_string ? 0 : max(0, leadpos - kCylonWidth);
    int16_t end_index = affects_whole_string ? strip.numPixels() : min(strip.numPixels(), leadpos + kCylonWidth + 1);
    for(int16_t i = start_index ; i < end_index ; ++i) {
      uint32_t c = basecolor;

      switch (mode) {
        //color modes that change accross strip
        case 3 :
          //static rainbow
          c =  Wheel(((i * 256 / strip.numPixels()) ) & 255);
          break;
        case 4 :
          //dynamic rainbow
          c = Wheel((i+j) & 255);
          break;
        case 5:
          c = Wheel(j);
          break;
        default:
          break;
      }

      int32_t distance = abs(leadpos - i);
      if (invert) {
        distance = max(0, kCylonWidth - distance);
      }
      if (distance >= kCylonWidth) {
        c = 0;
      } else if (distance) {
        int32_t percent = 100 * (kCylonWidth - distance) / kCylonWidth;
        c = fadeColor(c, percent);
      }

      strip.setPixelColor(i, c);
    } //end for loop
    strip.show();

    uint32_t loop_time = millis() - start_time;
    uint32_t delayleft = delaytime < loop_time ? 0 : delaytime - loop_time;
    if (delayleft) {
      delay(delayleft);
    }

  } //end of while loop
}

//###########################
//OTHER METHODS
//##########################

uint32_t fadeColor(uint32_t c, uint8_t percent) {
  percent = max(0, min(100, percent));
  uint8_t r,g,b;
  r = (uint8_t)(c >> 16);
  g = (uint8_t)(c >> 8);
  b = (uint8_t)c;

  r = (r * percent / 100);
  g = (g * percent / 100);
  b = (b * percent / 100);

  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

//dim the coor in factors of 2....basically right shift
// the r, g, b bytes.   Can only shift 0-7
uint32_t dimColor(uint32_t c, uint8_t dim) {
  uint8_t r,g,b;
  r = (uint8_t)(c >> 16);
  g = (uint8_t)(c >> 8);
  b = (uint8_t) c;
  if (dim > 7) {
    return c;    //do nothing
  }
  r = r >> dim;
  g = g >> dim;
  b = b >> dim;
  c = 0;
  c = ((uint32_t)r ) << 16;
  c = c + (uint32_t)(g << 8);
  c = c + b;

  return c;

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      if (wait) delay(wait);
  }
}

void setAll(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void rainbow(uint8_t wait, uint32_t number_of_cycles) {
  uint16_t i, j;

  for(j = 0; j < 256 * number_of_cycles; j++) {
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait, uint32_t number_of_cycles) {
  uint16_t i, j;

  for(j = 0; j < 256 * number_of_cycles; j++) { // 5 cycles of all colors on wheel
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
