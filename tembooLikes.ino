#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

//set up adafruit LCD backpack
Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();

//lookup table to for more visible brightness changes
uint8_t sweep[] = {1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1};

//set up vars for serial control
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        

// variables to hold the parsed data
char commandType[numChars] = {0}; //get command type, in this example only IG is recognized
int totalLikes = 0; 
boolean newData = false;

//============

void setup() {
  Serial.begin(19200);
  //verify LCD is connected
  if (! matrix.begin()) {
    Serial.println("LCDs Not Found");
    while (1);
   }
    Serial.println("LCDs Online");
    delay(500);
    Serial.println("Temboo: IG Likes Reporter Now Online");
  }
  
//============

void loop() {
    waitFade();
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
        parseData();
        showCommand();
        runLCD();
        newData = false;
        delay(1000);
        Serial.print("No new data, returning to idle.");
        waitFade();
        
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    //get serial data
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      
    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(commandType, strtokIndx); // copy it to commandType
 
    strtokIndx = strtok(NULL, ","); // get next value
    totalLikes = atoi(strtokIndx);     // convert this part to an integer


}

//============

void showCommand() {
    //return command info back over serial to sender
    Serial.print("Command Recieved: ");
    Serial.println(receivedChars);
    Serial.print("Command Type: ");
    Serial.println(commandType);
    Serial.print("Likes: ");
    Serial.println(totalLikes);
}
//============

void runLCD() {
  matrix.clear();
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  
  matrix.setTextColor(100);
  for (int8_t x=0; x>=-32; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("TEMBOO!");
    //delay(100);
  }
  matrix.clear();

  delay(500);
  //draw heart and = sign
  matrix.drawPixel(0,1,255);
  matrix.drawPixel(0,2,255);
  matrix.drawPixel(0,3,255);
  matrix.drawPixel(0,4,255);
  matrix.drawPixel(1,0,255);
  matrix.drawPixel(1,1,255);
  matrix.drawPixel(1,5,255);
  matrix.drawPixel(2,1,255);
  matrix.drawPixel(2,6,255);
  matrix.drawPixel(3,2,255);
  matrix.drawPixel(3,7,255);
  matrix.drawPixel(4,1,255);
  matrix.drawPixel(4,6,255);
  matrix.drawPixel(5,0,255);
  matrix.drawPixel(5,1,255);
  matrix.drawPixel(5,5,255);
  matrix.drawPixel(6,1,255);
  matrix.drawPixel(6,2,255);
  matrix.drawPixel(6,3,255);
  matrix.drawPixel(6,4,255);
  matrix.drawPixel(9,3,100);
  matrix.drawPixel(10,3,100);
  matrix.drawPixel(11,3,100);
  matrix.drawPixel(12,3,100);
  matrix.drawPixel(13,3,100);
  matrix.drawPixel(14,3,100);
  matrix.drawPixel(9,5,100);
  matrix.drawPixel(10,5,100);
  matrix.drawPixel(11,5,100);
  matrix.drawPixel(12,5,100);
  matrix.drawPixel(13,5,100);
  matrix.drawPixel(14,5,100);
  delay(1000);
  matrix.clear();
  delay(500);
  matrix.clear();
  matrix.setCursor(3,1);
  matrix.print(totalLikes);
  delay(4000);
}

void waitFade(){
    //idle fading
    for (uint8_t incr = 0; incr < 24; incr++)
    for (uint8_t x = 0; x < 16; x++)
      for (uint8_t y = 0; y < 9; y++)
        matrix.drawPixel(x, y, sweep[(x+y+incr)%24]);
     delay(20);
}

