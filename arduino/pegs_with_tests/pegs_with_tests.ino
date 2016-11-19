#include<PololuLedStrip.h>

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
int offset_normalMode[NUM_PEGS] = {0};
int offset_selfTest[NUM_PEGS] = {0};

rgb_color white, red, green, blue, black, magenta, cyan, yellow;

#define COLORS_COUNT 7
rgb_color* roll_colors[COLORS_COUNT] = {&cyan, &yellow, &magenta, &green, &blue, &red, &white};
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

void setup() {
  setup_colors();
  setup_gpio();
  setup_led();
  
  Serial.begin(BAUD);
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
          offset_normalMode[i] = (offset_normalMode[i] - 1);
          if(offset_normalMode[i] >= COLORS_COUNT) {
            offset_normalMode[i] = 0;
          } else if(offset_normalMode[i] < 0) {
            offset_normalMode[i] = COLORS_COUNT - 1;            
          }
      } 
      if(debug) Serial.print(i);
      if(debug) Serial.println(" - PEG IN! ");
      colors[i] = *roll_colors[offset_normalMode[i]]; 
      current_color = color_names[offset_normalMode[i]];   
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
    delay(100);
  } else {
    delay(100);
  }
}

void selfTest() {
  for(int i = 0; i < NUM_PEGS; i++) {
      offset_selfTest[i] = (offset_selfTest[i] + 1) % COLORS_COUNT;
//      colors[i] = *roll_colors[(offset[i] + i) % COLORS_COUNT];    
  }

  for(int i = 0; i < NUM_PEGS; i++) {
    colors[i] = *roll_colors[offset_selfTest[0]];    
  }

  ledStrip.write(colors, NUM_PEGS);  

  delay(1000);
}

void setColor() {
  ledStrip.write(colors, NUM_PEGS);  
  delay(100);
}

int loop_count = 0;
#define NORMAL_MODE_RESET_TIME (30*10) // 10 seconds
#define SELF_TEST_MODE_RESET_TIME 30 // 10 seconds
#define SET_COLOR_MODE_RESET_TIME (30*10)

void loop() {
    if(runMode == NOTHING) {
      // Do nothing
      if(debug) Serial.println("********** MODE: NOTHING *************");
      normalMode();
      if(loop_count > NORMAL_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == SELF_TEST) {
      if(debug) Serial.println("********** MODE: SELF TEST *************");
      selfTest();
      if(loop_count > SELF_TEST_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == SET_COLOR) {
      if(debug) Serial.println("********** MODE: SET COLOR *************");
      setColor();      
      if(loop_count > SET_COLOR_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } else if(runMode == GET_COLOR) {
      // Do nothing      
      if(debug) Serial.println("********** MODE: GET COLOR *************");
        loop_count = 0;
    } else if(runMode == NORMAL_MODE) {
      if(debug) Serial.println("********** MODE: NORMAL MODE *************");
      normalMode();
      if(loop_count > NORMAL_MODE_RESET_TIME) {
        loop_count = 0;
        clear_serial();
      }
    } 
    loop_count++;
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    unsigned char inChar = (unsigned char)Serial.read();
    // add it to the inputString:
    recvdMsg[recvdOffset] = inChar;
    recvdOffset++;

    if(recvdOffset > COMDATA) {
      if(recvdOffset >= LENGTH_OF_THIS_MSG[recvdMsg[COMDATA]]) {
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
  } else if(recvdMsg[COMACK] != 'C') {
    return;
  } else {
    if(runMode != GET_COLOR) {
      prev_runMode = runMode;
    }
    runMode = recvdMsg[COMDATA];
    createMsg();
    if(runMode == SET_COLOR) {
      for(int i = 0; i < NUM_PEGS; i++) {
          colors[i].red = recvdMsg[R1 + i*3];
          colors[i].green = recvdMsg[G1 + i*3];
          colors[i].blue = recvdMsg[B1 + i*3];
      }
    } else if(runMode == GET_COLOR) {
      for(int i = 0; i < NUM_PEGS; i++) {
        sendMsg[R1 + i*3] = colors[i].red;
        sendMsg[G1 + i*3] = colors[i].green;
        sendMsg[B1 + i*3] = colors[i].blue;
      }
    }
    sendMsg[HASH] = calcHash(sendMsg);
  }
  if(recvdMsg[ADDRESS] != ADDRESS_ALL) {
    txMsg(sendMsg);   // BRING ME BACK
  }
  if(runMode == GET_COLOR) {
    runMode = prev_runMode;
  }
}

void clear_serial() {
    recvdOffset = 0;
}

