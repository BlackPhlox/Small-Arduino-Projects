//Sonic Distance Program (Metric) || HC-SR04
//Controls : E to enable, D to disable (lowercase)
//Copyright (C) 2017 by Mikkel Luja Rasmussen. All Rights Reserved

int triggerPin = 2; // Green wire
int echoPin = 3; // Blue wire

//Diffiner Variabler
long duration;
float distance;
char incomingChar;
boolean hcSystem = false;
int distTemp = 1000;

//Køres én gang før loop functionen
void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

//Køres indtil noLoop() eller
//nå der ikke er strøm til arduinoen
void loop() {
  //Hvis der kommer input fra serial komandoprompt
  if (Serial.available() > 0) {
    commandControl();
  }
  distCalcSystem(true);
}

void distCalcSystem(boolean updateByTime){
  if (hcSystem == true){
  //Reset'er distancen
  int x = 1000;
  //Få ny distance
  x = getDist();

  //Søger om der kommer et
  //læsligt signal (når distancen > 0)
  if (x == 0 && distTemp != 0){
    Serial.println("Searching for surface");
    distTemp = 0;
  } else if (distTemp == 0){
      Serial.print(".");
  }

 //Hvis distacen ikke er nul (Out of range)
 if (x != 0){
  //Pharse boolean
  if (updateByTime == false){
    //Vis hvis det er en ny værdi ift. forrige distance
      if (distTemp != x){
        serialDisplayDist(x);
      }
    } else {
        serialDisplayDist(x);
        //En hundrededel tidsforsinkelse
        delay(100);
      }
    //Set ny temp distance
    distTemp = distance;
    }
  }
}

void commandControl(){
  incomingChar = Serial.read();
  Serial.println(incomingChar);
    if (incomingChar == 'e'){
      hcSystem = true;
      Serial.println("hcSystem enabled");
    } else if (incomingChar == 'd'){
      hcSystem = false;
      Serial.println("hcSystem disabled");
      }
}

void serialDisplayDist(int x){
  Serial.print("Distance: ");
  Serial.print(x);
  Serial.println(" cm");
}

unsigned long getDist() {
  //Reset Trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  //Transmitere lyd for 10 microsekunder
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  //Aflæser echo'et fra den transmiteret lyd i microsekunder
  duration = pulseIn(echoPin, HIGH);
  if (duration == 0){
    return 0;
  }

  //Omregner fra microsekunder til distance
  distance = duration * 0.039 / 2;

  //Usikkerhed med 1+/- cm
  //tone(leftSoundPin,map(distance,0,400,200,600),100);
  return distance;
}
