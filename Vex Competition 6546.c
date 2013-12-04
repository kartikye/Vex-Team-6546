#pragma config(Sensor, dgtl8,  q1,             sensorQuadEncoder)
#pragma platform(VEX)
//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"

//    ______   .__   __.  _______
//   /  __  \  |  \ |  | |   ____|
//  |  |  |  | |   \|  | |  |__
//  |  |  |  | |  . `  | |   __|
//  |  `--'  | |  |\   | |  |____
//   \______/  |__| \__| |_______|
//
//   _______   _______   _______ .______       _______  _______
//  |       \ |   ____| /  _____||   _  \     |   ____||   ____|
//  |  .--.  ||  |__   |  |  __  |  |_)  |    |  |__   |  |__
//  |  |  |  ||   __|  |  | |_ | |      /     |   __|  |   __|
//  |  '--'  ||  |____ |  |__| | |  |\  \----.|  |____ |  |____
//  |_______/ |_______| \______| | _| `._____||_______||_______|
//
//  .__   __.   ______   .______     .___________. __    __
//  |  \ |  |  /  __  \  |   _  \    |           ||  |  |  |
//  |   \|  | |  |  |  | |  |_)  |   `---|  |----`|  |__|  |
//  |  . `  | |  |  |  | |      /        |  |     |   __   |
//  |  |\   | |  `--'  | |  |\  \----.   |  |     |  |  |  |
//  |__| \__|  \______/  | _| `._____|   |__|     |__|  |__|
//
//      _                                                       _                       _             _
//   __(_)_ _  __ _ __ _ _ __  ___ _ _ ___   __ _ _ __  ___ _ _(_)__ __ _ _ _    ___ __| |_  ___  ___| |
//  (_-< | ' \/ _` / _` | '_ \/ _ \ '_/ -_) / _` | '  \/ -_) '_| / _/ _` | ' \  (_-</ _| ' \/ _ \/ _ \ |
//  /__/_|_||_\__, \__,_| .__/\___/_| \___| \__,_|_|_|_\___|_| |_\__\__,_|_||_| /__/\__|_||_\___/\___/_|
//            |___/     |_|
//
//     _/                                                _/_/_/  _/_/_/_/  _/  _/      _/_/_/
//  _/_/_/_/    _/_/      _/_/_/  _/_/_/  _/_/        _/        _/        _/  _/    _/
//   _/      _/_/_/_/  _/    _/  _/    _/    _/      _/_/_/    _/_/_/    _/_/_/_/  _/_/_/
//  _/      _/        _/    _/  _/    _/    _/      _/    _/        _/      _/    _/    _/
//   _/_/    _/_/_/    _/_/_/  _/    _/    _/        _/_/    _/_/_/        _/      _/_/





/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
//                               Pre-Autonomous Functions                                      //                                                          //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  bStopTasksBetweenModes = true;

  //Setup Encoder
  SensorValue[q1] = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//                                      Functions                                            //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

//Robot Movement

void moveRightMotors(int speed, int limit){
	motor[port10] = speed*limit;
	motor[port9] = speed*limit;
}

void moveLeftMotors(int speed, int limit){
	motor[port1] = speed*limit;
	motor[port2] = speed*limit;
}

void stopDriveMotors(){
	moveLeftMotors(0,0);
	moveRightMotors(0,0);
}

void moveForward(int t, int speed){
	moveLeftMotors(speed,1);
	moveRightMotors(speed,1);

	wait1Msec(t);

	stopDriveMotors();
}

void moveBackward(int t, int speed){
	moveLeftMotors(-1*speed,1);
	moveRightMotors(-1*speed,1);

	wait1Msec(t);

	stopDriveMotors();
}

//Arm Movement

void setArmSpeed(int right, int left){
	motor[port3] = left;
	motor[port4] = right;
}

void stopArmMotors(){
	setArmSpeed(0,0);
}

void moveArmDown(){
	setArmSpeed(-127,127);
}

void moveArmUp(){
	setArmSpeed(127,-127);
}

void moveArmToPosition(int p){
	if(p > SensorValue[q1]){
		while(p != SensorValue[q1]){
			moveArmUp();
		}
		stopArmMotors();
	}else if(p < SensorValue[q1]){
		while(p != SensorValue[q1]){
			moveArmDown();
		}
		stopArmMotors();
	}else{
		stopArmMotors();
	}
}

//Picker Upper Movement

void setPickerUpperSpeed(int speed){
	motor[port5] = speed;
}

void stopPickerUpper(){
	setPickerUpperSpeed(0);
}

void pickerUpperIn(){
	setPickerUpperSpeed(-127);
}

void pickerUpperOut(){
	setPickerUpperSpeed(127);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
//                                      Autonomous Task                                        //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

task autonomous(){

	//Raise Arm to Starting Position
  moveArmToPosition(60);

	//Move to First Ball
	moveForward(1300, 100);

	wait1Msec(500);

	//Move Back to Start Tile
	moveBackward(1300, 100);

	//Wait for 3 secs While Driver Turns Bot
	wait1Msec(3000);

	//Move to Second Ball
	moveForward(1700, 100);

	wait1Msec(500);

	//Move Back to Start Tile
	moveBackward(1700, 100);

	//Bring Arm Down
	moveArmToPosition(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                             //
//                                     User Control Task                                       //
//                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////

task usercontrol(){
	while (true){
	  if(SensorValue[q1] > 65 && ( vexRT[Ch3] > 0 || vexRT[Ch2] > 0)){
	    moveRightMotors(vexRT[Ch3],0.5);
	    moveLeftMotors(vexRT[Ch2],0.5);
	  }else{
	    moveRightMotors(vexRT[Ch3],1);
	    moveLeftMotors(vexRT[Ch2],1);
	  }

	  //Arm
	  if(vexRT[Btn6D] == 1 && SensorValue[q1] > 0){
	  	moveArmDown();
	  }else if(vexRT[Btn6U] == 1){
	    moveArmUp();
	  }else{
	    stopArmMotors();
	  }
	  //Picker Upper
	  if(vexRT[Btn5U] == 1){
	    pickerUpperIn();
	  }else if(vexRT[Btn5D] == 1){
	    pickerUpperOut();
	  }else{
	    stopPickerUpper();
	  }
	}
}
