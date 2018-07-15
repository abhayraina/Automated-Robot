int frontTrig = 29;
int frontEcho =  28;
int rightFrontTrig = 31;
int rightFrontEcho =  30;
int rightBackTrig = 6;    //unused for localization
int rightBackEcho =  7;   //unused for localization
int backTrig =  23;
int backEcho =   22;
int leftBackTrig = 25;    //unused for localization
int leftBackEcho =  24;   //unused for localization
int leftFrontTrig =  27;
int leftFrontEcho =   26;

int frontUltra = 0;
int rightFrontUltra = 0;
int rightBackUltra = 0;
int backUltra = 0;
int leftBackUltra = 0;
int leftFrontUltra = 0;

long duration;
int distance;

void setup()
{
  Serial.begin(9600);
  pinMode(12,OUTPUT); //A motor dir
  pinMode(9, OUTPUT); //A brake
  digitalWrite(9,LOW);
  pinMode(13,OUTPUT); //B motor dir
  pinMode(8, OUTPUT); //B brake
  digitalWrite(8,LOW);
  
  pinMode(frontTrig, OUTPUT); // Sets the trigPin as an Output
  pinMode(frontEcho, INPUT);
  pinMode(rightFrontTrig, OUTPUT);
  pinMode(rightFrontEcho, INPUT);
  pinMode(rightBackTrig, OUTPUT);
  pinMode(rightBackEcho, INPUT);
  pinMode(backTrig, OUTPUT);
  pinMode(backEcho, INPUT);
  pinMode(leftBackTrig, OUTPUT);
  pinMode(leftBackEcho, INPUT);
  pinMode(leftFrontTrig, OUTPUT);
  pinMode(leftFrontEcho, INPUT);
}

void loop()
{

  if (Serial.available())
  {
    char key = Serial.read();
    if(key == 'w')
    {
      Serial.println("Forward");
      moveA(75,true);  //150
      moveB(80,false); //160
      delay(285);
      moveA(75,false);  //150
      moveB(80,true); //160
      delay(2);
      fullstop();
      delay(1000);
    }
    else if(key == 'a')
    {
      Serial.println("Left");
      moveA(80,false);
      moveB(80,false);
      delay(390);
      fullstop();
      delay(1000);
    }
    else if(key == 'd')
    {
      Serial.println("Right");
      moveA(80,true);
      moveB(80,true);
      delay(375);
      fullstop();
      delay(1000);
    }
    else if(key == 's')
    {
      Serial.println("Back");
      moveA(75,false);  //150
      moveB(80,true); //160
      delay(285);
      moveA(75,true);  //150
      moveB(80,false); //160
      delay(2);
      fullstop();
      delay(1000);
    }
    else if(key == 'u')
    {
      frontUltra = getUltra(frontTrig,frontEcho);
      rightFrontUltra = getUltra(rightFrontTrig,rightFrontEcho);
      rightBackUltra = 1000;
//      rightBackUltra = getUltra(rightBackTrig,rightBackEcho);
      backUltra = getUltra(backTrig,backEcho);
      leftBackUltra = 1000;
//      leftBackUltra = getUltra(leftBackTrig,leftBackEcho);
      leftFrontUltra = getUltra(leftFrontTrig,leftFrontEcho);
      
      Serial.print(frontUltra);
      Serial.print(",");
      Serial.print(rightFrontUltra);
      Serial.print(",");
      Serial.print(rightBackUltra);   //unused for localization
      Serial.print(",");
      Serial.print(backUltra);
      Serial.print(",");
      Serial.print(leftBackUltra);    //unused for localization
      Serial.print(",");
      Serial.println(leftFrontUltra);
    }
  }
  else
  {

  }
}

void moveA(int speed, boolean rev)
{
  if(rev)
  {
    //Establishes backward direction of Channel A
    digitalWrite(12, LOW);
  }
  else
  {
    //Establishes forward direction of Channel A
    digitalWrite(12, HIGH); 
  }
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, speed);   //Spins the motor on Channel A at full speed
}  

void moveB(int speed, boolean rev)
{
  if(rev)
  {
    digitalWrite(13, LOW); //Establishes backwards direction of Channel B
  }
  else
  {
    digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  }
   
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, speed);   //Spins the motor on Channel B at full speed
}

void stopA()
{
  digitalWrite(9, HIGH);
}

void stopB()
{
  digitalWrite(8, HIGH);
}

void fullstop()
{
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}

int getUltra(int trig,int echo)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 35000);
  delayMicroseconds(100);
  distance= duration*0.034/2;
  return distance;
}

