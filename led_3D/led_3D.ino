/*
Based on ledcube.c from Make: September 7, 2007 weekend podcast  
http://blog.makezine.com/archive/2007/09/make_a_pocket_led_cube_we.html

Custom animation programmed by Mark Boszko, http://stationinthemetro.com
*/

#include <avr/pgmspace.h>        // allows use of PROGMEM to store patterns in flash

#define CUBESIZE 3
#define PLANESIZE CUBESIZE*CUBESIZE
#define PLANETIME 3333          // time each plane is displayed in us -> 100 Hz refresh
#define TIMECONST 3          // multiplies DisplayTime to get ms - why not =100?

// LED Pattern Table in PROGMEM - last column is display time in 100ms units
// TODO this could be a lot more compact but not with binary pattern representation
prog_uchar PROGMEM PatternTable[] = {

  
  B101, B010, B101, B000, B000, B000, B000, B000, B000, 50 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 50 , 
  B000, B000, B000, B000, B000, B000, B101, B010, B101, 50 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 50 , 
  B101, B000, B000, B010, B000, B000, B101, B000, B000, 50 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 50 , 
  B000, B000, B101, B000, B000, B010, B000, B000, B101, 50 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 50 ,    
  B101, B010, B101, B000, B000, B000, B000, B000, B000, 50 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 40 , 
  B000, B000, B000, B000, B000, B000, B101, B010, B101, 40 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 40 , 
  B101, B000, B000, B010, B000, B000, B101, B000, B000, 40 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 40 , 
  B000, B000, B101, B000, B000, B010, B000, B000, B101, 30 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 30 ,
  B101, B010, B101, B000, B000, B000, B000, B000, B000, 30 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 30 , 
  B000, B000, B000, B000, B000, B000, B101, B010, B101, 30 , 
  B000, B000, B000, B000, B010, B000, B000, B000, B000, 20 , 
  B011, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B101, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B110, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B100, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B110, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B101, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B011, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B001, B111, B000, B000, B000, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B000, B000, B000, 10 , 
  B101, B101, B111, B101, B101, B111, B000, B000, B000, 10 , 
  B110, B101, B111, B110, B101, B111, B000, B000, B000, 10 , 
  B111, B100, B111, B111, B100, B111, B000, B000, B000, 10 , 
  B111, B101, B110, B111, B101, B110, B000, B000, B000, 10 , 
  B111, B101, B101, B111, B101, B101, B000, B000, B000, 10 , 
  B111, B101, B011, B111, B101, B011, B000, B000, B000, 10 , 
  B111, B001, B111, B111, B001, B011, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B011, B101, B111, 10 , 
  B101, B101, B111, B101, B101, B111, B101, B101, B111, 10 , 
  B110, B101, B111, B110, B101, B111, B110, B101, B111, 10 , 
  B111, B100, B111, B111, B100, B111, B111, B100, B111, 10 , 
  B111, B101, B110, B111, B101, B110, B111, B101, B110, 10 , 
  B111, B101, B101, B111, B101, B101, B111, B101, B101, 10 , 
  B111, B101, B011, B111, B101, B011, B111, B101, B011, 10 , 
  B111, B001, B111, B111, B001, B111, B111, B001, B110, 10 ,
  B011, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B101, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B110, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B100, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B110, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B101, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B011, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B001, B111, B000, B000, B000, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B000, B000, B000, 10 , 
  B101, B101, B111, B101, B101, B111, B000, B000, B000, 10 , 
  B110, B101, B111, B110, B101, B111, B000, B000, B000, 10 , 
  B111, B100, B111, B111, B100, B111, B000, B000, B000, 10 , 
  B111, B101, B110, B111, B101, B110, B000, B000, B000, 10 , 
  B111, B101, B101, B111, B101, B101, B000, B000, B000, 10 , 
  B111, B101, B011, B111, B101, B011, B000, B000, B000, 10 , 
  B111, B001, B111, B111, B001, B011, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B011, B101, B111, 10 , 
  B101, B101, B111, B101, B101, B111, B101, B101, B111, 10 , 
  B110, B101, B111, B110, B101, B111, B110, B101, B111, 10 , 
  B111, B100, B111, B111, B100, B111, B111, B100, B111, 10 , 
  B111, B101, B110, B111, B101, B110, B111, B101, B110, 10 , 
  B111, B101, B101, B111, B101, B101, B111, B101, B101, 10 , 
  B111, B101, B011, B111, B101, B011, B111, B101, B011, 10 , 
  B111, B001, B111, B111, B001, B111, B111, B001, B110, 10 ,
  B011, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B101, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B110, B101, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B100, B111, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B110, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B101, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B101, B011, B000, B000, B000, B000, B000, B000, 10 , 
  B111, B001, B111, B000, B000, B000, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B000, B000, B000, 10 , 
  B101, B101, B111, B101, B101, B111, B000, B000, B000, 10 , 
  B110, B101, B111, B110, B101, B111, B000, B000, B000, 10 , 
  B111, B100, B111, B111, B100, B111, B000, B000, B000, 10 , 
  B111, B101, B110, B111, B101, B110, B000, B000, B000, 10 , 
  B111, B101, B101, B111, B101, B101, B000, B000, B000, 10 , 
  B111, B101, B011, B111, B101, B011, B000, B000, B000, 10 , 
  B111, B001, B111, B111, B001, B011, B000, B000, B000, 10 ,
  B011, B101, B111, B011, B101, B111, B011, B101, B111, 10 , 
  B101, B101, B111, B101, B101, B111, B101, B101, B111, 10 , 
  B110, B101, B111, B110, B101, B111, B110, B101, B111, 10 , 
  B111, B100, B111, B111, B100, B111, B111, B100, B111, 10 , 
  B111, B101, B110, B111, B101, B110, B111, B101, B110, 10 , 
  B111, B101, B101, B111, B101, B101, B111, B101, B101, 10 , 
  B111, B101, B011, B111, B101, B011, B111, B101, B011, 10 , 
  B111, B001, B111, B111, B001, B111, B111, B001, B110, 10 ,
   B000, B000, B000, B000, B000, B000, B100, B000, B000, 20 , 
  B000, B000, B000, B100, B000, B000, B000, B000, B000, 20 , 
  B100, B000, B000, B000, B000, B000, B000, B000, B000, 20 , 
  B010, B000, B000, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B010, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B010, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B001, B000, B000, 20 , 
  B000, B000, B000, B001, B000, B000, B000, B000, B000, 20 , 
  B001, B000, B000, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B001, B000, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B001, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B001, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B000, B001, 20 , 
  B000, B000, B000, B000, B000, B001, B000, B000, B000, 20 , 
  B000, B000, B001, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B010, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B010, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B000, B010, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B000, B100, 20 , 
  B000, B000, B000, B000, B000, B100, B000, B000, B000, 20 , 
  B000, B000, B100, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B100, B000, B000, B000, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B100, B000, B000, B000, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B100, B000, 20 , 
  B000, B000, B000, B000, B000, B000, B000, B000, B000, 20 , 
 
  };

/*
** Defining pins in array makes it easier to rearrange how cube is wired
** Adjust numbers here until LEDs flash in order - L to R, T to B
** Note that analog inputs 0-5 are also digital outputs 14-19!
** Pin DigitalOut0 (serial RX) and AnalogIn5 are left open for future apps
*/

//int LEDPin[] = {16, 3, 1, 15, 4, 6, 14, 5, 7};
int LEDPin[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10};

//int PlanePin[] = {19, 18, 17};
int PlanePin[] = {13, 12, 11};

// initialization
void setup()
{
  int pin;      // loop counter
  // set up LED pins as output (active HIGH)
  for (pin=0; pin<PLANESIZE; pin++) {
    pinMode( LEDPin[pin], OUTPUT );
  }
  // set up plane pins as outputs (active LOW) 
  for (pin=0; pin<CUBESIZE; pin++) {
    pinMode( PlanePin[pin], OUTPUT );
  }
}

// display pattern in table until DisplayTime is zero (then repeat)
void loop()
{
  // declare variables
  byte PatternBuf[ PLANESIZE ];      // saves current pattern from PatternTable
  int PatternIdx;
  byte DisplayTime;        // time*100ms to display pattern
  unsigned long EndTime;
  int plane;      // loop counter for cube refresh
  int patbufidx;   // indexes which byte from pattern buffer
  int ledrow;    // counts LEDs in refresh loop
  int ledcol;    // counts LEDs in refresh loop
  int ledpin;    // counts LEDs in refresh loop

  // Initialize PatternIdx to beginning of pattern table
  PatternIdx = 0;
  // loop over entries in pattern table - while DisplayTime>0
  do {
    // read pattern from PROGMEM and save in array
    memcpy_P( PatternBuf, PatternTable+PatternIdx, PLANESIZE );
    PatternIdx += PLANESIZE;
    // read DisplayTime from PROGMEM and increment index
    DisplayTime = pgm_read_byte_near( PatternTable + PatternIdx++ );    
    // compute EndTime from current time (ms) and DisplayTime
    EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST;

    // loop while DisplayTime>0 and current time < EndTime
    while ( millis() < EndTime ) {
      patbufidx = 0;    // reset index counter to beginning of buffer
      // loop over planes
      for (plane=0; plane<CUBESIZE; plane++) {
        // turn previous plane off
        if (plane==0) {
          digitalWrite( PlanePin[CUBESIZE-1], LOW );
        } else {
          digitalWrite( PlanePin[plane-1], LOW );
        }

        // load current plane pattern data into ports
        ledpin = 0;
        for (ledrow=0; ledrow<CUBESIZE; ledrow++) {
          for (ledcol=0; ledcol<CUBESIZE; ledcol++) {
            digitalWrite( LEDPin[ledpin++], PatternBuf[patbufidx] & (1 << ledcol) );
          }
          patbufidx++;
        } 

        // turn current plane on
        digitalWrite( PlanePin[plane], HIGH );
        // delay PLANETIME us
        delayMicroseconds( PLANETIME );
      }    // for plane
    }    // while <EndTime
  } while (DisplayTime > 0);        // read patterns until time=0 which signals end
}
