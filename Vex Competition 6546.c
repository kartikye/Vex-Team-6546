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

//Stop Motors

void stopDriveMotors(){
	motor[port10] = 0;
	motor[port9]  = 0;
	motor[port1]  = 0;
	motor[port2]  = 0;
}

void stopArmMotors(){
	motor[port3]  = 0;
	motor[port4]  = 0;
}

void stopPickerUpper(){
	motor[port5] = 0;
}

//Robot Movement

void moveForward(int t, int speed){
	motor[port10] = speed;
	motor[port9]  = speed;
	motor[port1]  = speed;
	motor[port2]  = speed;

	wait1Msec(t);

	stopDriveMotors();
}

void moveBackward(int t, int speed){
	motor[port10] = -1*speed;
	motor[port9]  = -1*speed;
	motor[port1]  = -1*speed;
	motor[port2]  = -1*speed;

	wait1Msec(t);

	stopDriveMotors();
}

void moveRightMotors(int speed, int limit){
	motor[port10] = speed*limit;
	motor[port9] = speed*limit;
}

void moveLeftMotors(int speed, int limit){
	motor[port1] = speed*limit;
	motor[port2] = speed*limit;
}

//Arm Movement

void moveArmDown(){
	motor[port3] =  127;
	motor[port4] = -127;
}

void moveArmUp(){
	motor[port3] = -127;
	motor[port4] =  127;
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

void pickerUpperIn(){
	motor[port5] = -127;
}

void pickerUpperOut(){
	motor[port5] =  127;
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
