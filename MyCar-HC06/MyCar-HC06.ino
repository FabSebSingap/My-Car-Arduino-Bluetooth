/* Remote Car with *******************************************
 *  + 2 DC motors
 *  + Servo motor Micro Servo 90
 *  + HC06 Bluetooth 
 *  + UC-025 Ultra Sonic
 *  + L293D chipet to use DC motors
 *  THe remote control is managed by a Android smartphone
 *  See on my Git FabSebSingap the Android Code
 *  Performed by Fabrice SEBILLE
 *  Version 2020-12-23
 ***********************************************************/

#include <SoftwareSerial.h> //Library Serial Port  
#include <Servo.h>          //Library for servi motor

Servo myservo;

// Pin for the US-025
#define TRIG_PIN 13
#define ECHO_PIN 14

/*Connection for HC06 ***********************************************
 * Before to use it configure in AT mode 
 * See on my Git FabSebSingap a HC06 AT Confgurator program
 * AT+ADDR? to get the MAC address. To pair it with Android program
 * AT to test it. It answer OK
 * HC06 BAUD mode is 9600,0,1 by default
 * AT+NAME:<Choose a name>
 *******************************************************************/
#define RxD 3    //Pin BRANCHEE SUR LE TX du HC06
#define TxD 2    //Pin BRANCHEE SUR LE RX du HC06 
SoftwareSerial BTSerie(RxD,TxD);  

/****************************************************
Motor using the L293D chip
Connect Left Motor on Pin 3 and Pin 6 of L293D
Connect Right Motor on Pin 11 and Pin 14 of L293D
****************************************************/
// Motor Left
#define EnableML 5    //Pin 1 on L293D
#define MotorPin1 6   //Pin 2 on L293D
#define MotorPin2 7   //Pin 7 on L293D

//Motor Right
#define EnableMR 8    //Pin 9 on L293D
#define MotorPin3 9   //Pin 10 on L293D
#define MotorPin4 10  //Pin 15 on L293D

/*************
Servo motor
*************/
#define ServoPin 11

#define RedLed 4

String msg="";
int i=0;

#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  800
#define NOTE_B5  988
#define NOTE_C6  1047

#define BuzzerPin 12

int melody[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

int duration = 500;  // 500 miliseconds

void setup()  
{
  //Sequence to initialize HC06**********************************************************
  Serial.begin(9600);    //115200 si on veut
  delay(500);
  Serial.println("HC-06 Bonjour");
  pinMode(RxD, INPUT);  
  pinMode(TxD, OUTPUT);
  BTSerie.begin(9600);
  delay(500); 
  BTSerie.print("AT+VERSION?");  //Demande la version Just to test from Serial monitor
  delay(500); 
  Serial.print( BTSerie.read() );     // afficher sur console ce qui est lu sur BT
  //*************************************************************************************

  //Set Pin for DC Motors**************
  pinMode(EnableML,OUTPUT);
  pinMode(MotorPin1,OUTPUT);
  pinMode(MotorPin2,OUTPUT);
  pinMode(EnableMR,OUTPUT);
  pinMode(MotorPin3,OUTPUT);
  pinMode(MotorPin4,OUTPUT);
  //**********************************

  //Set Pin for US-025 Utrasonic******
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //**********************************

  // Servo motor initialization*******
  myservo.attach(ServoPin);
  myservo.write(90); // Turn servo to middle

  
  
}  

void loop()  
{  
  char recvChar; 
  //On lit caractere par caractere sur le BTSerie et on affiche sur le Terminal Serie 
  delay(10);
  if (BTSerie.available() >0)
  {
    char c = BTSerie.read();  //gets one byte from serial buffer
    msg += c;                 //makes the string readString
  }

  if(msg!="")
  {
    Serial.println(msg);
    if(msg=="F") 
    {
        digitalWrite(RedLed,HIGH);
        car_forward();
    }
    else if (msg=="B")
    {
      digitalWrite(RedLed,LOW);
      car_back();
    }
    else if (msg=="S")
    {
      digitalWrite(RedLed,LOW);
      car_stop();
    }
     else if (msg=="L")
    {
      digitalWrite(RedLed,LOW);
      car_left();
    }
    else if (msg=="R")
    {
      digitalWrite(RedLed,LOW);
      car_right();
    }
    msg="";
  } 
  if (Serial.available())
  {  
    recvChar = Serial.read();  
    BTSerie.write(recvChar);  
  }  
}

void car_forward(){
  digitalWrite(EnableML,HIGH); // enable on
  digitalWrite(EnableMR,HIGH); // enable on
  digitalWrite(MotorPin1,1);
  digitalWrite(MotorPin2,0);
  digitalWrite(MotorPin3,1);
  digitalWrite(MotorPin4,0);
}

void car_back(){
  digitalWrite(EnableML,HIGH); // enable on
  digitalWrite(EnableMR,HIGH); // enable on
  digitalWrite(MotorPin1,0);
  digitalWrite(MotorPin2,1);
  digitalWrite(MotorPin3,0);
  digitalWrite(MotorPin4,1);
}

void car_stop(){
  digitalWrite(EnableML,LOW); // enable off
  digitalWrite(EnableMR,LOW); // enable off
}

void car_right(){
  digitalWrite(EnableML,HIGH);
  digitalWrite(EnableMR,LOW);
  digitalWrite(MotorPin1,0);
  digitalWrite(MotorPin2,1);
  digitalWrite(MotorPin3,0);
  digitalWrite(MotorPin4,0);
}

void car_left(){
  digitalWrite(EnableML,LOW);
  digitalWrite(EnableMR,HIGH);
  digitalWrite(MotorPin1,0);
  digitalWrite(MotorPin2,0);
  digitalWrite(MotorPin3,0);
  digitalWrite(MotorPin4,1);
}

int echoInMicroseconds(){
  int pulseBack;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);  
  while(digitalRead(ECHO_PIN) == LOW);
  int pulseStart = micros();
  
  while(digitalRead(ECHO_PIN) == HIGH);

  pulseBack = micros() - pulseStart;
 
  return pulseBack;
  
}

long calculDistance(){
  int duration = echoInMicroseconds();
  // Given the speed of sound in air is 332m/s = 3320cm/s = 0.0332cm/us).
  long distance = (duration / 2) * 0.332;
  return distance;
  
  if (distance<0)
  {
    return 0;
  }
}

void Buzzer (){
  for (int thisNote = 0; thisNote < 3; thisNote++) {
    // pin8 output the voice, every scale is 0.5 sencond
    tone(BuzzerPin, melody[thisNote], duration);
    delay(800);
  }
}
