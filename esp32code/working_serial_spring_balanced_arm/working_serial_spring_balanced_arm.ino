//for encoder
#include <ESP32Encoder.h>
ESP32Encoder BaseEnc;
ESP32Encoder ShoulderEnc;
ESP32Encoder ElbowEnc;

//for motors 
const int BaseMotor = 25; // top pin right pin
const int ShoulderMotor = 26; // second top right pin
const int ElbowMotor = 27; //5th top right pin
const int AmpInput = 14; //6th top right pin


void setup() {
  Serial.begin(921600);
  // encoder setup
  // Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors = puType::up;
  
	// use pin 19 and 18 for the first encoder
	BaseEnc.attachFullQuad(32, 33);
  ShoulderEnc.attachFullQuad(34, 35);
  ElbowEnc.attachFullQuad(36, 39);
  //sets values for zeroing 
  BaseEnc.setCount(-3000/2);
  ShoulderEnc.setCount(-2600/135*80);
  ElbowEnc.setCount(500); 
  
  // motor output setup
  pinMode(BaseMotor, OUTPUT);
  delay(100);
  pinMode(ShoulderMotor, OUTPUT);
  delay(100);
  pinMode(ElbowMotor, OUTPUT);
  delay(100);
  // opamp output setup
  pinMode(AmpInput, OUTPUT);
  delay(100);
  //sets all the outputs so robot isnt moving
  analogWrite(AmpInput, 120);
  analogWrite(BaseMotor, 120);
  analogWrite(ShoulderMotor, 120);
  analogWrite(ElbowMotor, 120);
  
  delay(2000);
}

long int time_before_write;
long int time_after_write;

String EncoderValProcess = " ";
char EncoderValOutput[20] = {};
int *EncoderVals[2]={};
int MaxBytesForWrite = 20;
//max is 16, lowest is 12
int MinBytesForRead = 14;
char TerminatingChar = ';';
int PWMOutput[3] = {};
int PointInArray = 0;
char testarray[12] ={'1','1','1',' ','2','2','2',' ','3','3','3'};
const int PWMReadValuesLength = 12;
char PWMReadValues[PWMReadValuesLength] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

// Input array struct: {'#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', ';'}
// Output array struct: {'±', '#', '#', '#', '#', ' ', '±', '#', '#', '#', '#', ' ', '±', '#', '#', '#', ';'}

String num = "";
char c;
int vals[] = {0, 0, 0};

void loop() {
  
 
  
  time_before_write = micros();
  //records values for writing to computer for encoder values
  
  EncoderValProcess = String(BaseEnc.getCount()) + ":" + String(ShoulderEnc.getCount()) + ":" + String(ElbowEnc.getCount()) + ";" + '\n'; 
  
  EncoderValProcess.toCharArray(EncoderValOutput, 20);
  
 
  
  //if reading slows down function enough can remove
  while (Serial.availableForWrite() < MaxBytesForWrite){}
  Serial.write(EncoderValOutput, MaxBytesForWrite);

  //for reading 
  if(Serial.available()){// checks that there is a char to read
    //Serial.println("test");
    c = Serial.read();//stores in value from buffer
    if (c == ':') {//checks for end of values
      vals[PointInArray] = num.toInt(); // sets values to the assinged joint in vals
      num = "";// reset 
      PointInArray++;
    } else if (c == ';') {
      vals[PointInArray] = num.toInt();
      num = "";
      PointInArray = 0;
      //Serial.write('1');
      analogWrite(BaseMotor,vals[0]);
      analogWrite(ShoulderMotor,vals[1]);
      analogWrite(ElbowMotor,vals[2]);
      
    } else {
      num += c;
    }

  }
  //match with ros2 code
  while(time_after_write - time_before_write < 300){
    time_after_write =micros();
  }
  //Serial.println(time_before_write - time_after_write);
  
}