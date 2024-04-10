#include "song.h"
#include <LiquidCrystal_I2C.h>

//the screen is at address 0x27 and has size 16 chars and 2 lines
LiquidCrystal_I2C screen(0x27, 16, 2);

char* names[] = {"Silent Night", "We Wish Xmas", "Twirly      ", "Scale       ", "Spongebob   ", "ShutAndDance", "Melody      ", "Some Nights ", "Some Nights2"};
Song songs[9];
Song SilentNight = Song(46); //the 46 is the amount of notes that are in the song
Song WishXmas = Song(52);
Song Twirly = Song(16);
Song Scale = Song(8);
Song Spongebob = Song(10);
Song ShutAndDance = Song(34);
Song Melody = Song(10);
Song SomeNights = Song(1);
Song SomeNightss = Song(70);

//this is the pin we read the pot from
const int potPin = A1;

//this currLine is used as a global in the write Arrow function so that we know which line
//the arrow is currently on and can change it if necessary
int currLine = 3;
int bottomSong = 1;

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
int whichLine(int currSong);
void writeArrow(int line);
void write(int bottom = 1);

//these are functions used in the loop
void pulseMotors();
void checkButtonPress(int song);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//SETUP STUFF
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void setup() {
  //Serial.begin(9600);

  //get the screen set up
  screen.init();
  screen.backlight();
  screen.clear();
  int whichSong = selectSong();
  int whichL = whichLine(whichSong);
  writeArrow(whichL);

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

  Twirly.insertNote(g, half);
  Twirly.insertNote(fSh, eighth/2);
  Twirly.insertNote(g, eighth/2);
  Twirly.insertNote(fSh, eighth/2);
  Twirly.insertNote(g, eighth/2);
  Twirly.insertNote(fSh, eighth/2);
  Twirly.insertNote(e, eighth/2);
  Twirly.insertNote(fSh, eighth/2);
  Twirly.insertNote(e, quarter);
  Twirly.insertNote(fSh, eighth/3);
  Twirly.insertNote(g, eighth/3);
  Twirly.insertNote(a, eighth/3);
  Twirly.insertNote(b, eighth/3);
  Twirly.insertNote(cSh, eighth/3);
  Twirly.insertNote(hiD, eighth/3);
  Twirly.insertNote(e, whole);


  Scale.insertNote(d, whole*2);
  Scale.insertNote(e, whole*2);
  Scale.insertNote(fSh, whole*2);
  Scale.insertNote(g, whole*2);
  Scale.insertNote(a, whole*2);
  Scale.insertNote(b, whole*2);
  Scale.insertNote(cSh, whole*2);
  Scale.insertNote(hiD, whole*2);

  Spongebob.insertNote(d, quarter);
  Spongebob.insertNote(g, eighth-4);
  Spongebob.insertNote(a, eighth-4);
  Spongebob.insertNote(b, eighth-4);
  Spongebob.insertNote(a, eighth*1.5);
  Spongebob.insertNote(b, eighth-4);
  Spongebob.insertNote(g, eighth*1.5);
  Spongebob.insertNote(d, eighth);
  Spongebob.insertNote(fSh, eighth/3);
  Spongebob.insertNote(g, half);

for (int i = 0; i < 2; ++i) {
  ShutAndDance.insertNote(fSh, eighth);
  ShutAndDance.insertNote(a, eighth);
  ShutAndDance.insertNote(hiD, eighth);
  ShutAndDance.insertNote(g, eighth);
  ShutAndDance.insertNote(a, eighth);
  ShutAndDance.insertNote(hiD, eighth);
  ShutAndDance.insertNote(g, eighth);
  ShutAndDance.insertNote(a, eighth);
  ShutAndDance.insertNote(fSh, eighth);
  ShutAndDance.insertNote(a, eighth);
  ShutAndDance.insertNote(hiD, eighth);
  ShutAndDance.insertNote(e, eighth);
  ShutAndDance.insertNote(a, eighth);
  ShutAndDance.insertNote(hiD, eighth);
  ShutAndDance.insertNote(e, eighth);
  ShutAndDance.insertNote(a, eighth);
}

  Melody.insertNote(d, quarter);
  Melody.insertNote(cSh, eighth);
  Melody.insertNote(d, 6 * quarter);
  Melody.insertNote(e, quarter);
  Melody.insertNote(d, 5 * quarter);
  Melody.insertNote(a, quarter);
  Melody.insertNote(fSh, quarter);
  Melody.insertNote(e, quarter);
  Melody.insertNote(d, half);

  SomeNights.insertNote(d, half);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(fSh, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(a, quarter * 1.5);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, eighth);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(d, half + quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(e, whole);
  SomeNights.insertNote(e, whole);

  SomeNights.insertNote(d, half);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(fSh, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, eighth);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(d, half + quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(b, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(e, half);
  SomeNights.insertNote(e, quarter);
  SomeNights.insertNote(e, whole); // rest 58.5

  SomeNights.insertNote(fSh, eighth);
  SomeNights.insertNote(a, quarter * 1.5);
  SomeNights.insertNote(b, quarter + half);
  SomeNights.insertNote(a, eighth * 1.5);
  SomeNights.insertNote(g, sixteenth);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(fSh, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(b, half + quarter);
  SomeNights.insertNote(a, half);
  SomeNights.insertNote(d, quarter);
  SomeNights.insertNote(fSh, quarter);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(b, half + quarter);
  SomeNights.insertNote(a, eighth * 1.5);
  SomeNights.insertNote(g, sixteenth); //22

  SomeNights.insertNote(fSh, half + eighth);
  SomeNights.insertNote(g, quarter * 1.5);
  SomeNights.insertNote(fSh, quarter * 1.5);
  SomeNights.insertNote(e, half);
  SomeNights.insertNote(g, quarter + half);
  SomeNights.insertNote(fSh, quarter);
  SomeNights.insertNote(e, half);
  SomeNights.insertNote(b, half);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(d, half + quarter);
  SomeNights.insertNote(b, half);
  SomeNights.insertNote(a, quarter);
  SomeNights.insertNote(fSh, half);
  SomeNights.insertNote(d, half); //28.5




  SomeNightss.insertNote(fSh, half);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(a, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter * 1.5);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, sixteenth);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(a, half);
  SomeNightss.insertNote(fSh, half + quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(a, half);
  SomeNightss.insertNote(a, whole);
  SomeNightss.insertNote(a, whole);

  SomeNightss.insertNote(fSh, half);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(a, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(a, half);
  SomeNightss.insertNote(fSh, half + quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(hiD, quarter);
  SomeNightss.insertNote(a, half);
  SomeNightss.insertNote(a, quarter);
  SomeNightss.insertNote(a, whole); // rest 56.75
  
  SomeNightss.insertNote(a, eighth);
  SomeNightss.insertNote(hiD, quarter * 1.5);
  SomeNightss.insertNote(hiD, half + quarter);
  SomeNightss.insertNote(hiD, half + 2);
  SomeNightss.insertNote(b, sixteenth+2);
  SomeNightss.insertNote(a, half + quarter);
  SomeNightss.insertNote(hiD, whole + half);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(a, quarter);
  SomeNightss.insertNote(hiD, whole);
  SomeNightss.insertNote(hiD, eighth * 1.5);
  SomeNightss.insertNote(b, sixteenth+2); //22

  SomeNightss.insertNote(a, half + eighth);
  SomeNightss.insertNote(b, half + quarter);
  SomeNightss.insertNote(a, quarter * 1.5);
  SomeNightss.insertNote(cSh, quarter + half);
  SomeNightss.insertNote(b, quarter);
  SomeNightss.insertNote(a, half);

  SomeNightss.insertNote(g, half);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(d, whole + quarter);
  SomeNightss.insertNote(g, half);
  SomeNightss.insertNote(fSh, quarter);
  SomeNightss.insertNote(d, whole);


  songs[0] = SilentNight;
  songs[1] = WishXmas;
  songs[2] = Twirly;
  songs[3] = Scale;
  songs[4] = Spongebob;
  songs[5] = ShutAndDance;
  songs[6] = Melody;
  songs[7] = SomeNights;
  songs[8] = SomeNightss;

  write();
  
}


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//MAIN LOOP
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void loop() {
  start = millis();
  pulseMotors();
  int whichSong = selectSong();
  int whichL = whichLine(whichSong);
  writeArrow(whichL);
  //button press stuff
  checkButtonPress(whichSong);
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
void write(int bottom = 1) {
  //change this ho so that it reads the names of the songs from the songs array
  //also so that it can deal with a new bottom line song
  screen.setCursor(0, 0);
  screen.print(names[bottom - 1]);
  screen.setCursor(0, 1);
  screen.print(names[bottom]);
  //screen.setCursor(14, 1);
  //screen.print("<-");
}

//this function reads the pot and decides which song we want
int selectSong() {
  int num = analogRead(potPin);
  int threshold = 1020 / ((sizeof(names)/sizeof(names[0]))-1);  //so im pretty sure that analog pins can read somewhere between 0 and 1023, I will check that later
  //this code essentially will read the analog pin and then determine how many songs there are and the threshold to pass them
  //for example, if there is one song, then the entire range of pot values will return 0 because 501/1 is 500 and even the maximum
  //value of 500 will return 0 because 1023/1024 is 0
  //by this same logic, it will be able to return the correct song for any range of songs

  //for example with 3 songs, the threshold will be more or less 340, this will allow the func to return 0, 1, and 2
  return num / threshold;
}


int whichLine(int currSong) {
  //check if it is the bottom song already printed
  if (currSong == bottomSong) {
    return 1;
    
    //check if it is the top song already printed
  } else if (currSong == bottomSong - 1) {
    return 0;

    //check if it is below the bottom song
  } else if (currSong > bottomSong) {
    bottomSong = currSong;
    write(bottomSong);
    return 1;

    //check if it is above the top song
  } else {
    //decrement the bottom song (scroll everything up 1 row)
    bottomSong -= 1;
    write(bottomSong);
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
void checkButtonPress(int song) {
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
    songs[song].play();

    //after playing the song we must now reset everything
    //we get a new time
    time = millis();
    //we clear the screen
    screen.clear();
    //we rewrite the songs
    write(bottomSong);
    //now this part is kind of annoying, but the writeArrow thing will only
    //write the arrow if it decides that it is not already on the correct line
    //It unfortunately does not know that the arrow has been wiped even though the line
    //hasn't changed. This means that we need to set currLine to -1 so that it will for
    //sure draw the arrow.
    int correctLine = currLine;
    currLine = -1;
    //we pass "correctLine" to the writeArrow function which is the line that the arrow was
    //on before the pressButton function started
    writeArrow(correctLine);
    currLine = 1;
    finish = millis();
    //motor pulse timing
    while (finish - time < 20) {
      delayMicroseconds(1);
      finish = millis();
    }
  }
}
