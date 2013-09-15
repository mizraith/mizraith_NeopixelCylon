/**
 CYLON for NeoPixel displays. This one specific to an Aruino Uno.
  Red Byer   8/30/2013
  www.redstoyland.com
  
*/

#include <Adafruit_NeoPixel.h>

//For the Pro Mini
//#define PIN 6

//For the Uno
#define PIN 2



#define NUMPIXELS 150


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

boolean HARDCODED = true;



/** ####################################################################
    MODE EXPLANATION AND DEFINTION
    #####################################################################

For the ARDUINO PRO MINI:
     8 bits of dip switch found on:  A0, A1, A2, A3, D10, D11, D12, D13
     HOWEVER, from "left to right" (1:8) the flow is
     D10, D11, D12, D13, A0, A1, A2, A3     where D13 is MSB

For the ARDUINO UNO:
     8 bits of dip switch found on D4, D5, D6, D7, D8, D9, D10, D11

Since we only read mode at startup (or every blue moon) we will not worry
about being efficient and will simply read the pins and shift them accoringly.

######################################################################## */

//FOR THE ARDUINO PRO MINI
//  <<  left shifts for reading in the dip switches
#define D10_shift  7       
#define D11_shift  6
#define D12_shift  5
#define D13_shift  4
#define A0_shift   3
#define A1_shift   2
#define A2_shift   1
#define A3_shift   0

//IPNPUT PINS FOR THE ARDUINO UNO
// 8 = MSB       1 = LSB
#define DIP_8    4
#define DIP_7    5
#define DIP_6    6
#define DIP_5    7
#define DIP_4    8
#define DIP_3    9
#define DIP_2    10
#define DIP_1    11


#define DIP_8_shift 7
#define DIP_7_shift 6
#define DIP_6_shift 5
#define DIP_5_shift 4
#define DIP_4_shift 3
#define DIP_3_shift 2
#define DIP_2_shift 1
#define DIP_1_shift 0


uint8_t CURRENT_MODE =  0x00;

//default mode is all yellow, per IPG standard
//and corresponds to all dipswitches being off.
#define MODE_DEFAULT 0    
#define MODE_RED     1
#define MODE_ORANGE  2
#define MODE_YELLOW  3
#define MODE_GREEN   4
#define MODE_AQUA    5
#define MODE_BLUE    6
#define MODE_VIOLET  7
#define MODE_WHITE   8
#define MODE_RAINBOW_NARROW   9
#define MODE_RAINBOW_WIDE    10

//Make top 2 bits (MSB, D10:D11) define cylon 
//Right shift to compare
#define cylon_shift 6

#define CYLON_OFF   0
#define CYLON_ON    1
#define CYLON_INV   2
#define DEMO        3

//cycle through all modes
#define MODE_DEMO   255




//#############################################
// COLORS
//assuming 32bit per color RGB
//#############################################
#define RED  0xFF0000
#define ORANGE 0xFF5500
#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define AQUA 0x00FFFF
#define BLUE 0x0000FF
//#define INDIGO 0x3300FF
#define VIOLET 0xFF00FF
#define WHITE  0xFFFFFF


unsigned long last_check_time = 0;

void setup() {
  Serial.begin(57600);
  
//For the ProMini 
//  pinMode(10, INPUT);
//  pinMode(11, INPUT);
//  pinMode(12, INPUT);
//  pinMode(13, INPUT);
//  pinMode(A0, INPUT);
//  pinMode(A1, INPUT);
//  pinMode(A2, INPUT);
//  pinMode(A3, INPUT);
//  //turn on pullups
//  digitalWrite(10, HIGH);
//  digitalWrite(11, HIGH);
//  digitalWrite(12, HIGH);
//  digitalWrite(13, HIGH);
//  digitalWrite(A0, HIGH);
//  digitalWrite(A1, HIGH);
//  digitalWrite(A2, HIGH);
//  digitalWrite(A3, HIGH);
  
//For the Uno  
  pinMode(DIP_8, INPUT);
  pinMode(DIP_7, INPUT);
  pinMode(DIP_6, INPUT);
  pinMode(DIP_5, INPUT);
  pinMode(DIP_4, INPUT);
  pinMode(DIP_3, INPUT);
  pinMode(DIP_2, INPUT);
  pinMode(DIP_1, INPUT);
  //turn on pullups
  digitalWrite(DIP_8, HIGH);
  digitalWrite(DIP_7, HIGH);
  digitalWrite(DIP_6, HIGH);
  digitalWrite(DIP_5, HIGH);
  digitalWrite(DIP_4, HIGH);
  digitalWrite(DIP_3, HIGH);
  digitalWrite(DIP_2, HIGH);
  digitalWrite(DIP_1, HIGH);
  
  //data pin for neopixels
  pinMode(PIN, OUTPUT);
  
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  setCurrentMode();
  
  last_check_time = millis();
  
  
  startupsequence();
}

/**
 * simple method for showing all pixels work
 */
void startupsequence() {
    colorWipe(RED, 5);
    colorWipe(GREEN, 5);
    colorWipe(BLUE, 5);
}

void setCurrentMode() {
  uint8_t k = 0;
  uint8_t temp = 0;
  
  k = ~digitalRead(DIP_8);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_8_shift);
  k = ~digitalRead(DIP_7);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_7_shift);
  k = ~digitalRead(DIP_6);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_6_shift);
  k = ~digitalRead(DIP_5);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_5_shift);
  k = ~digitalRead(DIP_4);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_4_shift);
  k = ~digitalRead(DIP_3);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_3_shift);
  k = ~digitalRead(DIP_2);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_2_shift);
  k = ~digitalRead(DIP_1);
  k = k & 0x01;    //only need last bit
  temp = temp | (k << DIP_1_shift);
 
  CURRENT_MODE = temp;
  if(HARDCODED) {
    CURRENT_MODE = 255;               //--------HARD CODED
  }
  Serial.print("CURRENT MODE: ");
  Serial.print(CURRENT_MODE, DEC);
  Serial.println();
}





/* #####################################################
       MAIN LOOP
   ##################################################### */


void loop() {
  unsigned long loop_time = millis();
  
  if( (loop_time - last_check_time) > 1000) {
    setCurrentMode();
    last_check_time = loop_time;
  }
  
  // Some example procedures showing how to display to the pixels:
  //  colorWipe(strip.Color(255, 0, 0), 20); // Red
  
        //check higher order modes first, then tunnel down
  uint8_t mode_upper = 0;
  uint8_t mode_low = 0;
  
  mode_upper = CURRENT_MODE >> cylon_shift;  //give us the top 2 bits
  mode_low = CURRENT_MODE & 0x0F;   //mask off upper nibble
  
  switch(mode_upper) {
      case CYLON_OFF:
            switch(mode_low) {
                case MODE_RAINBOW_NARROW:
                    rainbow(20);
                    break;
                case MODE_RAINBOW_WIDE:
                    rainbowCycle(20);
                    break;
                default:
                    solidMode(colorFromMode(mode_low));
                    break;
            }
            break;
      case CYLON_ON:
            cylon(mode_low, 2000, 4);  //put mode here
            break;
      case CYLON_INV:
            invertcylon(mode_low, 2000, 4);//put mode here
            break;
      case DEMO:
            demoMode();
            break;
  
  }
  
  
  
}



//###########################
//OTHER METHODS
//##########################



/**
 * Given a MODE number, return the valid color. 
 * NOTE, this only works for solid color modes (<=8), NOT for
 * the rainbow modes.   If MODE >8, will return YELLOW
 * as default.
 */
uint32_t colorFromMode(uint8_t mode) {
    switch (mode) {
          case MODE_DEFAULT:
              return YELLOW;
              break;
          case MODE_RED:
              return RED;
              break;
          case MODE_ORANGE:
              return ORANGE;
              break;
          case MODE_YELLOW:
              return YELLOW;
              break;
          case MODE_GREEN:
              return GREEN;
              break;
          case MODE_AQUA:
              return AQUA;
              break;
          case MODE_BLUE:
              return BLUE;
              break;
          case MODE_VIOLET:
              return VIOLET;
              break;
          case MODE_WHITE:
              return WHITE;
              break;
          default:
              return YELLOW;
              break;
      }
}






void solidMode(uint32_t color) {
  for(uint16_t i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, color);   
  }
  strip.show();
}


void demoMode() {
  colorWipe(RED, 10); // Red
  colorWipe(ORANGE, 10); // Orange
  colorWipe(YELLOW, 10 ); // Yellow
  colorWipe(GREEN, 10); // Green
  colorWipe(AQUA, 10); // Aqua
  colorWipe(BLUE, 10); // Blue
  colorWipe(VIOLET, 10); //Indigo
  colorWipe(WHITE, 10); // Purple
  cylon(MODE_RED, 4000, 5);
  cylon(MODE_GREEN, 4000, 5);
  cylon(MODE_BLUE, 4000, 5);
  cylon(MODE_RAINBOW_NARROW, 4000, 5);
  cylon(MODE_RAINBOW_WIDE, 4000, 5);
  invertcylon(MODE_RED, 4000, 5);
  invertcylon(MODE_GREEN, 4000, 5);
  invertcylon(MODE_BLUE, 4000, 5);
  invertcylon(MODE_RAINBOW_NARROW, 4000, 5);
  invertcylon(MODE_RAINBOW_WIDE, 4000, 5);
}



//dim the color in factors of 2....basically right shift
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
      delay(wait);
  }
}





//###########################
//DISPLAY METHODS
//##########################


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// rainbow helper method
//
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
     return strip.Color(255 - WheelPos * 3, WheelPos * 3, 0);     //was 2nd
  } else if(WheelPos < 170) {
   WheelPos -= 85;
     return strip.Color(0 , 255 - WheelPos * 3, WheelPos * 3);    //was 3rd
  } else {
     WheelPos -= 170;
     return strip.Color( WheelPos *3, 0, 255 - WheelPos * 3);     //was 1st
  }
}




//##################################################
// THIS VERISON SHORTCUTS CALCULATING EVERY PIXEL
//
//###################################################
//cylon back and forth in a given mode at loop speed.
// mode is what the cylon will overlay on top of
// set numberofpasses to 0 for infinite runs
//
// @params
//     uint8_t mode   the MODE NUMBER (bottom nibble only)
//     uint16_t loopspeed  (ms) full round trip time for cylon
//     uint16_t number of passes before exiting mode 
//
//  NOTE: in rainbowwide mode, the number of passes gets reset
// to 256 to allow for even transitions in non-demo mode
//
//
void cylon(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime = loopspeed / (strip.numPixels() * 2 );
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  //static color modes
  if(mode <= 8) {
    c = colorFromMode(mode);
  }
  
   //wide rainbow mode is special and should go through
  //multiples of 4 passes to prevent an uneven "jump"
  if ( (mode == MODE_RAINBOW_WIDE) &&
       (MODE_DEMO != CURRENT_MODE  ) ) {
    numberofpasses = 256;
  }
  
  
  while ( pass <= (numberofpasses - 1) ) {

      if(dir) {  leadpos++;  } 
      else    {  leadpos--;  } 
      
      if (leadpos==0) {  
          dir = 1; 
          pass++;  
          if(numberofpasses == 0) {
              pass = 0;
          }  
      } else if ( leadpos==(strip.numPixels()-1) ) {  
          dir = 0; 
      } 
      
      //counter for dynamic rainbows
      if(mode > MODE_WHITE) {
        j++;
        if (j==256) {
          j=0;
        }
      }        
      
      //color modes that change accross strip
      //and need to be recalculated in for loop
      switch (mode) {
        case MODE_RAINBOW_NARROW :
            c =  Wheel(((leadpos * 256 / strip.numPixels()) ) & 255);
            break;
        case MODE_RAINBOW_WIDE :
            c = Wheel((leadpos + j) & 255);
            break;
        default:
            break;
      }
      
      strip.setPixelColor(leadpos - 5,   0);  
      strip.setPixelColor(leadpos - 4,   dimColor(c, 4));
      strip.setPixelColor(leadpos - 3,   dimColor(c, 3));
      strip.setPixelColor(leadpos - 2,   dimColor(c, 2));     
      strip.setPixelColor(leadpos - 1,   dimColor(c, 1));      
      strip.setPixelColor(leadpos, c);
      strip.setPixelColor(leadpos + 1,   dimColor(c, 1));
      strip.setPixelColor(leadpos + 2,   dimColor(c, 2));
      strip.setPixelColor(leadpos + 3,   dimColor(c, 3));     
      strip.setPixelColor(leadpos + 4,   dimColor(c, 4));      
      strip.setPixelColor(leadpos + 5, 0 );   
           
      
      strip.show();
      delay(delaytime);   //not necessary since processor is running full out

   } //end of while loop
}




//##################################################
//  INVERTED CYLON
//###################################################
//cylon back and forth in a given mode at loop speed.
// mode is what the cylon will overlay on top of
// set numberofpasses to 0 for infinite runs
// @params
//     uint8_t mode   the MODE NUMBER (bottom nibble only)
//     uint16_t loopspeed  (ms) full round trip time for cylon
//     uint16_t number of passes before exiting mode
//
//  NOTE: in rainbowwide mode, the number of passes gets reset
// to 256 to allow for even transitions in non-demo mode
//
void invertcylon(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  
  //static color modes
  if(mode <= 8) {
    c = colorFromMode(mode);
  }
  
   //wide rainbow mode is special and should go through
  //multiples of 4 passes to prevent an uneven "jump"
  if ( (mode == MODE_RAINBOW_WIDE) &&
       (MODE_DEMO != CURRENT_MODE )  ) {
    numberofpasses = 256;
  }
  
  
  while ( pass <= (numberofpasses - 1) ) {
       //debug code follows
//    if(pass <= 1) {
//        delaytime =  8000 / (strip.numPixels() * 2);
//      }  
//     else if(pass >= 254) {
//        delaytime =  8000 / (strip.numPixels() * 2);
//      }
//      else {
//        delaytime = loopspeed / (strip.numPixels() * 2);
//      }

      if(dir) {  leadpos++;  } 
      else    {  leadpos--;  } 
      
      if (leadpos==0) {  
          dir = 1; 
          pass++;  
          if(numberofpasses == 0) {
              pass = 0;
          }  
      } else if ( leadpos==(strip.numPixels()-1) ) {  
          dir = 0; 
      } 
      
      //counter for dynamic rainbows
      if(mode > MODE_WHITE) {
        j++;
        if (j==256 ) {
          j=0;
        }
      }        
           
      for(uint16_t i=0; i< strip.numPixels(); i++) {
          //color modes that change accross strip
          //and need to be recalculated in for loop
          switch (mode) {
            case MODE_RAINBOW_NARROW :
                c =  Wheel(((i * 256 / strip.numPixels()) ) & 255);
                break;
            case MODE_RAINBOW_WIDE :
                //DEVELOPER NOTE:  This mode presents a continuity problem
                //The 'cylon' bounces and round trip only takes 63 steps
                //instead of 64.  After 4 round trips we fall short by a 
                //few steps (252 instead of 256), and this little jump
                //is visible.   
                c = Wheel((i+j) & 255);
                break;
            default:
                break;
          }
        
    
         if ( (i==leadpos) ) {
              //full OFF --------inverted
              strip.setPixelColor(i, 0);                
          }
          else {
              //full ON ----------inverted cylon
              strip.setPixelColor(i, c);
          }
         
          //now check 'trailers'
//          if(dir) {
              if (i == leadpos - 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 6));
              } else if ( i == leadpos - 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 4));                  
              } else if ( i == leadpos - 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 2));  
              } else if ( i == leadpos - 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 1));  
              }              
//          } else {
              if (i == leadpos + 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 6));
              } else if ( i == leadpos + 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 4));                  
              } else if ( i == leadpos + 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 2));           
              } else if ( i == leadpos + 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 1));  
              }                
//          }    
      } //end for loop for calculating strip
//      delay(delaytime);   //not necessary since processor is running full out
      delay(delaytime);
      strip.show();
   } //end of while loop
}









////##################################################
////  WORKING FULL SYMMETRICAL CYLON
////###################################################
////cylon back and forth in a given mode at loop speed.
//// mode is what the cylon will overlay on top of
//// set numberofpasses to 0 for infinite runs
//void cylonFULL(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
//  
//  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
//  uint32_t c = 0;        //color
//  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
//  uint16_t j = 0;   // arbitrary counters
//  uint16_t pass = 0;
//  uint16_t leadpos = 0;
//  
//  
//  // static color modes
//  switch (mode) {
//    case 0 :
//      c = strip.Color(255, 0, 0);  //red
//      break;
//    case 1 :
//      c = strip.Color(0, 255, 0);  //green
//      break;
//    case 2 :
//      c = strip.Color(0, 0, 255);  //blue
//      break;
//    default:
//      break;
//  }
//  
//  
//  
//  while ( pass <= (numberofpasses - 1) ) {
//      if(dir) {  leadpos++;  } 
//      else    {  leadpos--;  } 
//      
//      if (leadpos==0) {  
//          dir = 1; 
//          pass++;  
//          if(numberofpasses == 0) {
//              pass = 0;
//          }  
//      } else if ( leadpos==(strip.numPixels()-1) ) {  
//          dir = 0; 
//      } 
//      
//      //counter for dynamic rainbows
//      if(mode == 4) {
//        j++;
//        if (j==256) {
//          j=0;
//        }
//      }        
//           
//      for(uint16_t i=0; i< strip.numPixels(); i++) {
//          switch (mode) {
//            //color modes that change accross strip
//            case 3 :
//              //static rainbow
//              c =  Wheel(((i * 256 / strip.numPixels()) ) & 255);
//              break;
//            case 4 :
//              //dynamic rainbow
//              c = Wheel((i+j) & 255);
//              break;
//            default:
//              break;
//          }
//        
//    
//         if ( (i==leadpos) ) {
//              //full bright
//              strip.setPixelColor(i, c);                
//          }
//          else {
//              //off
//              strip.setPixelColor(i, 0);
//          }
//         
//          //now check 'trailers'
////          if(dir) {
//              if (i == leadpos - 1 ) {
//                //75% brightness
//                strip.setPixelColor(i, dimColor(c, 1));
//              } else if ( i == leadpos - 2 ) {
//                //50% brightness
//                strip.setPixelColor(i, dimColor(c, 2));                  
//              } else if ( i == leadpos - 3 ) {
//                //25% brightness
//                strip.setPixelColor(i, dimColor(c, 3));  
//              } else if ( i == leadpos - 4 ) {
//                //10% brightness
//                strip.setPixelColor(i, dimColor(c, 4));  
//              }              
////          } else {
//              if (i == leadpos + 1 ) {
//                //75% brightness
//                strip.setPixelColor(i, dimColor(c, 1));
//              } else if ( i == leadpos + 2 ) {
//                //50% brightness
//                strip.setPixelColor(i, dimColor(c, 2));                  
//              } else if ( i == leadpos + 3 ) {
//                //25% brightness
//                strip.setPixelColor(i, dimColor(c, 3));           
//              } else if ( i == leadpos + 4 ) {
//                //10% brightness
//                strip.setPixelColor(i, dimColor(c, 4));  
//              }                
////          }    
//      } //end for loop for calculating strip
////      delay(delaytime);   //not necessary since processor is running full out
//      delay(delaytime);
//      strip.show();
//   } //end of while loop
//}











//
//
//
////#############################################################
////This code is working
////#############################################################
////cylon back and forth in a given mode at loop speed.
//// mode is what the cylon will overlay on top of
//// set numberofpasses to 0 for infinite runs
//void cylonHalf(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
//  
//  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
//  uint32_t c = 0;        //color
//  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
//  uint16_t j = 0;   // arbitrary counters
//  uint8_t update = 0;   
//  uint16_t pass = 0;
//  uint16_t leadpos = 0;
//  
//  
//  // static color modes
//  switch (mode) {
//    case 0 :
//      c = RED;  //red
//      break;
//    case 1 :
//      c = GREEN;  //green
//      break;
//    case 2 :
//      c = BLUE;  //blue
//      break;
//    default:
//      break;
//  }
//  
//  
//  
//  while ( pass <= (numberofpasses - 1) ) {
//      if(dir) {  leadpos++;  } 
//      else    {  leadpos--;  } 
//      
//      if (leadpos==0) {  
//          dir = 1; 
//          pass++;  
//          if(numberofpasses == 0) {
//              pass = 0;
//          }  
//      } else if ( leadpos==(strip.numPixels()-1) ) {  
//          dir = 0; 
//      } 
//      
//      //counter for dynamic rainbows
//      if(mode == 4) {
//        j++;
//        if (j==256) {
//          j=0;
//        }
//      }        
//           
//      for(uint16_t i=0; i< strip.numPixels(); i++) {
//          switch (mode) {
//            //color modes that change accross strip
//            case 3 :
//              //static rainbow
//              c =  Wheel(((i * 256 / strip.numPixels()) ) & 255);
//              break;
//            case 4 :
//              //dynamic rainbow
//              c = Wheel((i+j) & 255);
//              break;
//            default:
//              break;
//          }
//        
//    
//         if ( (i==leadpos) ) {
//              //full bright
//              strip.setPixelColor(i, c);                
//          }
//          else {
//              //off
//              strip.setPixelColor(i, 0);
//          }
//         
//          //now check 'trailers'
//          if(dir) {
//              if (i == leadpos - 1 ) {
//                //75% brightness
//                strip.setPixelColor(i, dimColor(c, 1));
//              } else if ( i == leadpos - 2 ) {
//                //50% brightness
//                strip.setPixelColor(i, dimColor(c, 2));                  
//              } else if ( i == leadpos - 3 ) {
//                //25% brightness
//                strip.setPixelColor(i, dimColor(c, 3));  
//              } else if ( i == leadpos - 4 ) {
//                //10% brightness
//                strip.setPixelColor(i, dimColor(c, 4));  
//              }              
//          } else {
//              if (i == leadpos + 1 ) {
//                //75% brightness
//                strip.setPixelColor(i, dimColor(c, 1));
//              } else if ( i == leadpos + 2 ) {
//                //50% brightness
//                strip.setPixelColor(i, dimColor(c, 2));                  
//              } else if ( i == leadpos + 3 ) {
//                //25% brightness
//                strip.setPixelColor(i, dimColor(c, 3));           
//              } else if ( i == leadpos + 4 ) {
//                //10% brightness
//                strip.setPixelColor(i, dimColor(c, 4));  
//              }                
//          }    
//
//        } //end for loop
//        strip.show();
//        delay(delaytime);   //not necessary since processor is running full out
//      
//   } //end of while loop
//}
//



