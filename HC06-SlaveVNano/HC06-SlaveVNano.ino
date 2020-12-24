#include <SoftwareSerial.h>  //Software Serial Port  
#define RxD 3    //Pin BRANCHEE SUR LE TX du HC06
#define TxD 2    //Pin BRANCHEE SUR LE RX du HC06 

SoftwareSerial BTSerie(RxD,TxD);  
String msg="";
int i=0;
int RedLed = 4;

void setup()  
{  
  Serial.begin(9600);    //115200 si on veut
  delay(500);
  Serial.println("HC-06 Bonjour depuis le Nano");
  pinMode(RxD, INPUT);  
  pinMode(TxD, OUTPUT);
  pinMode(RedLed, OUTPUT);
  BTSerie.begin(9600);
  delay(500); 
  BTSerie.print("AT+VERSION?");  //Demande la version
  Serial.print( BTSerie.read() );     // afficher sur console ce qui est lu sur BT
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
    Serial.print("Master sent : " );
    Serial.println(msg);
    if(msg=="O") 
    {
        digitalWrite(RedLed,HIGH);
    }
    else if (msg=="N")
    {
      digitalWrite(RedLed,LOW);
    }
    msg="";
  } 
  if (Serial.available())
  {  
    recvChar = Serial.read();  
    BTSerie.write(recvChar);  
  }  
}
