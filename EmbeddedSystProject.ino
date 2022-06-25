// Written By : Lana Qawasmy 

#include <SoftwareSerial.h>
#include <Servo.h>


// initialize the variables :


SoftwareSerial SoftSerial(6, 7);   // RX.TX

Servo servoM;
int ServoMotor = 11;

int Motorpin1 = 2;
int Motorpin2 = 3;
int EN1 = 9;

// Define pins for ultrasonic 
int const echoPin = 4;
int const trigPin = 5;

char command;
int Motorspeed = 255;


//-------------------------------------------------------------------------------------------------

void setup()
{
  // initialize the pins :
  
  SoftSerial.begin(9600);  // Bluetooth connection
  Serial.begin(9600);      // Serial monitor

  servoM.attach(ServoMotor);
  
  pinMode(Motorpin1,OUTPUT);
  pinMode(Motorpin2,OUTPUT);
  pinMode(ServoMotor,OUTPUT);
  
  pinMode(EN1,OUTPUT); 
  pinMode(echoPin, INPUT); // echo pin should be input to get pulse width
  pinMode(trigPin, OUTPUT); // trig pin will have pulses output
  
}


//----------------------------------------------------------------------------------------------------


void loop()
{
  
    checkState();
   //Serial.println(checkState());

  Serial.println(Motorspeed);
  
  if(SoftSerial.available() > 0){ 
    
    char data = (char)SoftSerial.read();
    command = data;
    Serial.println(command);


    // GO Forward
    
    if(command == 'F' && checkState()){ 
      Serial.println("Forward");
      digitalWrite(Motorpin1, HIGH);  
      digitalWrite(Motorpin2, LOW);
      analogWrite(EN1,Motorspeed);
    }

    // GO BackWard
    
    if(command == 'B' && checkState()){
      Serial.println("BackWard");
      digitalWrite(Motorpin1, LOW);
      digitalWrite(Motorpin2, HIGH);
      analogWrite(EN1,Motorspeed);
     }
    
    // GO Left
    
    if(command == 'L'){
       Serial.println("Left");
       servoM.write(10); 

     }

    // GO Right
    
    if(command == 'R'){
      Serial.println("Right");
      servoM.write(150); 
    }


    // Stop ( NO Motion )
    
    if(command == 'S'){ 
      digitalWrite(Motorpin1, HIGH);
      digitalWrite(Motorpin2, HIGH);
      servoM.write(40); 
    }

    //  change speed (Increase)
    
    if(command == '+'){
      Serial.println("Increase speed");
      digitalWrite(Motorpin1, HIGH);  
      digitalWrite(Motorpin2, LOW);
      analogWrite(EN1,255);
    }

    //  change speed (Decrease)
    
    if(command == '-' ){
      Serial.println("Decrease speed");
      digitalWrite(Motorpin1, HIGH);  
      digitalWrite(Motorpin2, LOW);
      analogWrite(EN1,255);
    }
    
  }
}

//--------------------------------------------------------

// Functions : 

void reset(){
  digitalWrite(Motorpin1, HIGH);
  digitalWrite(Motorpin2, HIGH);
}

//-------------------------------------------------------

bool checkState(){
  
  // Duration will be the input pulse width and distance will be the distance to the obstacle in centimeters
  long duration;
  long distance;

  // Clears the trigPin condition
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  // Sets the trigPin HIGH (Active) for 10 microseconds
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  // Reads the echoPin , returns the sound wave travel time in microseconds
  duration=pulseIn(echoPin,HIGH);

  // Distance is half the duration devided by 29.1 (from datasheet)
  distance= (duration/2) / 29.1;
  //  Serial.println(String(distance));

 // if distance less than 0.1 meter and more than 0 (0 or less means over range)
  if(distance < 10 && distance >= 0){
    reset();
    return false;
  }
  return true;
}
