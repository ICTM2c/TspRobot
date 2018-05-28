#include <Servo.h> 

int servoPin = 3; 
Servo lepel; 

const int X_Direction = 7;
const int X_Speed = 6;

const int Y_Direction = 4;
const int Y_Speed = 5;

const int LEFT = LOW;
const int RIGHT = HIGH;

const int DOWN = HIGH;
const int UP = LOW;

const int X_Sensor = A1;
const int Y_Sensor = A0;
const int blackValue = 350;
const int X_NormalSpeed = 120;
const int X_ApproachSpeed = 120;
const int Y_NormalSpeed = 255;
const int Y_ApproachSpeed = 230;

int X_Pos;
int Y_Pos;

int Y_UpDown;

void setup() {

  // motorshield
  pinMode(X_Direction, OUTPUT);
  pinMode(X_Speed, OUTPUT);
  pinMode(Y_Direction, OUTPUT);
  pinMode(Y_Speed, OUTPUT);

  //sensor
  pinMode(X_Sensor, INPUT);
  pinMode(Y_Sensor, INPUT);

  //set start position
  X_Pos = 5;
  Y_Pos = 2;

  lepel.attach(servoPin);
  lepel.write(90); 

  Serial.begin(9600);
  Serial.println("Robot is ready");

  //driveToX(2);  
}

void loop() {
  //Serial.println(analogRead(X_Sensor));
  /*Serial.print("DE X ");
  Serial.println(analogRead(X_Sensor));
  Serial.print("DE Y OKE!!!: ");
  Serial.println(analogRead(Y_Sensor));*/
  if(Serial.available() > 0){
    String X_Input = Serial.readStringUntil(',');
    Serial.read(); 
    String Y_Input = Serial.readStringUntil(',');
    Serial.read();

    int x = X_Input.toInt();
    int y = Y_Input.toInt();   

    if(x == 9 && y == 9) {
      delay(200);
      Serial.println("los");
      driveToY(2);
      driveToX(6);  
      los();
      driveToX(5);
      driveToY(2);  
    } else if(x == 8 && y == 8){
      lepel.write(90);
      
    } 
  
  if (x > 0 && x <= 6 && y > 0 && y <= 5) {
        driveToX(x);
    }
  
  //let java know we're done
  Serial.print("x arrived at ");
  Serial.println(x);

  if (y > 0 && y <= 5) {
  driveToY(y);
  }
  //let java know we're done
  Serial.print("y arrived at ");
  Serial.println(y);

  pakop();
  } 
  }


void driveToX(int X_Des){
  int X_NextDes;
  if(X_Des < X_Pos){
    digitalWrite(X_Direction, LEFT);
    X_NextDes = X_Pos - 1;
    
  } else if(X_Des > X_Pos){
    digitalWrite(X_Direction, RIGHT);
    X_NextDes = X_Pos + 1;
    
  }else if(X_Des == X_Pos){
    X_NextDes = X_Pos;
  }

  if(X_Des != X_Pos){
    // get going
    analogWrite(X_Speed, 150);
    delay(50);
    
    // slow down when approaching target destination
    if(X_NextDes == X_Des && X_NextDes != 0){
      analogWrite(X_Speed, X_ApproachSpeed);
    } else if(X_NextDes == 0){
      analogWrite(X_Speed, 95);
    }
    else{
      analogWrite(X_Speed, X_NormalSpeed);
    }
    
    // prevent detecting own line
    delay(250);
  }
  
  // drive while not detecting black
  while(analogRead(X_Sensor) < blackValue){
    //delay(10);
  }
  
  X_Pos = X_NextDes;

  if (X_Pos != X_Des) {
//  Serial.println("next");
    driveToX(X_Des);
    return;
  } else {
    // stop
    // get current direction and switch it
    digitalWrite(X_Direction, !bitRead(PORTD,X_Direction));
    analogWrite(X_Speed,60);
    delay(200);
    analogWrite(X_Speed, 0);
  }
}

void driveToY(int Y_Des){
  int Y_NextDes;
  if(Y_Des < Y_Pos){
    digitalWrite(Y_Direction, UP);
    Y_NextDes = Y_Pos - 1;
    Serial.println(analogRead(Y_Sensor));
  } else if(Y_Des > Y_Pos){
    digitalWrite(Y_Direction, DOWN);
    Serial.println(analogRead(Y_Sensor));
    Serial.println(Y_Pos);
    Y_NextDes = Y_Pos + 1;
  }else if(Y_Des == Y_Pos){
    Y_NextDes = Y_Pos;
  }

  if(Y_Des != Y_Pos){
    // get going
    analogWrite(Y_Speed, 120);
    delay(50);
    
    // slow down when approaching target destination
    if(Y_NextDes == Y_Des && Y_NextDes != 0){
      if(Y_NextDes >= 2) {
        analogWrite(Y_Speed, Y_Des > Y_Pos ? Y_ApproachSpeed : (30));
        Serial.println("tokkie");
      } else if(Y_NextDes == 1){
        analogWrite(Y_Speed, Y_Des > Y_Pos ? Y_ApproachSpeed : (70));
        Serial.println("speedy");
      } else {
        analogWrite(Y_Speed, Y_Des > Y_Pos ? Y_ApproachSpeed : (18));
        Serial.println("slow poke");
      }
    } else if(Y_NextDes == 0){
      analogWrite(Y_Speed, 50);
    } else if(Y_Pos >= 4){
      analogWrite(Y_Speed, 70);
    }
    else{
      if(Y_Des < Y_Pos) {
        analogWrite(Y_Speed, 40);
      } else {   
        analogWrite(Y_Speed, Y_NormalSpeed);
      }
    }
    
    // prevent detecting own line
    delay(200);
  }
  
  // drive while not detecting black
  while(analogRead(Y_Sensor) < blackValue){
    //delay(10);
  }
  
  Y_Pos = Y_NextDes;

  if (Y_Pos != Y_Des) {
//  Serial.println("next");
    driveToY(Y_Des);
    return;
  } else {
    // stop
    // get current direction and switch it
   // digitalWrite(Y_Direction, !bitRead(PORTD,Y_Direction));
    //analogWrite(Y_Speed,60);
    //delay(200);
    analogWrite(Y_Speed, 0);
  }
}

void pakop() {
  pakop_omhoog();
  delay(600);
  lepel.write(0); 
  delay(1000); 
  omhoog();
  delay(200);
  stop_kraan();
  delay(400);
  lepel.write(90);
  delay(400);

  while(analogRead(Y_Sensor) < blackValue) {
    if(Y_Pos == 5){
      omlaag();
      Serial.println(Y_Pos);
      Serial.println("doet ding");
    } else if (Y_Pos >= 3) {
      minder_omlaag();
      Serial.println(Y_Pos);
      Serial.println("doet iets anders oke"); 
    } else {
      sloom_omlaag();
      Serial.println(Y_Pos);
      Serial.println("doet iets anders");
    }
    Serial.println("its white");
  }
  pakop_omhoog(); 
  Serial.println("its black");
  delay(200);
  Serial.print("done");
}

void los() {
  delay(500);
  lepel.write(0);
  delay(400); 
  omlaag_kraan();
  delay(250);
  stop_kraan();
  delay(400);
  lepel.write(70);
  Serial.print("box");
  delay(3000);
  lepel.write(130);
  Serial.print("box");
  delay(3000);
  lepel.write(180);
  Serial.print("box");
  delay(3000);
  lepel.write(90);
  delay(2000);
  omhoog();
  delay(300);
  stop_kraan();
  delay(200);
}

void omhoog() {
  digitalWrite(Y_Direction,HIGH);        
  analogWrite(Y_Speed, 255);
}

void pakop_omhoog() {
  digitalWrite(Y_Direction,HIGH);        
  analogWrite(Y_Speed, 1);
}

void omlaag() {
  digitalWrite(Y_Direction,LOW);        
  analogWrite(Y_Speed, 80);
}

void omlaag_kraan() {
  digitalWrite(Y_Direction,LOW);        
  analogWrite(Y_Speed, 100);
}

void minder_omlaag() {
  digitalWrite(Y_Direction,LOW);        
  analogWrite(Y_Speed, 50);
}

void sloom_omlaag() {
  digitalWrite(Y_Direction,LOW);        
  analogWrite(Y_Speed, 40);
}

void stop_kraan() {
  digitalWrite(Y_Direction,LOW);
  analogWrite(Y_Speed, 0);
}
