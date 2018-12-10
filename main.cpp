/*
Konna-anturi
5.11.2018
Päivitetty 10.12.2018
Saku Huuha & Niko Hämäläinen
*/
#include "mbed.h"
#include "L3G4200D.h"
#include <math.h>
#define TS 0.025

Timer timer;
Ticker kulmatikkeri;
Serial pc(USBTX,USBRX);
L3G4200D gyro(D14, D15);
int g[2]={0, 0};
int gr[3]={0, 0, 0};
double kulma[2] = {0.0 ,0.0};

double skaalaus = 0.06923;
AnalogIn xVal(A0);
AnalogIn yVal(A1);
AnalogIn zVal(A2);
double x,y;
double xr,yr;
double kulma_x,kulma_y;

AnalogIn lampo(A3);
float Ur = 0;
float r0 = 12;
float U0 = 3.3;
float ntcR = 0;
float lampoLukema = 0;
float lampotila = 0;
float painotus = 0;

/*
Kulmien laskenta suoritetaan keskeytyksissa Tickerin avulla. 
Taustalla rajoittavana tekijana digitaalisesti lukemia antava gyrosensori vrt. analogisesti luettavaan kiihtyvyyssensoriin.
Tickerilla varmistetaan, etta gyrosensorista luetaan oikeita arvoja, eika painotus heittele koodin suoritusajan tai digitaalisen mittauksen vuoksi
*/

void kulmatikkeri_isr(void)
{

//asetetaan gyrosensorin arvot muuttujiin
g[0] = gr[0];
g[1] = gr[1];

// kiihtyvyysanturin arvot muuttujiin ja muunnos kiihtyvyydeksi kalibroinnin perusteella

x = xr;
//x = x * 0.0015-48.466;
x = 0.0015 *  x - 48.83;

y= yr;
y = y* 0.0015 - 48.79;

//x = xVal;
//x = x * 0.0103 + 0.4987;


//asetetaan kiihtyvyksien maksimit ja minimit
if(x > 9.81){x = 9.81;}
if(y > 9.81){y = 9.81;}


if(x < -9.81){x = -9.81;}
if(y < -9.81){y = -9.81;}




//kulma_x laskut kiihtyvyyden osalta
//kiihtyvyyden kulma x on gyron ja lopullisen kulman y
 
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




//kulma_y laskut kiihtyvyyden osalta
//kiihtyvyyden kulma y on gyron ja lopullisen kulman x

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



//gyro-arvon korjaus kalibroinnin perusteella


                g[0] =g[0]- 10 ;

                g[1] =g[1] + 1.1 ;

//lasketaan kulmat gyroarvojen ja kiihtyvyysarvojen perusteella	 
		

kulma[0]+= g[0] * 0.025 * skaalaus + 0.01*(kulma_y-kulma[0]);




kulma[1] += g[1] * 0.025 * skaalaus + 0.01*(kulma_x-kulma[1]); 

//pysytaan -360 ja +360 asteen valilla

if ( kulma [0] > 360||kulma[0] < -360)
{
 kulma[0] = 0;
}
if(kulma [1] > 360||kulma[1] < -360) 
{
kulma[1] = 0;
}
 
               
}

// lampotilan laskemiseen funktiot excelista. kaytetaan yhta(aaripaissa) tai kahta funktiota, painotettuna sensoriarvon perusteella, laskemaan lampotila
void lampotilalaskut()
{
lampoLukema = lampo.read_u16();
Ur = (lampoLukema/65535)*U0;

ntcR = r0*(U0/Ur-1);


if(ntcR >= 130.156865)
{
lampotila = (2*(pow(10,-8)))*(pow(ntcR,4))+(2*(pow(10,-5)))*(pow(ntcR , 3))+(0.0052*(pow(ntcR,2)))- 0.7976*ntcR+ 16.014;
}

if(ntcR >= 49.29905 && ntcR < 130.156865)
{
   //lampotila = -21.62*(log(ntcR)) + 71.969;

   painotus=(ntcR-49.29905 )/(130.156865-49.29905 );
   lampotila = (1-painotus)*(-21.62*(log(ntcR)) + 71.969) +(painotus) *(2*(pow(10,-8))*(pow(ntcR,4))+(2*(pow(10,-5)))*(pow(ntcR , 3))+(0.0052*(pow(ntcR,2)))- 0.7976*ntcR+ 16.014);
    
}

if(ntcR >=20.3535 && ntcR < 49.29905 )
{
  //lampotila = -25.93*(log(ntcR)) + 86.273;

painotus = (ntcR-20.3535)/(49.29905 -20.3535);

lampotila = (1-painotus)*(-25.93*(log(ntcR)) + 86.273) + (painotus)*(lampotila = -21.62*(log(ntcR)) + 71.969);

}

if (ntcR >=9.212 && ntcR < 20.3535)
{
  //lampotila = -0.0195 *(pow(ntcR,3)) + 0.7932
//*(pow(ntcR,2))-12.288 * ntcR + 88.058;

painotus = (ntcR-9.212)/(20.3535-9.212);
lampotila = (1-painotus)*(-0.0195 *(pow(ntcR,3)) + 0.7932
*(pow(ntcR,2))-12.288 * ntcR + 88.058 )+(painotus)* (-25.93*(log(ntcR)) + 86.273);


}

if (ntcR < 9.212 && ntcR >=4.2795)
{
 //lampotila = -28.36*(log(ntcR)) + 89.648;
  painotus = (ntcR-4.2795)/(9.212-4.2795);
lampotila = (1-painotus)*(-28.36*(log(ntcR)) + 89.648)+ (painotus)* (-0.0195 *(pow(ntcR,3)) + 0.7932
*(pow(ntcR,2))-12.288 * ntcR + 88.058);



}

if(ntcR< 4.2795 )
{
   lampotila = -28.36*(log(ntcR)) + 89.648;
  

}

}

int main() 
{
//kiinnitetaan Ticker
kulmatikkeri.attach(&kulmatikkeri_isr, TS); 
timer.start();
timer.reset();



	while(true)
	{
//luetaan sensoriarvot
gyro.read(gr);
xr = xVal.read_u16();
yr = yVal.read_u16();
	
//tulostetaan arvot sarjaportille noin 250 ms valein
if(timer.read_ms() >= 250)
{
lampotilalaskut();
pc.printf("%7.2f, %7.2f, %7.2f\n" ,lampotila,kulma[0],kulma[1]);
timer.reset();
}
wait(0.01);

	}

}	  