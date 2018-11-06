/*
Konna-anturi
5.11.2018
Saku Huuha
*/
#include "mbed.h"
#include "L3G4200D.h"


Timer timer;
Serial pc(USBTX,USBRX);
L3G4200D gyro(D14, D15);
int g[3]={0, 0, 0};
double kulma[3] = {0.0 ,0.0 ,0.0};
double skaalaus = 0.06923;
int aika = 0;
double laskukulma = 0;
double kulmaprint = 0;
double testiarvo = 14.3;


int main() 
{
timer.start();
timer.reset();


	while(true)
	{
	/*
		gyro.read(g);

                // ota aika 
                aika = timer.read_ms();
                timer.reset();
                g[0] =g[0] - 10;

                g[1] =g[1] - 13;

                g[2] =g[2] - 16;

		for(int i=0; i<3; i++)
		{
                     kulma[i] += g[i] * 0.001 * aika * skaalaus;
	            //pc.printf("%i, ",g[i]);
                    // pc.printf("%f, ",kulma[i]);

		}
                laskukulma = kulma[1]/90;
              
                if(laskukulma > 1)
                {
                 laskukulma = 1;
                }
                if(laskukulma < 0)
                {
                 laskukulma = 0;
                }
                

   
            kulmaprint = kulma[1];

		if (kulmaprint <0)
		{
			kulmaprint = 0;
		}
		if(kulmaprint > 90)
		{
			kulmaprint = 90;
		}

                  pc.printf("%f,",kulmaprint); 
                  pc.printf("%f,",nopeuskm);  
 	*/
                if(testiarvo==14.3)
		{
			testiarvo = 666.6;
		}
		else if(testiarvo==666.6)
		{
			testiarvo = 14.3;
		}
                
                     
		pc.printf("%f",testiarvo);
		pc.printf("\n");
		wait(5);
	}
}	  