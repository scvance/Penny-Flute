#include "song.h"
#include <LiquidCrystal_I2C.h>

//the screen is at address 0x27 and has size 16 chars and 2 lines
LiquidCrystal_I2C screen(0x27, 16, 2);


char line1[12] = {'S', 'I', 'L', 'E', 'N', 'T', ' ', ' N', 'I', 'G', 'H', 'T'};
char line2[12] = {'W', 'E', ' ', 'W', 'I', 'S', 'H', ' ', 'X', 'M', 'A', 'S'};

Song songs[2];
Song SilentNight = Song(46); //the 46 is the amount of notes that are in the song
Song WishXmas = Song(52);

//this is the pin we read the pot from
const int potPin = A1;

//this currLine is used as a global in the write Arrow function so that we know which line
//the arrow is currently on and can change it if necessary
int currLine = 1;

//start we set to the millis() in order to time the loops
unsigned int start;
unsigned int end;


// I'm going to put the number of pin that the I/O command goes to
// PORTD = B11111100
//.         765432       <- those are the pins IE if B1 then pin 7 is hi, B11 means 7 and 6 hi
//This is a D scale
//Ie D E F# G A B C# D

//A servo motor is expecting pulses to start once per 20ms
//we can use pulses from 1ms to 2ms in width to control from 0-180degrees
//I'll probably use HIGH to be 1.5 ms so that the motor only has to move 90 degrees which i think is plenty

//motors connected to pins 3,5, and 6 are backwards and must be reversed. (this means they close with a shorter pulse as opposed to a long one)

const byte D = B01101000;
const byte E = B11101000;
const byte FSH = B10101000;
const byte G = B10001000;
const byte A = B10011000;
const byte B = B10010000;
const byte C = B11110100; 
const byte CSH = B10010100;
const byte HID = B01101100;


//when i say top i mean from the mouthpiece down
Note d = Note(D); //close all holes
Note e = Note(E); //close the top 5 holes
Note fSh = Note(FSH); //close the top 4 holes
Note g = Note(G); //close top 3 holes
Note a = Note(A); //close top 2 holes
Note b = Note(B); //close top hole
Note c = Note(C); //close the 2 holes beneath the top hole
Note cSh = Note(CSH); //all holes open
Note hiD = Note(HID); //close the bottom 5 holes (all holes except the one closest to your mouth)

// these are functions used to control the screen
void write();
int whichLine();
void writeArrow(int line);

//these are functions used in the loop
void pulseMotors();
void checkButtonPress();

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//SETUP STUFF
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void setup() {
  //get the screen set up
  screen.init();
  screen.backlight();
  screen.clear();
  write();

  //set the button input pin as an input
  pinMode(8, INPUT);

  //sets the digital pins from 2-7 to OUTPUT
  DDRD = DDRD |  B11111100;

  //make silent night
  for (int i = 0; i < 2; ++i) {
    SilentNight.insertNote(a, quarter * 1.5);
    SilentNight.insertNote(b, eighth);
    SilentNight.insertNote(a, quarter);
    SilentNight.insertNote(fSh, half * 1.5);
  }

  SilentNight.insertNote(e, half);
  SilentNight.insertNote(e, quarter);
  SilentNight.insertNote(cSh, half * 1.5);
  SilentNight.insertNote(hiD, half);
  SilentNight.insertNote(hiD, quarter);
  SilentNight.insertNote(a, half * 1.5);

  for (int i = 0; i < 2; ++i) {
    SilentNight.insertNote(b, half);
    SilentNight.insertNote(b, quarter);
    SilentNight.insertNote(hiD, quarter * 1.5);
    SilentNight.insertNote(cSh, eighth);
    SilentNight.insertNote(b, quarter);
    SilentNight.insertNote(a, quarter * 1.5);
    SilentNight.insertNote(b, eighth);
    SilentNight.insertNote(a, quarter);
    SilentNight.insertNote(fSh, half * 1.5);
  }

  SilentNight.insertNote(e, half);
  SilentNight.insertNote(e, quarter);
  SilentNight.insertNote(g, quarter * 1.5);
  SilentNight.insertNote(e, eighth);
  SilentNight.insertNote(cSh, quarter);
  SilentNight.insertNote(hiD, half * 1.5);

  //this one would have a rest but I haven't programmed that in yet
  SilentNight.insertNote(fSh, half + 24);
  SilentNight.insertNote(hiD, quarter);
  SilentNight.insertNote(a, quarter);
  SilentNight.insertNote(fSh, quarter);
  SilentNight.insertNote(a, quarter * 1.5);
  SilentNight.insertNote(g, eighth);
  SilentNight.insertNote(e, quarter);
  SilentNight.insertNote(d, whole + 10);

  //make we wish you a merry christmas
  WishXmas.insertNote(d, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(g, eighth);
  WishXmas.insertNote(a, eighth);
  WishXmas.insertNote(g, eighth);
  WishXmas.insertNote(fSh, eighth);
  WishXmas.insertNote(e, quarter);
  WishXmas.insertNote(e, quarter);

  WishXmas.insertNote(e, quarter);
  WishXmas.insertNote(a, quarter);
  WishXmas.insertNote(a, eighth);
  WishXmas.insertNote(b, eighth);
  WishXmas.insertNote(a, eighth);
  WishXmas.insertNote(g, eighth);
  WishXmas.insertNote(fSh, quarter);
  WishXmas.insertNote(d, quarter);

  WishXmas.insertNote(d, quarter);
  WishXmas.insertNote(b, quarter);
  WishXmas.insertNote(b, eighth);
  WishXmas.insertNote(c, eighth);
  WishXmas.insertNote(b, eighth);
  WishXmas.insertNote(a, eighth);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(e, quarter);

  WishXmas.insertNote(d, eighth);
  WishXmas.insertNote(d, eighth);
  WishXmas.insertNote(e, quarter);
  WishXmas.insertNote(a, quarter);
  WishXmas.insertNote(fSh, quarter);
  WishXmas.insertNote(g, half);

  WishXmas.insertNote(d, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(fSh, half);
  WishXmas.insertNote(fSh, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(fSh, quarter);
  WishXmas.insertNote(e, quarter);
  WishXmas.insertNote(d, half);

  WishXmas.insertNote(a, quarter);
  WishXmas.insertNote(b, quarter);
  WishXmas.insertNote(a, quarter);
  WishXmas.insertNote(g, quarter);
  WishXmas.insertNote(hiD, quarter);
  WishXmas.insertNote(d, quarter);
  WishXmas.insertNote(d, eighth);
  WishXmas.insertNote(d, eighth);
  WishXmas.insertNote(e, quarter);
  WishXmas.insertNote(a, quarter);
  WishXmas.insertNote(fSh, quarter);
  WishXmas.insertNote(g, whole);
  songs[0] = SilentNight;
  songs[1] = WishXmas;
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//MAIN LOOP
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void loop() {
  start = millis();
  pulseMotors();
  int whichSong = whichLine();
  writeArrow(whichSong);
  //button press stuff
  checkButtonPress();
  end = millis();
  while (end - start < 20) {
    delayMicroseconds(1);
    end = millis();
  }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//DEFINE THE FUNCTIONS
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

//this will pulse the motors while a song is not going
//it is to keep them under control
void pulseMotors() {
  PORTD = B11111100;
  delayMicroseconds(1300);
  PORTD = CSH;
  delayMicroseconds(200);
  PORTD = B00000000;
}

//this simply writes the song names out and sets the arrow
void write() {
  screen.setCursor(0, 0);
  screen.print("Silent Night");
  screen.setCursor(0, 1);
  screen.print("We Wish Xmas");
  screen.setCursor(14, 1);
  screen.print("<-");
}

//this function reads the pot and decides which line we should put the arrow on
int whichLine() {
  int num = analogRead(potPin);
  if (num < 250) {
    return 1;
  } else {
    return 0;
  }
}

//this function will write the arrow AND/OR change the arrow to a different line if needs be
void writeArrow(int line) {
  if (line != currLine) {
    screen.setCursor(14, currLine);
    screen.print("  ");
    screen.setCursor(14,line);
    screen.print("<-");
    currLine = line;
  }
}

//this essentially takes care of reading the button press pin as well as doing a countdown and then finally plays the song
void checkButtonPress() {
  if (digitalRead(8) == HIGH) {
    screen.clear();
    screen.print("Playing in 3");
    //this entire nested while loop is to delay 1 full second while not skimping on our
    //20 ms per pulse servos
    unsigned int time = millis();
    unsigned int finish = millis();
    while (finish - time < 1000) {
      unsigned int loopTime = millis();
      unsigned int endTime = millis();
      pulseMotors();
      while (endTime - loopTime < 20) {
        delayMicroseconds(1);
        endTime = millis();
      }
      finish = millis();
    }
    screen.setCursor(11, 0);
    screen.print("2");

    time = millis();
    finish = millis();
    while (finish - time < 1000) {
      unsigned int loopTime = millis();
      unsigned int endTime = millis();
      pulseMotors();
      while (endTime - loopTime < 20) {
        delayMicroseconds(1);
        endTime = millis();
      }
      finish = millis();
    }
    screen.setCursor(11, 0);
    screen.print("1");

    time = millis();
    finish = millis();
    while (finish - time < 1000) {
      unsigned int loopTime = millis();
      unsigned int endTime = millis();
      pulseMotors();
      while (endTime - loopTime < 20) {
        delayMicroseconds(1);
        endTime = millis();
      }
      finish = millis();
    }

    screen.clear();
    screen.print("Now Playing");
    songs[currLine].play();
    time = millis();
    screen.clear();
    write();
    currLine = 1;
    finish = millis();
    while (finish - time < 20) {
      delayMicroseconds(1);
      finish = millis();
    }
  }
}
