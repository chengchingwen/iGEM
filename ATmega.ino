#include <dht.h>
#include <Wire.h>
#include <MCP23008.h>
#include <Bridge.h>
char pin13[2];
char cmd[2];
int MCP23008_ADDROne =  0x20 ;
int MCP23008_ADDRTwo = 0x22 ;
int MCP23008_ADDRThree = 0x24 ;


const byte A_1 = 14;  //tem humidity sensor analog pin
//const byte A_2 = 23;  //water sensor analog pin 
const byte IN1 = 5;    // 馬達A 的正反轉接腳編號
const byte IN2 = 7;
const byte ENA = 6;   // 馬達A 的 PWM 轉速控制
const byte A = 18;  //mud sensor
//const byte LED = 13; 

/*腳位:D1 A1 GND 5V*/
const byte mqPin3 = 20;
const byte mqPin2 = 19;
const byte uvPin  = 23;
const byte trigPin = 11;
const byte echoPin = 12;
const byte watPin = 16;

class WatLevel{
 public:
  WatLevel(byte a, byte b ):trigPin(a), echoPin(b){
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }
  float read(){
    float duration, distance;
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    return distance;
  }  
 private:
    byte trigPin;
    byte echoPin;
};
 

class UVsen{
 public:
 UVsen(byte a):pin(a){
  }
  int read(){
    value = analogRead(pin);
    return value;
  }
 private:
  byte pin;
  int value = -1;
};

class Mq3{
 public:
 Mq3(byte a):pin(a){
  }
  int read(){
    value = analogRead(pin);
    return value;
  }
 private:
  byte pin;
  int value = -1;
};



class Mq2{
 public:
 Mq2(byte a):pin(a){
  }
  int read(){
    value = analogRead(pin);
    return value;
  }
 private:
  byte pin;
  int value = -1;
};

class HumSen{
 public:
  struct pair{
    int hum;
    int tem;
  };
 HumSen(byte a):pin(a){
  }
  struct pair read(){
    struct pair a;
    DHT.read11(pin);   
    a.hum = DHT.humidity;
    a.tem = DHT.temperature;
    return a;
  } 
 private:
  dht DHT;
  byte pin;
};


class Sprinklers{
 public:
 Sprinklers(byte a, byte b, byte c, byte d):direct(a), in2(b), pwm(c), Read(d){
    pinMode(Read, INPUT);
    pinMode(direct,OUTPUT);
    pinMode(in2,OUTPUT);
    pinMode(pwm,OUTPUT);
  }
  int read(){
    s = analogRead(Read); //take a sample
    return s;
  }
  void spray(int cmd){
    if(cmd) { //(s < 85 && s >= 60)
      digitalWrite(direct, HIGH);
      digitalWrite(in2, HIGH);
      digitalWrite(pwm, HIGH);
    }
    else{
      digitalWrite(direct,LOW);
      digitalWrite(in2, LOW);
      digitalWrite(pwm, LOW);
    }
  }
 private:
  byte Read;
  byte direct;
  byte in2;
  byte pwm;
  int s=0;
};

class IR_recever{
public:
  IR_recever(){
  }  
  void setup() {
    Serial.begin(9600);
  }
  int read() {
    if (Serial.available() > 0) {
      int mothNum = Serial.read();
      return mothNum;
    }
  }
private:
  int mothNum = 0;
};

HumSen myhumsen(A_1);
Sprinklers mysprink(IN1, IN2, ENA, A);
Mq3 mymq3(mqPin3);
Mq2 mymq2(mqPin2);
UVsen myuvsen(uvPin);
WatLevel mywat(trigPin, echoPin);
IR_recever myir;

String str(int a){
  return String("") + a;
}
String str(float a){
  return String("") + a;
}

void setup() {
  Bridge.begin();
  Wire.begin();
  myir.setup();
  delay(1500);
}

void loop() {
  Bridge.get("Reg_done", pin13, 2);
  digitalWrite(LED, atoi(pin13));
  Bridge.get("cmd", cmd, 2);
  mysprink.spray(atoi(cmd));
  int mud = mysprink.read();
  int hum = myhumsen.read().hum;
  int tem = myhumsen.read().tem;  
  int co = mymq2.read();
  int co2 = mymq3.read();
  int uv = myuvsen.read();
  float wad = mywat.read();
  int ir = myir.read();
  Bridge.put("mud", str(mud));
  Bridge.put("hum", str(hum));
  Bridge.put("tem", str(tem));
  Bridge.put("co", str(co));
  Bridge.put("co2", str(co2));
  Bridge.put("uv", str(uv));
  Bridge.put("wad", str(wad));
  Bridge.put("ir", str(ir));
  delay(1000);
}
