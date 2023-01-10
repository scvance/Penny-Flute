#ifndef note_h
#define note_h

class Note {
private:
  byte portControl;
public:
  Note(byte port = B00000000) : portControl(port) {}

  //this turns it into a functor so that we can just call the note as a function
  void operator ()(void) {
    PORTD = portControl;
  }

  //assignment operator
  Note& operator=(const Note& n) {
    this->portControl = n.portControl;
    return *this;
  }
};


#endif
