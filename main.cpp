/*
Konna-anturi
5.11.2018
Päivitetty 19.11.2018
Saku Huuha & Niko Hämäläinen
*/
#include "mbed.h"
#include "L3G4200D.h"

#define TS 0.025

Timer timer;
Ticker kulmatikkeri;
Serial pc(USBTX,USBRX);
L3G4200D gyro(D14, D15);
int g[3]={0, 0, 0};
int gr[3]={0, 0, 0};
double kulma[3] = {0.0 ,0.0 ,0.0};

double skaalaus = 0.06923;
int aika = 0;
double laskukulma = 0;
double kulmaprint = 0;
double testiarvo = 14.3;
double xKulma = 0;
AnalogIn xVal(A0);
AnalogIn yVal(A1);
AnalogIn zVal(A2);
double x,y,z;
double xr,yr,zr;
double kulma_x,kulma_y,kulma_z;
int laskuri = 0; 

void kulmatikkeri_isr(void)
{


g[0] = gr[0];
g[1] = gr[1];
g[2] = gr[2];

// kiihtyvyysanturin arvot

x = xr;
//x = x * 0.0015-48.466;
x = 0.0015 *  x - 48.83;

y= yr;
y = y* 0.0015 - 48.79;
z = zr;
z = 0.0015 * z - 49.813;
//x = xVal;
//x = x * 0.0103 + 0.4987;

if(x > 9.81){x = 9.81;}
if(y > 9.81){y = 9.81;}
if(z > 9.81){z = 9.81;}

if(x < -9.81){x = -9.81;}
if(y < -9.81){y = -9.81;}
if(z < -9.81){z = -9.81;}



//kulma_x laskut
 
if (kulma[1] > 90 && kulma [1] < 270)
{
  kulma_x = 180 + asin(x / 9.8101) * 180.0 / 3.141592;

}


else if(kulma[1] > 269 && kulma[1] < 360)
{
kulma_x = 360 - asin(x / 9.8101) * 180.0 / 3.141592;
}

else if( kulma[1] < -90 && kulma[1] > -270 ) 
{
kulma_x = -180 + asin(x / 9.8101) * 180.0 / 3.141592;

}

else if(kulma[1] < -269 && kulma[1] > -360)
{
kulma_x = -360 - asin(x / 9.8101) * 180.0 / 3.141592;
}


else
{

kulma_x = -asin(x /9.8101) * 180.0 / 3.141592;

}




//kulma_y laskut 

if(kulma[0] > 90 && kulma[0] < 270)
 { 
   kulma_y = 180 - asin(y / 9.8101) * 180.0 / 3.141592;

 }

else if(kulma[0] > 269 && kulma[0] < 360)
{
kulma_y = 360 + asin(y / 9.8101) * 180.0 / 3.141592;
}

else if( kulma[0] < -90 && kulma[0] > -270 ) 
{
kulma_y = -180 - asin(y / 9.8101) * 180.0 / 3.141592;

}

else if(kulma[0] < -269 && kulma[0] > -360)
{
kulma_y = -360 + asin(y / 9.8101) * 180.0 / 3.141592;
}



else
 {
   kulma_y = asin(y / 9.8101) * 180.0 / 3.141592;
 }




kulma_z = asin(z / 9.8101) * 180.0 / 3.141592;

 


                g[0] =g[0]- 10 ;

                g[1] =g[1] + 1.1 ;

                g[2] =g[2] -1.05 ;

	 
		

kulma[0]+= g[0] * 0.025 * skaalaus + 0.01*(kulma_y-kulma[0]);




kulma[1] += g[1] * 0.025 * skaalaus + 0.01*(kulma_x-kulma[1]); 


if ( kulma [0] > 360||kulma[0] < -360)
{
 kulma[0] = 0;
}
if(kulma [1] > 360||kulma[1] < -360) 
{
kulma[1] = 0;
}
 

                if(testiarvo==14.3)
		{
			testiarvo = 666.6;
		}
		else if(testiarvo==666.6)
		{
			testiarvo = 14.3;
		}
               
}

int main() 
{

	kulmatikkeri.attach(&kulmatikkeri_isr, TS); 
timer.start();
timer.reset();



	while(true)
	{
gyro.read(gr);
xr = xVal.read_u16();
yr = yVal.read_u16();
zr = zVal.read_u16();
	
if(timer.read_ms() >= 200)
{
pc.printf("%7.2f, %7.2f, %7.2f\n" ,testiarvo,kulma[0], kulma[1]);
timer.reset();
}
wait(0.01);

	}

}	  