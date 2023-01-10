#ifndef song_h
#define song_h
#include "note.h"
//these are the durations of a for loop for given notes, assuming 4/4 time
const int eighth = 12;
const int quarter = 24;
const int half = 48;
const int whole = 96;
const int sixteenth = 6;

class Song {
private:
  void resize() {
    int* tempDurs = new int[capacity * 2];
    Note* tempNotes = new Note[capacity * 2];
    for (int i = 0; i < capacity; ++i) {
      tempNotes[i] = notes[i];
      tempDurs[i] = durations[i];
    }
    delete [] durations;
    delete [] notes;
    durations = tempDurs;
    notes = tempNotes;
    capacity *= 2;
  } 
  //the following functions have a param for duration which the static global variables
  //declared at the top of this .h file will be used for

  void rest(int dur) {
    unsigned long startTime = 0;
    unsigned long endTime = 0;
    for (int i = 0; i < dur; ++i) {
      startTime = millis();
      //this PORTD sets all pins to HIGH (except rx and tx pins 0 and 1 because we cant use those) 
      //whether the motor is supposed to uncover or cover the hole, it needs to have
      //a pulse of some length, so at the beginning of the loop, all pins must be set to high.
      PORTD = B11111100;
      delayMicroseconds(1300);
      PORTD = B00000000;
      endTime = millis();
      while (endTime - startTime < 20) {
        delayMicroseconds(1);
        endTime = millis();
      }
    }
  }


  void playNote(Note note, int dur) {
    unsigned long startTime = 0;
    unsigned long endTime = 0;
    for (int i = 0; i < dur; ++i) {
      startTime = millis();
      //this PORTD sets all pins to HIGH (except 0 and 1 which are rx and tx), whether the motor is supposed to uncover or cover the hole, it needs to have
      //a pulse of some length (minimum 1.3ms), so at the beginning of the loop, all pins must be set to high.
      PORTD = B11111100;
      delayMicroseconds(1300);
      //this note command will set certain pins to low (depending on which note) so that we close only the holes we want shut
      note();
      delayMicroseconds(200);
      //after 200 more microseconds we finish the pulse by setting PORTD to B00000000 which means OFF for all pins 0-7
      PORTD = B00000000;
      endTime = millis();
      //the motor needs a pulse every 20 ms so using the start time that we calculated at the beginning
      //we can check if 20ms has passed yet
      while (endTime - startTime < 20) {
        //if 20ms has not yet passed, we will delay by 1 microsecond up until the loop is exactly 20ms in length
        delayMicroseconds(1);
        endTime = millis();
      }
    }
  }

  //10 is just the default capacity I chose, but when creating a song most of the time we will pass
  //a note quantity to the constructor and will create an array with exactly that amount of space
  int capacity = 10;
  int usedSpace = 0;
  Note* notes = new Note[capacity];
  int* durations = new int[capacity];

public:
  Song() = default;
  Song(int numNotes) : capacity(numNotes), usedSpace(0) {}
  ~Song() {
    delete [] notes;
    delete [] durations;
  }

  Song& operator=(const Song& old) {
    delete [] this->durations;
    delete [] this->notes;
    this->durations = new int[old.size()];
    this->notes = new Note[old.size()];
    for (int i = 0; i < old.size(); ++i) {
      this->durations[i] = old.durations[i];
      this->notes[i] = old.notes[i];
    }
    this->capacity = old.capacity;
    this->usedSpace = old.usedSpace;
    return *this;
  }

  void insertNote(const Note& n, const int & d) {
    if (usedSpace == capacity) resize();
    notes[usedSpace] = n;
    durations[usedSpace] = d;
    usedSpace += 1;
  }

  void play() {
    for (int i = 0; i < usedSpace; ++i) {
      playNote(notes[i], durations[i]);
    }
  }

  int size() {
    return usedSpace;
  }
};
#endif
