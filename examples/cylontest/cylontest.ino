#include <Adafruit_NeoPixel.h>

#define PIN 6

#define NUMPIXELS 60

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



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






void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 20); // Red
  colorWipe(RED, 20); // Red
  colorWipe(ORANGE, 20); // Orange
  colorWipe(YELLOW, 20); // Yellow
  colorWipe(GREEN, 20); // Green
  colorWipe(AQUA, 20); // Aqua
  colorWipe(BLUE, 20); // Blue
  colorWipe(VIOLET, 20); //Indigo
  colorWipe(WHITE, 20); // Purple
  cylon(0, 1000, 6);
  cylon(1, 1000, 6);
  cylon(2, 1000, 6);
  cylon(3, 2000, 6);
  cylon(4, 2000, 6);
  invertcylon(0, 1000, 6);
  invertcylon(1, 1000, 6);
  invertcylon(2, 1000, 6);
  invertcylon(3, 2000, 6);
  invertcylon(4, 2000, 6);
//  rainbow(20);
//  rainbowCycle(20);
}


//##################################################
//  WORKING FULL SYMMETRICAL CYLON
//###################################################
//cylon back and forth in a given mode at loop speed.
// mode is what the cylon will overlay on top of
// set numberofpasses to 0 for infinite runs
void cylonFULL(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  
  // static color modes
  switch (mode) {
    case 0 :
      c = strip.Color(255, 0, 0);  //red
      break;
    case 1 :
      c = strip.Color(0, 255, 0);  //green
      break;
    case 2 :
      c = strip.Color(0, 0, 255);  //blue
      break;
    default:
      break;
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
      if(mode == 4) {
        j++;
        if (j==256) {
          j=0;
        }
      }        
           
      for(uint16_t i=0; i< strip.numPixels(); i++) {
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
            default:
              break;
          }
        
    
         if ( (i==leadpos) ) {
              //full bright
              strip.setPixelColor(i, c);                
          }
          else {
              //off
              strip.setPixelColor(i, 0);
          }
         
          //now check 'trailers'
//          if(dir) {
              if (i == leadpos - 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 1));
              } else if ( i == leadpos - 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 2));                  
              } else if ( i == leadpos - 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 3));  
              } else if ( i == leadpos - 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 4));  
              }              
//          } else {
              if (i == leadpos + 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 1));
              } else if ( i == leadpos + 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 2));                  
              } else if ( i == leadpos + 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 3));           
              } else if ( i == leadpos + 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 4));  
              }                
//          }    
      } //end for loop for calculating strip
//      delay(delaytime);   //not necessary since processor is running full out
      delay(delaytime);
      strip.show();
   } //end of while loop
}



//##################################################
// THIS VERISON SHORTCUTS CALCULATING EVERY PIXEL
//
//###################################################
//cylon back and forth in a given mode at loop speed.
// mode is what the cylon will overlay on top of
// set numberofpasses to 0 for infinite runs
void cylon(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime = loopspeed / (strip.numPixels() * 2 );
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  
  // static color modes
  switch (mode) {
    case 0 :
      c = strip.Color(255, 0, 0);  //red
      break;
    case 1 :
      c = strip.Color(0, 255, 0);  //green
      break;
    case 2 :
      c = strip.Color(0, 0, 255);  //blue
      break;
    default:
      break;
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
      if(mode == 4) {
        j++;
        if (j==256) {
          j=0;
        }
      }        
      
     switch (mode) {
        //color modes that change accross strip
        case 3 :
          //static rainbow
          c =  Wheel(((leadpos * 256 / strip.numPixels()) ) & 255);
          break;
        case 4 :
          //dynamic rainbow
          c = Wheel((leadpos + j) & 255);
          break;
        case 5:
          //need to vary based on "i" which no longer exists
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
void invertcylon(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  
  // static color modes
  switch (mode) {
    case 0 :
      c = strip.Color(255, 0, 0);  //red
      break;
    case 1 :
      c = strip.Color(0, 255, 0);  //green
      break;
    case 2 :
      c = strip.Color(0, 0, 255);  //blue
      break;
    default:
      break;
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
      if(mode == 4) {
        j++;
        if (j==256) {
          j=0;
        }
      }        
           
      for(uint16_t i=0; i< strip.numPixels(); i++) {
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
                strip.setPixelColor(i, dimColor(c, 4));
              } else if ( i == leadpos - 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 3));                  
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
                strip.setPixelColor(i, dimColor(c, 4));
              } else if ( i == leadpos + 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 3));                  
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





















//#############################################################
//This code is working
//#############################################################
//cylon back and forth in a given mode at loop speed.
// mode is what the cylon will overlay on top of
// set numberofpasses to 0 for infinite runs
void cylonHalf(uint8_t mode, uint16_t loopspeed, uint16_t numberofpasses) {
  
  uint32_t delaytime =  loopspeed / (strip.numPixels() * 2);
  uint32_t c = 0;        //color
  uint8_t dir = 1;   //1 = RIGHT (+)--->   0 = LEFT <----(-)   arbitrary
  uint16_t j = 0;   // arbitrary counters
  uint8_t update = 0;   
  uint16_t pass = 0;
  uint16_t leadpos = 0;
  
  
  // static color modes
  switch (mode) {
    case 0 :
      c = RED;  //red
      break;
    case 1 :
      c = GREEN;  //green
      break;
    case 2 :
      c = BLUE;  //blue
      break;
    default:
      break;
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
      if(mode == 4) {
        j++;
        if (j==256) {
          j=0;
        }
      }        
           
      for(uint16_t i=0; i< strip.numPixels(); i++) {
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
            default:
              break;
          }
        
    
         if ( (i==leadpos) ) {
              //full bright
              strip.setPixelColor(i, c);                
          }
          else {
              //off
              strip.setPixelColor(i, 0);
          }
         
          //now check 'trailers'
          if(dir) {
              if (i == leadpos - 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 1));
              } else if ( i == leadpos - 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 2));                  
              } else if ( i == leadpos - 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 3));  
              } else if ( i == leadpos - 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 4));  
              }              
          } else {
              if (i == leadpos + 1 ) {
                //75% brightness
                strip.setPixelColor(i, dimColor(c, 1));
              } else if ( i == leadpos + 2 ) {
                //50% brightness
                strip.setPixelColor(i, dimColor(c, 2));                  
              } else if ( i == leadpos + 3 ) {
                //25% brightness
                strip.setPixelColor(i, dimColor(c, 3));           
              } else if ( i == leadpos + 4 ) {
                //10% brightness
                strip.setPixelColor(i, dimColor(c, 4));  
              }                
          }    

        } //end for loop
        strip.show();
        delay(delaytime);   //not necessary since processor is running full out
      
   } //end of while loop
}




//###########################
//OTHER METHODS
//##########################



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
      delay(wait);
  }
}

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

