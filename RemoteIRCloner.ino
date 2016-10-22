/*
* 
* Author: Enrique Wood (ewoodg@hotmail.com)
* Description: This code allows you to copy any remote controller's code sequences
*              the output is the time (in microseconds) that the IR led is turned on,
*              then off and so on.
*              You can retransmit those codes with an IR LED (or even paste the pattern
*              to Android code as I used this tool to)
*/

const int iRSensor = A0;     //IR receiver (phototransistor) 
const int ledPin = 13;          //Led pin to indicate input's feedback (faster than Serial feedback)
boolean reading = false;        //Indicates whether the reading has or not started 
byte changes = 0;               //Index for the pulse's array
int current = 0;                //Last value from the IR sensor
int prev = 0;                   //Previous value from the IR sensor

unsigned long pulse[255];       //Store the whole pulse times to format and show data
unsigned long time  = 0;        //Microseconds in last change
unsigned long diffTime = 0;     //Difference between now and time

void setup() {
  Serial.begin(57600); 
  pinMode(iRSensor,INPUT);
  pinMode(ledPin,OUTPUT);
}

void loop() {
 
  //Read the logic value of the analog input
  current = analogRead(iRSensor) >0?1:0;  //Converts analogic signals to digital  
            
  
  if(current == 1 && !reading){ //First 1 received: start reading
    reading = true;  
    //Serial.println("Reading new pulse..."); //Not recomended to use as it slows loops and reading
    prev = current;
    time = micros();
    digitalWrite(ledPin,HIGH);  
  }
  
  if(reading){
    // It already received the first one and the value just read is different from prev
    if(current != prev){ 
      pulse[changes] = micros() - time;
      time = micros();
      prev = current;
      changes++;
      digitalWrite(ledPin,current);
    }
    diffTime = (micros()-time);
    if(prev == 0 && diffTime > 1000000){
      int totalTime = 0,i;
      
      for(i=0;i<changes;i++) totalTime += pulse[i];
      Serial.print("\nPulse read succesfully! (");
      Serial.print("changes: ");
      Serial.print(changes);
      Serial.print(" in ");
      Serial.print(totalTime);
      Serial.println(" microseconds)");
      
      time = 0;
      reading = false;
      printPulseFormatted(changes);
      changes = 0;
    }    
  }
}

//Formats the on/off's times as an array
void printPulseFormatted(int changes){
  int i;
  Serial.println("//Code auto-generated from Arduino");
  Serial.print("int[] pattern = {");
  for(i = 0;i<changes-1; i++){
    if(i!=0 && i%12 == 0) Serial.println("");
    Serial.print(pulse[i]);
    Serial.print(", ");
  }
  Serial.print(pulse[i]);
  Serial.println("};");  
}  

