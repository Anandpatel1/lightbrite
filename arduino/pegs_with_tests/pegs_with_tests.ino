#include<PololuLedStrip.h>
#include "MsTimer2.h"
//#include "TimerOne.h"

#define MY_ADDRESS 10

#define ADDRESS_ALL 255

#define debug 0
#define BAUD 9600

#define TX1 1
#define RX0 0
//#define RST 29
#define D2 2
#define D3 3 
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 A6
#define A7 A7


#define PIN_NUMBER_LED D3

#define DELAY_COUNT 5
/*
#define NUM_PEGS 7
// List of Sensor 1 pins of all pegs
int sensor_pin1[NUM_PEGS] = {D2, D5, D8, D11, A0, A3, A6};
// List of Sensor 2 pins of all pegs
int sensor_pin2[NUM_PEGS] = {D3, D6, D9, D12, A1, A4, A7};
// List of Micro switch pins of all pegs
int on_off_pin[NUM_PEGS] = {RX0, D4, D7, D10, D13, A2, A5};
*/

/*#define NUM_PEGS 1
// List of Sensor 1 pins of all pegs
int sensor_pin1[NUM_PEGS] = {D6};
// List of Sensor 2 pins of all pegs
int sensor_pin2[NUM_PEGS] = {D5};
// List of Micro switch pins of all pegs
int on_off_pin[NUM_PEGS] = {D4};
*/
//#define NUM_PEGS 2
//// List of Sensor 1 pins of all pegs
//int sensor_pin1[NUM_PEGS] = {D6, A5};
//// List of Sensor 2 pins of all pegs
//int sensor_pin2[NUM_PEGS] = {D5, A6};
//// List of Micro switch pins of all pegs
//int on_off_pin[NUM_PEGS] = {D4, A7};


#define NUM_PEGS 6
// List of Sensor 1 pins of all pegs
//int on_off_pin[NUM_PEGS] = {D7, D10, D13, A2, A5, D6};
int sensor_pin1[NUM_PEGS] = {D6, A5, A2, D13, D10, D7};
// List of Sensor 2 pins of all pegs
//int sensor_pin2[NUM_PEGS] = {D9, D12, A1, A4, A7, D4};
int on_off_pin[NUM_PEGS] = {D4, A7, A4, A1, D12, D9};
// List of Micro switch pins of all pegs
//int sensor_pin1[NUM_PEGS] = {D8, D11, A0, A3, A6, D5};
int sensor_pin2[NUM_PEGS] = {D5, A6, A3, A0, D11, D8};

int sensor_input1[NUM_PEGS] = {0};
int prev_sensor_input1[NUM_PEGS] = {0};
int sensor_input2[NUM_PEGS] = {0};
int prev_sensor_input2[NUM_PEGS] = {0};
int on_off_input[NUM_PEGS] = {0};
int prev_on_off_input[NUM_PEGS] = {0};
int delay_count[NUM_PEGS] = {0};

PololuLedStrip<PIN_NUMBER_LED> ledStrip;
rgb_color colors[NUM_PEGS];
rgb_color temp_colors[NUM_PEGS];
int offset[NUM_PEGS] = {0};

rgb_color white, red, green, blue, black, magenta, cyan, yellow;

#define COLORS_COUNT 7
rgb_color* roll_colors[COLORS_COUNT] = {&cyan, &yellow, &magenta, &green, &blue, &red, &white};
//rgb_color* roll_colors[COLORS_COUNT] = {&white, &red, &green, &blue, &magenta, &cyan, &yellow};
char* color_names[COLORS_COUNT] = {"white", "red", "green", "blue", "magenta", "cyan", "yellow"};

char* current_color;

int prevSensorInput1 = 0;
int prevSensorInput2 = 0;
int prevOnOffSwitch = 0;

volatile char runMode = 0;
volatile char prev_runMode = 0;

/*************
 * COMMUNICATION
 */

#define MAX_TYPE_OF_MESSAGES 5

char LENGTH_OF_THIS_MSG[MAX_TYPE_OF_MESSAGES] = {5, 5, 23, 5, 5};
char TX_LENGTH_OF_THIS_MSG[MAX_TYPE_OF_MESSAGES] = {5, 5, 5, 23, 5};
#define MAX_MSG_LENGTH 23

#define NOTHING 0
#define SELF_TEST 1
#define SET_COLOR 2
#define GET_COLOR 3
#define NORMAL_MODE 4

#define MSGID 0
#define ADDRESS 1
#define HASH 2
#define COMACK 3
#define COMDATA 4
#define R1 5
#define G1 6
#define B1 7
#define R2 8
#define G2 9
#define B2 10
#define R3 11
#define G3 12
#define B3 13
#define R4 14
#define G4 15
#define B4 16
#define R5 17
#define G5 18
#define B5 19
#define R6 20
#define G6 21
#define B6 22

unsigned char msgId = 0;

unsigned char recvdMsg[MAX_MSG_LENGTH] = {0};
char recvdOffset = 0;

unsigned char sendMsg[MAX_MSG_LENGTH] = {0};


void setup_colors() {
  white.red = 255;
  white.green = 255;
  white.blue = 100;

  black.red = 0;
  black.green = 0;
  black.blue = 0;

  red.red = 255;
  red.green = 0;
  red.blue = 0;

  green.red = 0;
  green.green = 255;
  green.blue = 0;

  blue.red = 0;
  blue.green = 0;
  blue.blue = 255;

  magenta.red = 255;
  magenta.green = 0;
  magenta.blue = 255;
  
  cyan.red = 0;
  cyan.green = 255;
  cyan.blue = 255;
  
  yellow.red = 255;
  yellow.green = 255;
  yellow.blue = 0;
  
}

void setup_gpio() {
//  for(int i = 0; i < NUM_PEGS; i++) {
//    digitalWrite(sensor_pin1[i], HIGH);
//    digitalWrite(sensor_pin2[i], HIGH);
//    digitalWrite(on_off_pin[i], HIGH);
//  }  
  for(int i = 0; i < NUM_PEGS; i++) {
    pinMode(sensor_pin1[i], INPUT);
    pinMode(sensor_pin2[i], INPUT);
    pinMode(on_off_pin[i], INPUT);
  }
  pinMode(PIN_NUMBER_LED, OUTPUT);  

  pinMode(RX0, INPUT_PULLUP);
}

void setup_led() {
  for(int i = 0; i < NUM_PEGS; i++) {
    colors[i] = blue;
  }
  ledStrip.write(colors, NUM_PEGS);  
  current_color = "blue";
}

void setup_timer() {
//  Timer1.initialize(1000000);  
//  Timer1.attachInterrupt( clear_serial );
//  Timer1.stop();
//        setup_timer();
  MsTimer2::set(500, clear_serial);
}
void setup() {
//  if(debug) 
//  else 
//    Serial.end();

  setup_colors();
  setup_gpio();
  setup_led();

    Serial.begin(BAUD);

//  UCSR0B = 0;
}

void normalMode() {
  for(int i = 0; i < NUM_PEGS; i++) {
    sensor_input1[i] = digitalRead(sensor_pin1[i]);  
    sensor_input2[i] = digitalRead(sensor_pin2[i]);  
    on_off_input[i] = digitalRead(on_off_pin[i]);  

    if((sensor_pin1[i] == A6) || (sensor_pin1[i] == A7)) {
      sensor_input1[i] = (analogRead(sensor_pin1[i]) < 512)?0:1;
//      Serial.print("Analog 1 = ");
//      Serial.println(analogRead(sensor_pin1[i]));
    }

    if((sensor_pin2[i] == A6) || (sensor_pin2[i] == A7)) {
      sensor_input2[i] = (analogRead(sensor_pin2[i]) < 512)?0:1;      
//      Serial.print("Analog 2 = ");
//      Serial.println(analogRead(sensor_pin2[i]));
    }

/*    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(on_off_pin[i]);

    Serial.print("A7: ");
    Serial.println(A7);
*/
    if((on_off_pin[i] == A6) || (on_off_pin[i] == A7)) {
      int analogValue = analogRead(on_off_pin[i]);
      on_off_input[i] = (analogValue < 512)?0:1;      
      if(debug) Serial.print("Analog 3 = ");
      if(debug) Serial.println(analogValue);
    }

//    if(i == 2) {
      if(debug) Serial.print(i);
      if(debug) Serial.print(" - Sensor Input 1 = ");
      if(debug) Serial.print(sensor_input1[i]);
      if(debug) Serial.print(" - Sensor Input 2 = ");
      if(debug) Serial.print(sensor_input2[i]);
      if(debug) Serial.print(" - On Off Input = ");
      if(debug) Serial.println(on_off_input[i]);
//    }
  
  }

  bool change = false;

  for(int i = 0; i < NUM_PEGS; i++) {
//    if(delay_count[i] > 0) delay_count[i]--;
    if(on_off_input[i]) {
      if(prev_on_off_input[i] != on_off_input[i]) {
        prev_sensor_input2[i] = sensor_input2[i];
        prev_sensor_input1[i] = sensor_input1[i];
        change = true;
          offset[i] = (offset[i] - 1);
          if(offset[i] >= COLORS_COUNT) {
            offset[i] = 0;
          } else if(offset[i] < 0) {
            offset[i] = COLORS_COUNT - 1;            
          }
      } 
      if(debug) Serial.print(i);
      if(debug) Serial.println(" - PEG IN! ");
      colors[i] = *roll_colors[offset[i]]; 
      current_color = color_names[offset[i]];   
    } else {
      if(debug) Serial.print(i);
      if(debug) Serial.println(" - PEG OUT! ");
      colors[i] = black;
      current_color = "black";
    }
  }

  ledStrip.write(colors, NUM_PEGS);  

  for(int i = 0; i < NUM_PEGS; i++) {
    prev_sensor_input1[i] = sensor_input1[i];
    prev_sensor_input2[i] = sensor_input2[i];
    prev_on_off_input[i] = on_off_input[i];
  }

  if(change) {
//    delay(300);
    delay(100);
  } else {
    delay(100);
  }
}

void selfTest() {
/*    for(int i = 0; i < NUM_PEGS; i++) {
      colors[i] = green;
    }
    
    ledStrip.write(colors, NUM_PEGS);  

    return;
*/
  for(int i = 0; i < NUM_PEGS; i++) {
      offset[i] = (offset[i] + 1) % COLORS_COUNT;
//      colors[i] = *roll_colors[(offset[i] + i) % COLORS_COUNT];    
  }

  for(int i = 0; i < NUM_PEGS; i++) {
    colors[i] = *roll_colors[offset[0]];    
  }

  ledStrip.write(colors, NUM_PEGS);  

  delay(1000);
}

void setColor() {
/*
    for(int i = 0; i < NUM_PEGS; i++) {
      colors[i] = blue;
    }
    
    ledStrip.write(colors, NUM_PEGS);  

    return;
*/
  
  ledStrip.write(colors, NUM_PEGS);  
  delay(100);
}

int loop_count = 0;
#define NORMAL_MODE_RESET_TIME (30*10) // 10 seconds
#define SELF_TEST_MODE_RESET_TIME 30 // 10 seconds
#define SET_COLOR_MODE_RESET_TIME (30*10)

void loop() {
    if(runMode == NOTHING) {
/*     for(int i = 0; i < NUM_PEGS; i++) {
        colors[i] = red;
      }
*/
      // Do nothing
      if(debug) Serial.println("********** MODE: NOTHING *************");
      normalMode();
      if(loop_count > NORMAL_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == SELF_TEST) {
     for(int i = 0; i < NUM_PEGS; i++) {
//        colors[i] = green;
      }
      if(debug) Serial.println("********** MODE: SELF TEST *************");
      selfTest();
      if(loop_count > SELF_TEST_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == SET_COLOR) {
     for(int i = 0; i < NUM_PEGS; i++) {
//        colors[i] = white;
      }
      if(debug) Serial.println("********** MODE: SET COLOR *************");
      setColor();      
      if(loop_count > SET_COLOR_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == GET_COLOR) {
     for(int i = 0; i < NUM_PEGS; i++) {
//        colors[i] = magenta;
      }
      // Do nothing      
      if(debug) Serial.println("********** MODE: GET COLOR *************");
        loop_count = 0;
    } else if(runMode == NORMAL_MODE) {
     for(int i = 0; i < NUM_PEGS; i++) {
//        colors[i] = cyan;
      }
      if(debug) Serial.println("********** MODE: NORMAL MODE *************");
      normalMode();
      if(loop_count > NORMAL_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } 
//    colors[runMode] = black;
//    ledStrip.write(colors, NUM_PEGS);
    loop_count++;
}


void serialEvent() {

/*  for(int i = 0; i < NUM_PEGS; i++) {
    colors[i] = red;
  }
  ledStrip.write(colors, NUM_PEGS);  
  current_color = "red";
*/
  while (Serial.available()) {
    // get the new byte:
    unsigned char inChar = (unsigned char)Serial.read();
    if(recvdOffset == 0) {
//      Timer1.attachInterrupt( clear_serial );
//      Timer1.restart();
//      Timer1.start();
//      MsTimer2::start();      
//      reset_timer();
    }
    // add it to the inputString:
    recvdMsg[recvdOffset] = inChar;
    recvdOffset++;


    if(recvdOffset > COMDATA) {
      if(recvdOffset >= LENGTH_OF_THIS_MSG[recvdMsg[COMDATA]]) {
//        Timer1.detachInterrupt ();
//        Timer1.stop();
//       MsTimer2::stop();
//       cancel_timer();
       recvdOffset = 0;
       processMsg();
       loop_count = 0;
     }
    }
  }
}  

bool checkHash() {
  unsigned char hash = calcHash(recvdMsg);
  
  if(hash == (unsigned char) recvdMsg[HASH]) {
    return true;
  } else {
    return false;
  }

}

unsigned char calcHash(unsigned char* msg) {
  unsigned char hash = 0;
  for(int i = 0; i < LENGTH_OF_THIS_MSG[msg[COMDATA]]; i++) {
    if(i == HASH) {
      
    } else {
      hash ^= recvdMsg[i];
    }
  }

  return hash;
}

void createNack() {
  sendMsg[MSGID] = msgId++;
  sendMsg[ADDRESS] = MY_ADDRESS;
  sendMsg[HASH] = 0;
  sendMsg[COMACK] = 'N';
  sendMsg[COMDATA] = calcHash(recvdMsg);
  sendMsg[HASH] = calcHash(sendMsg);
}

void createMsg() {
  sendMsg[MSGID] = msgId++;
  sendMsg[ADDRESS] = MY_ADDRESS;
  sendMsg[HASH] = 0;
  sendMsg[COMACK] = 'A';
  sendMsg[COMDATA] = recvdMsg[COMDATA];
}

void txMsg(unsigned char* msg) {
//    for(int i = 0; i < NUM_PEGS; i++) {
//      colors[i] = white;
//    }
//    
//    ledStrip.write(colors, NUM_PEGS);   
  delay(100);
  if(msg[COMACK] == 'N') {
    for(int i = 0; i < TX_LENGTH_OF_THIS_MSG[NOTHING]; i++) {
      Serial.print((char)msg[i]);
      //delay(100);
    }  
  } else {
    for(int i = 0; i < TX_LENGTH_OF_THIS_MSG[msg[COMDATA]]; i++) {
      Serial.print((char)msg[i]);
      //delay(100);
    }
  }
}

void processMsg() {
  if(recvdMsg[ADDRESS] != MY_ADDRESS && recvdMsg[ADDRESS] != (unsigned char) ADDRESS_ALL) { 
    return;
  }
  if(!checkHash()) {    // BRING ME BACK

    createNack();

//    return txMsg(recvdMsg);
    
  } else if(recvdMsg[COMACK] != 'C') {
//    for(int i = 0; i < NUM_PEGS; i++) {
//      colors[i] = white;
//    }
//    
//    ledStrip.write(colors, NUM_PEGS);   
    return;
  } else {
//    for(int i = 0; i < NUM_PEGS; i++) {
//      colors[i] = yellow;
//    }
//    
//    ledStrip.write(colors, NUM_PEGS);  

    if(runMode != GET_COLOR) {
      prev_runMode = runMode;
    }
    runMode = recvdMsg[COMDATA];
//    colors[runMode] = white;
//    ledStrip.write(colors, NUM_PEGS);   
    createMsg();
//    Serial.println(runMode+48);
    if(runMode == SET_COLOR) {
      for(int i = 0; i < NUM_PEGS; i++) {
          colors[i].red = recvdMsg[R1 + i*3];
          colors[i].green = recvdMsg[G1 + i*3];
          colors[i].blue = recvdMsg[B1 + i*3];
      }
/*    ledStrip.write(colors, NUM_PEGS);   
  */    
/*        colors[0].red = recvdMsg[R1];
        colors[0].green = recvdMsg[G1];
        colors[0].blue = recvdMsg[B1];
        colors[1].red = recvdMsg[R2];
        colors[1].green = recvdMsg[G2];
        colors[1].blue = recvdMsg[B2];
        colors[2].red = recvdMsg[R3];
        colors[2].green = recvdMsg[G3];
        colors[2].blue = recvdMsg[B3];
        colors[3].red = recvdMsg[R4];
        colors[3].green = recvdMsg[G4];
        colors[3].blue = recvdMsg[B4];
        colors[4].red = recvdMsg[R5];
        colors[4].green = recvdMsg[G5];
        colors[4].blue = recvdMsg[B5];
        colors[5].red = recvdMsg[R6];
        colors[5].green = recvdMsg[G6];
        colors[5].blue = recvdMsg[B6];
        */
    } else if(runMode == GET_COLOR) {
      //Serial.println("inside Get color");
      for(int i = 0; i < NUM_PEGS; i++) {
        //Serial.println("inside for");
        sendMsg[R1 + i*3] = colors[i].red;
        sendMsg[G1 + i*3] = colors[i].green;
        sendMsg[B1 + i*3] = colors[i].blue;
      }
    }
    sendMsg[HASH] = calcHash(sendMsg);
  }
  //delay(100);
  if(recvdMsg[ADDRESS] != ADDRESS_ALL) {
    txMsg(sendMsg);   // BRING ME BACK
  }
  if(runMode == GET_COLOR) {
    runMode = prev_runMode;
  }
}




/************************************
 * 
 * 
 * TIMERS
 * 
 * 
 * *********************************/


unsigned long compare_count = 2*5; //seconds
unsigned long step_count = 0;


/*unsigned long compare_count = 5; //seconds
unsigned long step_count = 0;
unsigned int mult_factor = 62500; // for 1 second

void setup_timer(){

//  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 1*mult_factor;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 bit for 256-prescaler
  TCCR1B |= (1 << CS12);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  step_count = 0;
  sei();//allow interrupts

}//end setup
*/
/*

void reset_timer()
{
  TIMSK1 |= (1 << OCIE1A);
  step_count = 0;
}

void cancel_timer()
{
  TIMSK0 &= ~_BV(TOIE0);
  TIMSK1 &= ~(1 << OCIE1A);  
}
*/
void clear_serial() {
//  step_count++;
//  if(step_count >= compare_count) {    
    recvdOffset = 0;
//    step_count = 0;
//  }
}

/*
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
//  boolean value = 0;
  step_count++;
  if(step_count >= compare_count) {    // 100000 - 1 second
    recvdOffset = 0;
    step_count = 0;
  }
}

*/
