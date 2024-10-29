#include <Arduino.h>

#define ENC_A 36
#define ENC_B 39
#define SW 26
 
volatile int counter = 0;
int lastCounter = 0;
bool has_changed = false;
long lastTimeCounterChanged = 2147483647;

int lastSWState = 0;
bool sw_pressed = false;
bool rawSW = false;

bool rotary_acceleration;


void reset_counter(){
    counter = 0;
    lastCounter = 0;
    lastTimeCounterChanged = millis();
}

void setupRotencoder(){
    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);

    pinMode(SW, INPUT_PULLUP);

    reset_counter();
}

void read_encoder(){
  static uint8_t old_AB = 3;  // Lookup table index
  static int8_t encval = 0;   // Encoder value  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table
 
  old_AB <<=2;  // Remember previous state  
 
  if (digitalRead(ENC_A)) old_AB |= 0x02; // Add current state of pin A
  if (digitalRead(ENC_B)) old_AB |= 0x01; // Add current state of pin B
   
  encval += enc_states[( old_AB & 0x0f )];
 
  // Update counter if encoder has rotated a full indent, that is at least 4 steps
  if( encval > 3 ) {        // Four steps forward
    counter--;              // Decrease counter
    encval = 0;
  }
  else if( encval < -3 ) {  // Four steps backwards
   counter++;               // Increase counter
   encval = 0;
  }

  if(lastCounter != counter){
    has_changed = true;

    if(rotary_acceleration){
        int revolutionTime = millis() - lastTimeCounterChanged;
        if(revolutionTime < 27){
            int rev = 27 - revolutionTime ;
            int result = 10^(int(rev/1));

            if (lastCounter > counter){
                counter -= result;
            }else{
                counter += result;
            }
        }
    }

    lastTimeCounterChanged = millis();
  }else{
    has_changed = false;
  }

  lastCounter = counter; 
}

void set_counter(int val){
    counter = val;
}

bool has_rotary_changed(){
    return has_changed;
}

void read_sw(){
    rawSW = !digitalRead(SW);
    
    if(rawSW == false && lastSWState == true){
        sw_pressed = true;
    }else{
        sw_pressed = false;
    }

    lastSWState = rawSW;
}

bool is_sw_pressed(){
    return sw_pressed;
}

int get_rotencoder_count(){
    return counter;
}

void set_rotencoder_acceleration(bool val){
    rotary_acceleration = val;
}