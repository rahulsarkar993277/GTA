
#include <wiringPiI2C.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>

#define Device_Address 0x68						/*Device Address/Identifier for MPU6050*/

#define PWR_MGMT_1   0x6B
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define INT_ENABLE   0x38
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define GYRO_XOUT_H  0x43
#define GYRO_YOUT_H  0x45
#define GYRO_ZOUT_H  0x47
#define TEMPERATURE  0x41

int fd;

struct data
{   float ax;
	float ax;
	float ay;
	float az;
 	float gx;
	float gy;
	float gz;
	float t;
};


int fd;

void MPU6050_Init(){
	
	wiringPiI2CWriteReg8 (fd, SMPLRT_DIV, 0x07);	/* Write to sample rate register */
	wiringPiI2CWriteReg8 (fd, PWR_MGMT_1, 0x01);	/* Write to power management register */
	wiringPiI2CWriteReg8 (fd, CONFIG, 0);			/* Write to Configuration register */
	wiringPiI2CWriteReg8 (fd, GYRO_CONFIG, 24);		/* Write to Gyro Configuration register */
	wiringPiI2CWriteReg8 (fd, INT_ENABLE, 0x01);	/*Write to interrupt enable register */

	} 
short read_raw_data(int addr){
	short high_byte,low_byte,value;
	high_byte = wiringPiI2CReadReg8(fd, addr);
	low_byte = wiringPiI2CReadReg8(fd, addr+1);
	value = (high_byte << 8) | low_byte;
	return value;
}

void ms_delay(int val){
	int i,j;
	for(i=0;i<=val;i++)
		for(j=0;j<1200;j++);
}

struct data read_(struct data *d1)
{
			
//	struct data d1;

	float Acc_x,Acc_y,Acc_z;
	float Gyro_x,Gyro_y,Gyro_z;
	float Tmp;
	float Ax=0, Ay=0, Az=0;
	float Gx=0, Gy=0, Gz=0;
		/*Read raw value of Accelerometer and gyroscope from MPU6050*/
		Acc_x = read_raw_data(ACCEL_XOUT_H);
		Acc_y = read_raw_data(ACCEL_YOUT_H);
		Acc_z = read_raw_data(ACCEL_ZOUT_H);
		
		Gyro_x = read_raw_data(GYRO_XOUT_H);
		Gyro_y = read_raw_data(GYRO_YOUT_H);
		Gyro_z = read_raw_data(GYRO_ZOUT_H);
		
		Tmp = read_raw_data(TEMPERATURE);
		
		/* Divide raw value by sensitivity scale factor */
		d1->ax = Acc_x/16384.0;
		d1->ay = Acc_y/16384.0;
		d1->az = Acc_z/16384.0;
		
		d1->gx = Gyro_x/131;
		d1->gy = Gyro_y/131;
		d1->gz = Gyro_z/131;
		
		d1->t=Tmp/340+36.53;
		
		printf("\n Gx=%.3f °/s\tGy=%.3f °/s\tGz=%.3f °/s\tAx=%.3f g\tAy=%.3f g\tAz=%.3f g\tT=%.3f °C\n",d1->gx,d1->gy,d1->gz,d1->ax,d1->ay,d1->az,d1->t);
//		delay(500);
	//	delay(2000);	
//	return d1;
}

int comp(struct data *d1,struct data *d2)
{
	float ax;
	float ay;
	float az;
	float gx;
	float gy;
	float gz;
	float t;
	int i=0;
	ax=d1->ax-d2->ax;
	ay=d1->ay-d2->ay;
	az=d1->az-d2->az;
	gx=d1->gx-d2->gx;
	gy=d1->gy-d2->gy;
	gz=d1->gz-d2->gz;
	t=d1->t-d2->t;
	
	ax=ax<0?-ax:ax;
	ay=ay<0?-ay:ay;
	az=az<0?-az:az;
	gx=gx<0?-gx:gx;
	gy=gy<0?-gy:gy;
	gz=gz<0?-gz:gz;
	t =t<0?-t:t;
	
		printf("\n Gx d=%.3f °/s\tGy d=%.3f °/s\tGz d=%.3f °/s\tAx d=%.3f g\tAy d=%.3f g\tAz d=%.3f g\tT d=%.3f °C\n",gx,gy,gz,ax,ay,az,t);
//	ax>0.2?i++:0;
//	ay>0.2?i++:0;
//	az>0.2?i++:0;
//	gx>0.1?i++:0;
//	gy>0.1?i++:0;
//	gz>0.1?i++:0;
//	t =t>5?i++:0;
	ax>0.5?i++:0;
	ay>0.5?i++:0;
	az>0.5?i++:0;
	gx>2.5?i++:0;
	gy>0.5?i++:0;
	gz>0.5?i++:0;
//	if(i>0)
	
	return i;
}
//	float tollerate(int i)
//	{
//	const float tax=0.2;
//	const float tay=0.2;
//	const float taz=0.2;
//	const float tgx=0.1;
//	const float tgy=0.1;
//	const float tgz=0.1;
//	const float tt =5.0;
//	
//	switch(i)
//	{
//		case 1: return tax;
//		break;
//		case 2: return tay;
//		break;
//		case 3: return taz;
//		break;
//		case 4: return tgx;
//		break;
//		case 5: return tgy;
//		break;
//		case 6: return tgz;
//		break;
//		default: return tt;
//		break;		
//		
//	}
//	}

int main()
{
	
	

		struct data d1,d2,dr;
	int r;
	
	fd = wiringPiI2CSetup(Device_Address);   /*Initializes I2C with device Address*/
	MPU6050_Init();		                     /* Initializes MPU6050 */
	delay(100);
	read_(&d1);
	
	while(1)
	{
		delay(2000);
		read_(&d2);
		r=comp(&d1,&d2);
	if (r>0)
	{
		d1=d2;
		system("./on");
		break;
	}
	}
	return 0;
}
