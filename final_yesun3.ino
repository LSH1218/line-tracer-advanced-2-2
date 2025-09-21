//라인트레인싱 센서
int rightLineSensor = 8; /// leftLineSensor
int centerLineSensor = 9; /// centerLineSensor
int leftLineSensor = 10; /// rightLineSensor

//모터 정의
#include <L298Drv.h>
L298Drv Motor0(7, 22); 
L298Drv Motor1(8, 23);  

#define THRESHOLD0 int(120)//문턱값

void setup() {
  pinMode(leftLineSensor, INPUT); ///leftLineSensor INPUT 설정
  pinMode(centerLineSensor, INPUT); ///centerLineSensor INPUT 설정
  pinMode(rightLineSensor, INPUT); ///rightLineSensor INPUT 설정
  analogReference(DEFAULT);
  Serial.begin(9600);


  // put your setup code here, to run once:

}

void loop() {
  loop1();/// 초반부
  loop2();/// 경기장 진입 전
  loop3();/// 경기장 진입 후 진행
  loop4();/// 경기장 탈출 후 진행
}

///왼쪽 모터가 조금더 빠르므로 모터값을 오른쪽-10의 값으로 설정
void loop1() {/// 초반부
  while(true){
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 1 1 1 박스안 라인인식 전 - 직진
      Motor0.drive(240);
      Motor1.drive(230);
    }   
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 라인 인식 시작 0 0 0 - 직진
      Motor0.drive(240);
      Motor1.drive(230);
    }
    break;
  }
  return 0;
}

void loop2() { ///경기장 진입 전 라인트레이싱
  while(true){
    if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 라인 중앙 잡고 진행 (101) -직진
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
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 0 0 0 - 1번 교차로 인식
      break;
    }
  }
  return 0;
}

void loop3(){ ///경기장 진입 후 진행(라인트레이싱을 최소화시키고 딜레이로 진행)
  while(true){
    Motor0.drive(240);///직진
    Motor1.drive(230);
    delay(2500); /// 교차로~경기장 내부까지 직진
    Motor0.drive(220);///우회전
    Motor1.drive(0);
    delay(530);///
    Motor0.drive(240);///직진
    Motor1.drive(230);
    delay(300);///방향 전환 후 경기장 외곽까지 직진
    while(analogRead(leftLineSensor) < THRESHOLD0 ){
      Motor0.drive(240);/// 직진
      Motor1.drive(230);
    } 
    Motor0.drive(240);///좌회전
    Motor1.drive(0);
    delay(700);    
    while(analogRead(rightLineSensor) < THRESHOLD0 ){
      Motor0.drive(250);///원 커브
      Motor1.drive(150);
    }
    Motor0.drive(240);/// 직진
    Motor1.drive(230);   
    delay(200);
    Motor0.drive(-250);///90도 회전
    Motor1.drive(250);
    delay(700);
    Motor0.drive(240); ///경기장 탈출 - 직진
    Motor1.drive(230);
    delay(800);
    break;
  }
  return 0;
}

void loop4() { /// 경기장 이후
  while(true){
    if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 0 0 0 - 2번 교차로 인식
      Motor0.drive(180);
      Motor1.drive(170);
    }
     else if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 1 0 1 - 다시 라인트레이싱
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
    else if (analogRead(rightLineSensor)<THRESHOLD0 && analogRead(centerLineSensor)<THRESHOLD0 && analogRead(leftLineSensor)<THRESHOLD0){ /// 0 0 0 - 도착지점 인식
      Motor0.drive(100);
      Motor1.drive(90);
    }
    else if (analogRead(rightLineSensor)>THRESHOLD0 && analogRead(centerLineSensor)>THRESHOLD0 && analogRead(leftLineSensor)>THRESHOLD0){ /// 1 1 1 - 도착지점 정지
      Motor0.drive(0);
      Motor1.drive(0);
    }
    break;
  }
  return 0;
}     
