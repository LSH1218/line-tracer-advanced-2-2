//라인트레인싱 센서
int rightLineSensor = 8; // leftLineSensor
int centerLineSensor = 9; // centerLineSensor
int leftLineSensor = 10; // rightLineSensor

//초음파 센서
int trig = 12; ///트리거 지정
int echo = 13; ///에코핀 지정


//모터 정의
#include <L298Drv.h>
L298Drv Motor0(7, 22); 
L298Drv Motor1(8, 23);  

#define THRESHOLD0 int(120)//문턱값
float duration;
float dis;


void setup() {
  pinMode(leftLineSensor, INPUT);
  pinMode(centerLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  analogReference(DEFAULT);
  Serial.begin(9600);


  // put your setup code here, to run once:

}

void loop() {
  loop1();/// 초반부
  loop2();/// 경기장 진입 전
  ssi();/// 경기장 진입 후 진행
}

void loop1() {/// 초반부
  while(true){
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 1 1 1 - 직진
      Motor0.drive(240);
      Motor1.drive(230);
    }   
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 시작 0 0 0 (라인 인식 시작)
      Motor0.drive(240);
      Motor1.drive(230);
    }
    break;
  }
  return 0;
}

void loop2() {///경기장 진입 전
  while(true){
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 라인 중앙 잡고 진행 (101)
      Motor0.drive(240);
      Motor1.drive(230);
    }
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 좌측 커브 진행 - 좌회전(011)
      Motor0.drive(0);
      Motor1.drive(220);
    }
    else if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 1 1 0 - 우회전
      Motor0.drive(220);
      Motor1.drive(0);
    }
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 0 0 0
      break;
    }
  }
  return 0;
}


void ssi() {/// 초음파센서 활성화 + 본선 씨름장
  while(true){
    digitalWrite(trig,HIGH);
    delayMicroseconds(5);///딜레이 초 지정
    digitalWrite(trig,LOW);
    duration = pulseIn(echo,HIGH, 30000UL);///pulseIn함수의 단위는 ms(마이크로 세컨드)
    dis = ((34000*duration)/1000000)/2;
    delay(100);
    
    Motor0.drive(250);// 1번 교차로~경기장 내부까지 직진
    Motor1.drive(240);
    delay(2000);
    
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 1 1 1 경기장 끝에 도달하였을 때 - 후진
      Motor0.drive(-210);
      Motor1.drive(-200);
    }
    if (dis < 7){ /// 탐지된 물체 거리가 7cm미만일 경우 - 돌진
      Motor0.drive(250);
      Motor1.drive(240);
    }
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 1 1 0 왼쪽 끝에 도달 - 직진 후 우회전
      Motor0.drive(200);
      Motor1.drive(190);
      delay(1500);
      Motor0.drive(220);
      Motor1.drive(0);
      delay(3000);
    }
    if (dis < 7){
      Motor0.drive(250);
      Motor1.drive(240);
    }
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 1 0 0 왼쪽 끝에 도달 위의 110보다 더 끝에 있는 상황 - 직진 후 우회전
      Motor0.drive(200);
      Motor1.drive(190);
      delay(1500);
      Motor0.drive(220);
      Motor1.drive(0);
      delay(4000);
    }
    if (dis < 7){
      Motor0.drive(250);
      Motor1.drive(240);
    }
    if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 0 1 1 오른쪽 끝에 도달 - 직진 후 우회전
      Motor0.drive(200);
      Motor1.drive(190);
      delay(1500);
      Motor0.drive(220);
      Motor1.drive(0);
      delay(3000);
    }
    if (dis < 7){
      Motor0.drive(250);
      Motor1.drive(240);
    }
    if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 0 0 1 오른쪽 끝에 도달 위의 011보다 더 끝에 있는 상황 - 직진 후 우회전
      Motor0.drive(200);
      Motor1.drive(190);
      delay(1500);
      Motor0.drive(220);
      Motor1.drive(0);
      delay(4000);
    }
    if (dis < 7){
      Motor0.drive(250);
      Motor1.drive(240);
    }
    if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 0 0 0 직진 후 우회전
      Motor0.drive(200);
      Motor1.drive(190);
      delay(1500);
      Motor0.drive(220);
      Motor1.drive(0);
      delay(3000);
    }
    if (dis < 7){
      Motor0.drive(250);
      Motor1.drive(250);
    }
  }
}
