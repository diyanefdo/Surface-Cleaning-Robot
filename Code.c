
//Written by Kathan Shah
int menu()
{
	int choice = 0;
	while (!getButtonPress(buttonEnter))
	{
		if (choice == 0)
		{
			displayInverseString(5, "Rectangle");
			displayString(6, "Circle");
			displayString(7, "Other");
		}
		if (choice == 1)
		{
			displayString(5, "Rectangle");
			displayInverseString(6, "Circle");
			displayString(7, "Other");
		}
		if (choice == 2)
		{
			displayString(5, "Rectangle");
			displayString(6, "Circle");
			displayInverseString(7, "Other");
		}
		while (!getButtonPress(buttonAny))
		{}
		if (getButtonPress(buttonUp))
		{
			while (getButtonPress(buttonUp))
			{}
			choice--;
			if (choice < 0)
				choice = 2;
		}
		if (getButtonPress(buttonDown))
		{
			while (getButtonPress(buttonDown))
			{}
			choice++;
			if (choice > 2)
				choice = 0;
		}
		displayString(8, "%d", choice);
	}
	eraseDisplay();
	return choice;
}

//Written by all
void redirectSafe(int portNum1, int portNum2)
{
	motor[motorA] = motor[motorC] = 0;
	//if the touch sensor(s) are not pressed redirect back onto table
	if(SensorValue[portNum1] == 0)
	{
		motor[motorA] = 15;
		motor[motorC] = -15;
		while(SensorValue[portNum1] != 1)
		{}
		motor[motorA] = motor[motorD] = 0;
		wait1Msec(50);
		motor[motorA] = motor[motorD] = -15;
	}
	else if(SensorValue[portNum2] == 0)
	{
		motor[motorA] = 15;
		motor[motorC] = -15;
		while(SensorValue[portNum2] != 1)
		{}
		motor[motorA] = motor[motorC] = 0;
		wait1Msec(50);
		motor[motorA] = motor[motorC] = -15;
	}
}

//written by Mark Hannah
void turnAroundGyro(int Gyro)
{
	//turns the robot 180 degrees
	resetGyro(Gyro);
	wait1Msec(100);
	while (abs(getGyroDegrees(Gyro)) < 180)
	{
		motor[motorA] = 25;
		motor[motorC] = -25;
	}
	motor[motorA] = motor[motorC] = 0;
}


//written by Mark Hannah
void turn90Gyro(int Gyro, bool direction)
{
	//turns the robot 90 degrees
	resetGyro(Gyro);
	wait1Msec(100);
	int direct = 1;
	if (direction == false)
	{
		direct = -1;
	}
	while (abs(getGyroDegrees(Gyro)) < 90)
	{
		motor[motorA] = direct * 25;
		motor[motorC] = direct * -25;
	}

	motor[motorA] = motor[motorC] = 0;
}

void spray_refill(int uSonic)
{
	// the exact sensor value at which fluid should be refilled should be tested

	motor[motorA] = motor[motorC] = 0;
	displayString(0,"Please refill the cleaning fluid");
	displayString(1,"Press the left button to stop beeping");
	while(!getButtonPress(buttonLeft))
	{
		playSound(soundBeepBeep);
	}
	//clearAll;
	displayString(0,"Continuing to clean...");
}

void liqSpray(int & sprayNumber, int uSonic)
{
	motor[motorB] = 20;
	wait1Msec(200);
	motor[motorB] = -20;
	wait1Msec(200);
	motor[motorB] = 0;
	sprayNumber++;
	if (sprayNumber >= 35) //Number of sprays calculated to complete a 100 mL bottle with cleaning solution
	{
		spray_refill(uSonic);
		sprayNumber = 0;
	}
}

//Diyane Fernando
float evade_distance ( int arc_angle)
{
	//calculates the distance to travel around the obstacle
	const float half_width = 11.75;
	float angle = (2.75*( arc_angle*PI/180))/half_width;
	return ((30/cos(angle))/(2*PI*2.75))*360;
}

//Diyane Fernando
void evade_obstacles(int port_ultra, int touch_left, int touch_right, int Gyro)
{
	if (SensorValue[port_ultra] < 15)
	{
		motor[motorA] = motor[motorC] = 0;
		//resets values neccessary to turn the robot around obstacle
		resetGyro(S1);
		motor[motorA] = motor[motorC] = 0;
		wait1Msec(300);
		nMotorEncoder[motorA]=0;
		motor[motorA]= -15;
		motor[motorC]= 15;
		while(SensorValue[port_ultra] < 30)
		{}
		wait1Msec(300);

		int turn = abs(nMotorEncoder[motorA]);
		motor[motorA] = motor[motorC] = 0;
		nMotorEncoder[motorA] = 0;
		motor[motorA] = motor[motorC] = -20;
		clearTimer(T1);
		while(abs(nMotorEncoder[motorA]) < evade_distance(turn) && (SensorValue[touch_right] == 1))
		{}

		int time_obs = time1[T1];
		if(SensorValue[touch_right] == 0)
		{
			clearTimer(T1);
			motor[motorA]= motor[motorC] = 20;
			while(time1[T1]<time_obs)
			{}
			motor[motorA]= motor[motorC] = 0;
			nMotorEncoder[motorA]=0;
			motor[motorA] = 15;
			motor[motorC] = -15;
			while(abs(nMotorEncoder[motorA]) < turn)
			{}
			motor[motorA] = motor[motorB] = 0;
			wait1Msec(200);
			nMotorEncoder[motorA]=0;
			while(SensorValue[port_ultra] < 30)
			{
				motor[motorA]= 15;
				motor[motorC]= -15;
			}
			wait1Msec(300);
			int turn = abs(nMotorEncoder[motorA]);

			motor[motorA] = motor[motorC] = 0;
			nMotorEncoder[motorA] = 0;
			motor[motorA] = motor[motorC] = -20;
			while(abs(nMotorEncoder[motorA]) < evade_distance(turn))
			{}
			motor[motorA]= motor[motorC] = 0;
			nMotorEncoder[motorA]=0;
			motor[motorA]= -15;
			motor[motorC]= 15;

			while(	abs(nMotorEncoder[motorA])<turn)
			{}
			motor[motorA]=motor[motorC]=0;
			wait1Msec(300);
			motor[motorA] = motor[motorC] = -25;
			wait1Msec(1200);
			motor[motorA]=motor[motorC]=0;
			wait1Msec(300);
			nMotorEncoder[motorA]=0;
			motor[motorA]= -15;
			motor[motorC]= 15;
			while(abs(nMotorEncoder[motorA])<turn)
			{}
			motor[motorA]=motor[motorC]=0;
			nMotorEncoder[motorA]=0;
			wait1Msec(300);
			motor[motorA] = motor[motorC] = -20;
			while(abs(nMotorEncoder[motorA]) < evade_distance(turn))
			{}
			motor[motorA] = motor[motorC] = 0;
			wait1Msec(300);
			nMotorEncoder[motorC]=0;
			motor[motorA] = 15;
			motor[motorC] = -15;
			while(abs(nMotorEncoder[motorC])<turn)
			{}
			motor[motorA] = motor[motorC] = 0;
		}
		//	--------------------------------------------------
		// if a obstacle is in the middle of a circular table, use motor ratios to make the robot move in an
		// arc to evade the object
		else
		{
			motor[motorA]= motor[motorC] = 0;
			nMotorEncoder[motorA]=0;
			motor[motorA]= 15;
			motor[motorC]= -15;

			while(abs(nMotorEncoder[motorA])<turn)
			{}
			motor[motorA]=motor[motorC]=0;
			wait1Msec(300);
			motor[motorA] = motor[motorC] = -25;
			wait1Msec(2000);
			motor[motorA]=motor[motorC]=0;
			wait1Msec(300);
			nMotorEncoder[motorA]=0;
			motor[motorA]= 15;
			motor[motorC]= -15;
			while(abs(nMotorEncoder[motorA])<turn)
			{}
			motor[motorA]=motor[motorC]=0;
			nMotorEncoder[motorA]=0;
			wait1Msec(300);
			motor[motorA] = motor[motorC] = -25;
			while(abs(nMotorEncoder[motorA]) < evade_distance(turn))
			{}
			motor[motorA] = motor[motorC] = 0;
			wait1Msec(300);
			nMotorEncoder[motorC]=0;
			motor[motorA] = -15;
			motor[motorC] = 15;
			while(abs(nMotorEncoder[motorC])<turn)
			{}
			motor[motorA] = motor[motorC] = 0;
		}
		turnAroundGyro(Gyro);
		nMotorEncoder[motorA] = 0;
		motor[motorA] = motor[motorC] = -25;
		while (SensorValue[port_ultra] >= 15)
		{}
		int encoder = abs(nMotorEncoder[motorA]);
		motor[motorA] = motor[motorC] = 25;
		nMotorEncoder[motorA] = 0;
		while (abs(nMotorEncoder[motorA]) <= encoder)
		{}
		turnAroundGyro(Gyro);
	}
}

//Mark Hannah
void adjust(int touch1, int touch2, int uSonic, int gyro)
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorC]= -25;
	while(SensorValue[touch1] == 1 || SensorValue[touch2] == 1)
	{}
	wait1Msec(100);
	if (SensorValue[touch1] == 0 && SensorValue[touch2] == 0)
		motor[motorA] = motor[motorC]= 10;
	nMotorEncoder[motorA] = 0;
	const int DISTANCETOBACKUP = 9 / (2 * PI * 2.75) * 360;
	while (nMotorEncoder[motorA] <= DISTANCETOBACKUP)
	{}
	motor[motorA] = motor[motorC]= 50;
	while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
	{}
	motor[motorA] = motor[motorC] = 0;
	wait1Msec(1000);
	motor[motorA] = motor[motorC]= 10;
	nMotorEncoder[motorA] = 0;
	const int DISTANCETOBACKUP = 9 / (2 * PI * 2.75) * 360;
	while (nMotorEncoder[motorA] <= DISTANCETOBACKUP)
	{}
	motor[motorA] = motor[motorC]= 50;
	while (nMotorEncoder[motorA] <= DISTANCETOBACKUP)
	{}
	if ((SensorValue[touch1] ==0 && SensorValue[touch2] == 1 ))
	{
	motor[motorC] = 0;
	motor[motorA] = -15;
	while (SensorValue[touch2] ==1)
	{}
	}

	else if (SensorValue[touch1] == 1 && SensorValue[touch2] == 0)
	{
	motor[motorA]= 0;
	motor[motorC] = -15;
	while (SensorValue[touch1] == 1)
	{}
	}
	motor[motorA] = motor[motorC]= 0;
}

//Mark Hanna
void getRectDimensions(float & length, float & width, int touch1, int touch2, int uSonic, int gyro, int & sprayNumber)
{
	turnAroundGyro(gyro);
	wait1Msec(5000);
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorC] = -25;
	while (SensorValue[touch1]== 1 && SensorValue[touch2]== 1)
	{
		evade_obstacles(touch1, touch2, uSonic, gyro);
	}
	motor[motorA] = motor[motorC]= 50;
	while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
	{}

	motor[motorA] = motor[motorC] = 0;
	wait1Msec(2000);
	int encoderValue = nMotorEncoder[motorA];
	length = abs(nMotorEncoder[motorA])*(2 * PI * 2.75)/360;
	displayString(6,"%f", length);
	wait1Msec(3000);
	// get the length after it adjusts itself
	turn90Gyro(gyro, false);
	motor[motorA]=motor[motorC] = -25;
	while (SensorValue [touch1]==1 && SensorValue [touch2]==1)
	{
		evade_obstacles(touch1, touch2, uSonic, gyro);
		redirectSafe(touch1, touch2);
		liqSpray(sprayNumber, uSonic);
	}
	motor[motorA] = motor[motorC]= 50;
	while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
	{}
	motor[motorA]=motor[motorC] = 0;
	// goes to the corner
	turnAroundGyro(gyro);

	nMotorEncoder[motorA]= 0;
	motor[motorA]=motor[motorC] = -25;
	while (SensorValue [touch1]==1 && SensorValue [touch2]==1)
	{
		evade_obstacles(touch1, touch2, uSonic, gyro);
		redirectSafe(touch1, touch2);
	}
	motor[motorA] = motor[motorC]= 50;
	while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
	{}
	motor[motorA]=motor[motorC] = 0;
	wait1Msec(2000);
	encoderValue = nMotorEncoder[motorA];
	width = abs(nMotorEncoder[motorA])*(2 * PI * 2.75)/360;
	displayString(6,"%f", width);
	wait1Msec(3000);
	// gets the width.

	for (int x = -1; x <(length/(23 * 2)); x++)
	{

		turn90Gyro(gyro, true);
		nMotorEncoder[motorA] = 0;
		motor[motorA] = motor[motorC] = -25;
		while (abs(nMotorEncoder[motorA]) <= 11.6 * 360 / (2 * PI * 2.75) && SensorValue [touch1]==1 && SensorValue [touch2]==1)
		{}
		motor[motorA] = motor[motorC]= 50;
		while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
		{}
		motor[motorA] = motor[motorC] = 0;
		turn90Gyro(gyro, true);
		nMotorEncoder[motorA]= 0;
		motor[motorA]=motor[motorC] = -25;
		while (	abs(nMotorEncoder[motorA]) < width * 360/(2 * 2.75 * PI) && SensorValue [touch1]==1 && SensorValue [touch2]==1)
		{}
		motor[motorA] = motor[motorC]= 50;
		while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
		{}
		motor[motorA]=motor[motorC] = 0;

		turn90Gyro(gyro, false);
		nMotorEncoder[motorA] = 0;
		motor[motorA] = motor[motorC] = -25;
		while (abs(nMotorEncoder[motorA]) <= 11.6 * 360 / (2 * PI * 2.75) && SensorValue [touch1]==1 && SensorValue [touch2]==1)
		{}
		motor[motorA] = motor[motorC]= 50;
		while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
		{}
		motor[motorA] = motor[motorC] = 0;
		turn90Gyro(gyro, false);
		nMotorEncoder[motorA]= 0;
		motor[motorA]=motor[motorC] = -25;
		while (	abs(nMotorEncoder[motorA]) < width * 360/(2 * 2.75 * PI) && SensorValue [touch1]==1 && SensorValue [touch2]==1)
		{}
		motor[motorA] = motor[motorC]= 50;
		while (SensorValue[touch1] == 0 || SensorValue[touch2] == 0)
		{}
		motor[motorA]=motor[motorC] = 0;
	}
}

//written by Tristan Curry
void followCircum(float Dia, int touch1, int touch2, int gyro)
{
	const int robotW = 23;
	float circum = 0;
	circum = PI * Dia;
	motor[motorC] = -15;
	for(int ratio = (10 * ((Dia + robotW)/Dia)); ratio > 2; ratio -=2)
	{
		displayString(6, "ratio: %f", ratio);
		motor[motorA] = -ratio;
		nMotorEncoder[motorA] = 0;
		while(abs(nMotorEncoder[motorA]) <= abs(circum * (360/(11 * PI))))
		{
			float test = abs(circum * (360/(11 * PI)));
			displayString(7,"%d", nMotorEncoder[motorA]);
			displayString(5, "%f", test);
		}
		nMotorEncoder[motorA] = 0;
		/liqSpray(Dia, S3);
	}
	//scrubs the centre of the table once the robot has reached it
	for (int repetition = 0; repetition < 3; repetition++)
	{
		motor[motorA] = motor[motorC] = 25;
		wait1Msec(1000);
		motor[motorA] = motor[motorC] = -25;
		wait1Msec(1000);
	}
	motor[motorA] = motor[motorC] = 0;
	playSound(1);
}

//Written by Kathan Shah
int sameTable(string table,float length, float width, float diameter)
{
	int choice = 0;
	while (!getButtonPress(buttonEnter))
	{
		//displays the menu on the display
		if (choice == 0)
		{
			if (table == "Rectangle")
				displayInverseString(5, "Yes (L: %d W: %d)", length, width);
			else if (table == "Circle")
				displayInverseString(5, "Yes (D: %d)", diameter);
			else if (table == "Odd")
				displayInverseString(5, "Yes");
			displayString(6, "No");
		}
		if (choice == 1)
		{
			if (table == "Rectangle")
				displayString(5, "Yes (L: %d W: %d)", length, width);
			else if (table == "Circle")
				displayString(5, "Yes (D: %d)", diameter);
			else if (table == "Odd")
				displayString(5, "Yes");
			displayInverseString(6, "No");
		}
		while (!getButtonPress(buttonAny))
		{}
		if (getButtonPress(buttonUp))
		{
			while (getButtonPress(buttonUp))
			{}
			choice--;
			if (choice < 0)
				choice = 1;
		}
		if (getButtonPress(buttonDown))
		{
			while (getButtonPress(buttonDown))
			{}
			choice++;
			if (choice > 1)
				choice = 0;
		}
	}
	eraseDisplay();
	return choice;
}

//Written by all
task main
{
	SensorType[S1] = sensorEV3_Gyro;
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S4] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Ultrasonic;
	string tableShape;
	int sprayNumber = 0;
	//menu is dsiplayed and the table typed is received
	int menuChoice = menu();
	if (menuChoice == 0)
		tableShape = "Rectangle";
	else if (menuChoice == 1)
		tableShape = "Circle";
	else if (menuChoice == 2)
		tableShape = "Odd";

	float tableLength = 0, tableWidth = 0, tableDiameter = 0;
	do
	{
		if (tableShape == "Rectangle")
		{
			adjust(S2, S4, S3, S1);
			wait1Msec(350);
			getRectDimensions(tableLength, tableWidth, S2, S4, S3, S1, sprayNumber);
		}
		else if (tableShape == "Circle")
		{
			nMotorEncoder[motorA] = 0;
			const int speed = -15;

			wait1Msec(50);
			motor[motorA] = motor[motorC] = speed;
			while(SensorValue[S2] == 1 && SensorValue[S4] == 1)
			{}
			motor[motorA] = motor[motorC] = 0;
			float Dia = (nMotorEncoder[motorA] * ((5.5 * PI) / 360)) - 12;
			redirectSafe(S2,S4);
			followCircum(Dia, S2,S4,S1);
		}
		int tableCount = 0;
		//clean area
		tableCount++;
		displayString(5, "Cleaning table #%d", tableCount);
	} while (sameTable(tableShape, tableLength, tableWidth, tableDiameter) == 0);
}
