#include <AccelStepper.h>
#include <Servo.h>

#define step1 2
#define step2 3
#define step3 4
#define step4 5
#define photoone analogRead(0)
#define phototwo analogRead(1)
#define photothree analogRead(2)
#define photofour analogRead(3)

void moveStepper(AccelStepper);
int calcErrorH();
int calcErrorV();
void moveServos(int);
void getPhotoData();

Servo servo_one;
Servo servo_two;
AccelStepper stepper(AccelStepper::HALF4WIRE, step1, step2, step3, step4);
int photo[4];
int photo_offset[4];
int top_avg,bot_avg,lef_avg,rig_avg,servo_pos = 90;

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(100);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(50);
  stepper.setSpeed(500);
  stepper.moveTo(2048); // ?????
  servo_one.attach(9);
  servo_two.attach(10);
  servo_one.write(servo_pos);
  servo_two.write(servo_pos);
  Serial.begin(9600);
}

void loop() {
  getPhotoData();
  if(servo_pos == 135) {
servo_pos -= 1;
  }
  else if(servo_pos == 45) {
    servo_pos += 1;
  }
  else if(bot_avg > top_avg) {
    servo_pos -= 1;
  }
  else if (top_avg > bot_avg) {
    servo_pos += 1;
  }
  moveServos(servo_pos);
  stepper.setSpeed(calcErrorH());
  stepper.runSpeed();
  Serial.println(calcErrorH());

  delay(40);

}

void moveStepper(AccelStepper stepper) {
  stepper.setSpeed(stepper.maxSpeed()*calcErrorH()/500);
}

int calcErrorH() {
  return (lef_avg - rig_avg)*(1);

}
int calcErrorV() {return (abs(top_avg - bot_avg));}

void moveServos(int deg) {
  servo_one.write(180-deg);
  servo_two.write(deg);

}

void getPhotoData() {
  //blue wire/mark === A0
  for(int i=0; i<4;i++) {
      photo[i] = analogRead(i);
  }
//Top avg
  top_avg = (photo[0] + photo[3])/2;
//Bottom avg
  bot_avg = ((photo[1]+photo[2])/2);
//Left avg
  lef_avg = (photo[0]+photo[1])/2;
//Right avg
  rig_avg = (photo[2]+photo[3])/2;
}