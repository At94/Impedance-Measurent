//.................. "Variable initialization and definition".............//

int EN = 11;
#include "Wire.h"

#define SLAVE_ADD 0x0D
#define ADD_PTR 0xB0

#define START_FREQ_REG1 0x82
#define START_FREQ_REG2 0x83
#define START_FREQ_REG3 0x84

#define FREG_INCR_REG1 0x85
#define FREG_INCR_REG2 0x86
#define FREG_INCR_REG3 0x87

#define NUM_INCR_REG1 0x88
#define NUM_INCR_REG2 0x89

#define CYCLE_REG1 0x8A 
#define CYCLE_REG2 0x8B 

#define REAL_REG1 0x94
#define REAL_REG2 0x95

#define IMG_REG1 0x96
#define IMG_REG2 0x97


#define CRL_REG 0x80
#define CRL_REG1 0x81
#define STATUS_REG 0x8F



void freq_25K();
void freq_50K();
void freq_100K();
void freq_250K();
void freq_1M();
void freq_2M();
void freq_4M();
void freq_16M();
void runSweep(double gain);
void RFB(int val);


double phase_sweep(double img, double re);
void writeData(int addr, int data);
byte getFrequency(float freq, int n);
int readData(int addr);
//const float MCLK = 25.3*pow(10,3); // AD5933 Internal Clock Speed 16.776 MHz for formula
float freq=0; // Set start freq, < 100Khz (for formula)
float time_interval=0;
byte value;
int count=0;
float state; 
double gain;
const double pi = 3.141592654;
const int ds_address = 0x58; //DS1077 default address
float MCLK;
int foutyseven;
int threethirt;
int onek;
int tenk;
int foutysevenk;
int hundredk;
double phase=0;
double impedance=0;
float q=0;
int p=0;
short re=0;
short img=0;
double sysphase;

//////////........................................//////////////////

//.................. " In Setup fucntion we set pinMode and take data Serially ".............//

void setup() {

     pinMode(0, OUTPUT);
     pinMode(1, OUTPUT);
     pinMode(2, OUTPUT);
     pinMode(3, OUTPUT);
     pinMode(4, OUTPUT);
     pinMode(5, OUTPUT);
     
    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW); 
    digitalWrite(5, LOW);

    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);

    
    Wire.begin();
    Serial.begin(115200);

  while (Serial.available() == 0);
  String valu = Serial.readStringUntil('\n'); 
  freq = valu.toFloat();



  while (Serial.available() == 0);
  String val = Serial.readStringUntil('\n'); 
  time_interval = val.toFloat();
  writeData(CRL_REG,0x0);

}

//////////........................................//////////////////

void loop(){
         
    while (Serial.available() == 0);
   
     String sat = Serial.readStringUntil('\n'); 
     state = sat.toFloat();
      if (state == 1)
      { 
         RFB(2);

 if(freq>=10 && freq<=19)
     { 
      MCLK = 25.1*pow(10,3); 
      freq_25K();
      delay(100);
     }
     else if (freq>=20 && freq<=29)
     {
      MCLK = 50.5*pow(10,3);
      freq_50K();
      delay(100);
     }
     else if(freq>=30 && freq<=99)
     {

      MCLK = 101.19*pow(10,3);  
      freq_100K();
      delay(100);
     }
     else if(freq>=100 && freq<=199)
     {
      MCLK = 249.2*pow(10,3);  
      freq_250K();
      delay(100);
     }
     else if (freq>=200 && freq<=299)
     {
      MCLK = 1.01316*pow(10,6); 
      freq_1M();
      delay(100);
     }
     else if (freq>=300 && freq<=999)
     {
        MCLK = 2.1250*pow(10,6);
       freq_2M();
       delay(100);
     }
     else if(freq>=1000 && freq<=4999)
     {
        MCLK = 4.17*pow(10,6);
       freq_4M();
       delay(100);
     }
     else if(freq>=5000 || freq<=100000)
     {
        MCLK = 16.72*pow(10,6);
       freq_16M();
       delay(100);
     }
   
  writeData(START_FREQ_REG1, getFrequency(freq,1));
  writeData(START_FREQ_REG2, getFrequency(freq,2));
  writeData(START_FREQ_REG3, getFrequency(freq,3));

  float incre_freq1=0;
  writeData(FREG_INCR_REG1, getFrequency(incre_freq1,1)); 
  writeData(FREG_INCR_REG2, getFrequency(incre_freq1,2)); 
  writeData(FREG_INCR_REG3, getFrequency(incre_freq1,3));


 int incre_num1=0;
 writeData(NUM_INCR_REG1, (incre_num1 & 0x001F00)>>0x08 );
 writeData(NUM_INCR_REG2, (incre_num1 & 0x0000FF));

  writeData(CYCLE_REG1, 0x00);
  writeData(CYCLE_REG2, 0x0F);

          
  // 1. Standby '10110001'
 writeData(CRL_REG, 0xB1);
 
// Enable external oscillator
 writeData(CRL_REG1, 0x08);
 
// 2. Initialize sweep '00010001'
 writeData(CRL_REG, 0x11);
 delay(200);
     
// 3. Start sweep '00100000'
  writeData(CRL_REG, 0x21); 
     delay(2);

    if(freq>=9 && freq<=12)
     {
      gain = 2.27622956230382*pow(10,-7);
     }
     else if(freq>12 && freq<=18)
     {
      gain = 1.53610538911854*pow(10,-7);
     
     }
     else if(freq>18 && freq<=35)
     {
      gain = 1.19117529693022*pow(10,-7);
      
     }
     else if(freq>35 && freq<=58)
     {
      gain = 1.06611399317261*pow(10,-7);
     
     }
     else if(freq>58 && freq<=110)
     {
      gain = 1.01574093730542*pow(10,-7);
      
     }
     else if(freq>110 && freq<=180)
     {
      gain= 1.00528276090857*pow(10,-7);
       
     }
     else if(freq>180 && freq<=330)
     {
      gain = 9.20600083958728*pow(10,-8);
      
     }
     else if(freq>330 && freq<=580)
     {
       gain = 9.88230178573193*pow(10,-8);
      
     }  
     else if(freq>580 && freq<=1100)
     {
       gain = 9.96693966114399*pow(10,-8);
      
     } 
     else if(freq>1100 && freq<=2000)
     {
       gain = 9.96596622534046*pow(10,-8);
        
     } 
     else if(freq>2000 && freq<=4000)
     {
       gain = 9.95860209110727*pow(10,-8);
       
     } 
     else if(freq>4000 && freq<=7000)
     {
       gain = 9.90223523155882*pow(10,-8);
     
     } 
     else if(freq>7000 && freq<=12000)
     {
       gain = 9.97268481628819*pow(10,-8);
       
     } 
     else if(freq>12000 && freq<=19000)
     {
       gain = 9.97394804769941*pow(10,-8);
      
     } 
     else if(freq>19000 && freq<=35000)
     {
       gain = 9.99788044934474*pow(10,-8);
        
     } 

      else if(freq>35000 && freq<=52000)
     {
        gain = 1.00413*pow(10,-7);// gain of 50k

       runSweep(gain);
       
     } 
     
     else if(freq>52000 && freq<=60000)
     {
       gain = 1.0048130545312*pow(10,-7);
       
     } 
     else if(freq>60000 && freq<=110000)
     {
       gain = 1.01942306770906*pow(10,-7);
        
     }
         runSweep(gain);


      }
      else
      {
        Serial.print("Wrong input");
      }

}

void runSweep(double gain)
{

  double mag=0;
  double  MAG=0;
  int i=0;
  float j=0;

if (time_interval < 30)
{
  Serial.print("Please insert correct number");
}
else if (time_interval == 30)
{
while((readData(STATUS_REG) & 0x07) < 4 ) {  // Check that status reg != 4, sweep not complete

    if(freq>=10 && freq<=19)
     {
      delay(2100);// delay between measurements
     }
     else if (freq>=20 && freq<=29)
     {
      delay(1100); // delay between measurements
     }
     else if(freq>=30 && freq<=99)
     {
      delay(700); // delay between measurements
     }
     else if(freq>=100 && freq<=199)
     {
      delay(300); // delay between measurements
     }
     else if (freq>=200 && freq<=299)
     {
      delay(200); // delay between measurements
     }
     else if (freq>=300 && freq<=999)
     {
      delay(100); // delay between measurements
     }
     else if(freq>=1000 && freq<=4999)
     {
      delay(20); // delay between measurements
     }
     else if(freq>=5000 || freq<=100000)
     {
       delay(20); // delay between measurements
     }
   

     int flag = readData(STATUS_REG)& 2;
      if (flag==2)
    
    {


               
      byte R1 = readData(REAL_REG1);
      byte R2 = readData(REAL_REG2);
      re = (R1 << 8) | R2;

      R1  = readData(IMG_REG1);
      R2  = readData(IMG_REG2);
      img = (R1 << 8) | R2;


      
      mag = sqrt(pow(double(re),2)+pow(double(img),2));
      impedance =1.0/(gain*mag);

       check();
       q=q+time_interval;

         //Increment frequency
      if((readData(STATUS_REG) & 0x07) < 4 ){
        
       writeData(CRL_REG,0x41);
       
      } 
}
}
}

else
{
  while((readData(STATUS_REG) & 0x07) < 4 ) {  // Check that status reg != 4, sweep not complete

    if(freq>=10 && freq<=19)
     {
      delay(2100);// delay between measurements
     }
     else if (freq>=20 && freq<=29)
     {
      delay(1100); // delay between measurements
     }
     else if(freq>=30 && freq<=99)
     {
      delay(700); // delay between measurements
     }
     else if(freq>=100 && freq<=199)
     {
      delay(300); // delay between measurements
     }
     else if (freq>=200 && freq<=299)
     {
      delay(200); // delay between measurements
     }
     else if (freq>=300 && freq<=999)
     {
      delay(100); // delay between measurements
     }
     else if(freq>=1000 && freq<=4999)
     {
      delay(20); // delay between measurements
     }
     else if(freq>=5000 || freq<=100000)
     {
       delay(20); // delay between measurements
     }
   

     int flag = readData(STATUS_REG)& 2;
      if (flag==2)
    
    {           
      byte R1 = readData(REAL_REG1);
      byte R2 = readData(REAL_REG2);
      re = (R1 << 8) | R2;

      R1  = readData(IMG_REG1);
      R2  = readData(IMG_REG2);
      img = (R1 << 8) | R2;
           
      mag = sqrt(pow(double(re),2)+pow(double(img),2));
      impedance =1.0/(gain*mag);
     //Serial.print(mag);
   
        check();
        q=q+time_interval;
         //Increment frequency
      if((readData(STATUS_REG) & 0x07) < 4 ){
        
       writeData(CRL_REG,0x41);
       
      } 
     delay(time_interval);
}
}
}
}

void check()
{

if (impedance>=42.77 && impedance < 99)
  {
    if(foutyseven == 1)
    {
      
        if(freq>=9 && freq<=12)
     {

      sysphase= 209.95;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {

     sysphase= 226.33;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {

      sysphase= 239.22;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {

      sysphase= 253.85;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {

      sysphase= 261.34;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.48;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {

      sysphase= 267.17;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {

       sysphase= 269.76;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {

       sysphase= 270.89;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {

       sysphase= 272.74;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {

       sysphase= 275.59;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {

       sysphase= 273.67;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {

       sysphase= 276.76;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {

       sysphase= 282.15;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {

       sysphase= 291.69;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>35000 && freq<=52000)
     {

       sysphase=304.85;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {

       sysphase= 308.63;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {

       sysphase= 338.71;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }

      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++;
    }
    else
    {
      RFB(0);
      writeData(CRL_REG,0x41);
      check_47gain();
    }
  } 
  
 else if(impedance>=99 && impedance < 900)
  {
    if (threethirt == 1)
    {
      if(freq>=9 && freq<=12)
     {

      sysphase=209.96;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {

     sysphase= 226.33;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {

      sysphase=239.21;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {

      sysphase=253.86;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {

      sysphase= 261.34;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.49;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {

      sysphase= 267.16;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {

       sysphase= 269.76;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {

       sysphase= 270.87;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {

       sysphase= 272.71;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {

       sysphase= 275.54;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {

       sysphase=273.61;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {

       sysphase= 276.65;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {

       sysphase= 281.95;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {

       sysphase= 291.34;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 

     else if(freq>35000 && freq<=52000)
     {

       sysphase=304.1;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {

       sysphase= 308.01;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {

       sysphase= 337.56;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     
      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++;
  }
  else
  {
      RFB(1);
      writeData(CRL_REG,0x41);
      check_330gain();
  }
  }
 else if(impedance>=900 && impedance <9000)

  {
     if (onek == 1)
    {

      if(freq>=9 && freq<=12)
     {

      sysphase= 209.99; 
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {

     sysphase= 226.33;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {

      sysphase= 239.21;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {

      sysphase= 253.86;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {

      sysphase= 261.35;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.49;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {

      sysphase= 267.15;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {

       sysphase= 269.75;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {

       sysphase= 270.87;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {

       sysphase= 272.7;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {

       sysphase= 275.52;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {

       sysphase= 273.58;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {

       sysphase= 276.6;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {

       sysphase= 281.86;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {

       sysphase= 291.19;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 

     else if(freq>35000 && freq<=52000)
     {

       sysphase=304.17;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {

       sysphase= 307.72;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {

       sysphase= 337.05;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
   
      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++; 
  }
  else
  {
      RFB(2);
      writeData(CRL_REG,0x41);
      check_1kgain();
      
  }
  }
  else if(impedance>=9000 && impedance <42770)
  {
     if (tenk == 1)
    {

        if(freq>=9 && freq<=12)
     { 
      sysphase= 209.98;

       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {
     sysphase= 226.33;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {
      sysphase= 239.2;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {
      sysphase= 253.87;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {
      sysphase= 261.35;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.49;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {
      sysphase= 267.14;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {
       sysphase= 269.76;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {
       sysphase= 270.91;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {
       sysphase= 272.77;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {
       sysphase= 275.63;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {
       sysphase= 273.74;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {
       sysphase= 276.88;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {
       sysphase= 282.37;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {
       sysphase= 292.07;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 

     else if(freq>35000 && freq<=52000)
     {

       sysphase=307.1;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {
       sysphase= 309.24;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {
       sysphase= 339.56;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     
     
      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++;
  }
  else
  {
      RFB(3);
      writeData(CRL_REG,0x41);
      check_10kgain();
      
  }
     
  }
  else if(impedance>=42770 && impedance <99999)
  {
     if (foutysevenk == 1)
    {

      if(freq>=9 && freq<=12)
     {

      sysphase= 209.96; 
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {

     sysphase= 226.31;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {

      sysphase= 239.22;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {

      sysphase= 253.85;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {

      sysphase= 261.34;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.48;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {

      sysphase= 267.19;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {

       sysphase= 269.82;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {

       sysphase= 271.03;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {

       sysphase= 272.97;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {

       sysphase= 276;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {

       sysphase= 274.3;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {

       sysphase= 277.9;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {
       sysphase= 284.1;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {
       sysphase=295.11;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 

     else if(freq>35000 && freq<=52000)
     {

       sysphase=318.04;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {
       sysphase= 314.39;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {
       sysphase= 347.65;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }

      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++;
  }
  else
  {
      RFB(4);
      writeData(CRL_REG,0x41);
      check_47kgain();
      
  }
     
  } 
 else if(impedance>=9999 && impedance <=1.09*pow(10,6))
  {
     if (hundredk == 1)
    {

      if(freq>=9 && freq<=12)
     {

      sysphase= 209.97;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>12 && freq<=18)
     {

     sysphase= 226.38;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>18 && freq<=35)
     {

      sysphase= 239.22;
      phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>35 && freq<=58)
     {

      sysphase= 253.87;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>58 && freq<=110)
     {

      sysphase= 261.39;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>110 && freq<=180)
     {

      sysphase= 268.46;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>180 && freq<=330)
     {

      sysphase= 267.25;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
     else if(freq>330 && freq<=580)
     {

       sysphase= 269.86;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }  
     else if(freq>580 && freq<=1100)
     {

       sysphase= 271.12;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>1100 && freq<=2000)
     {

       sysphase= 273.13;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>2000 && freq<=4000)
     {

       sysphase= 276.26;
       phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>4000 && freq<=7000)
     {

       sysphase= 274.78;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>7000 && freq<=12000)
     {

       sysphase= 278.74;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>12000 && freq<=19000)
     {

       sysphase= 285.64;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>19000 && freq<=35000)
     {

       sysphase= 297.78;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 

      else if(freq>35000 && freq<=52000)
     {

       sysphase=331.31;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     
     else if(freq>52000 && freq<=60000)
     {

       sysphase= 319.45;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     } 
     else if(freq>60000 && freq<=110000)
     {

       sysphase= 355.15;
        phase = phase_sweep(img, re);
      phase = phase-sysphase;
     }
 
      Serial.print(q);
      Serial.print("\t");
      Serial.print(impedance);
      Serial.print("\t");
      Serial.println(phase); 
      p++;
  }
  else
  {
      
      RFB(5);
      writeData(CRL_REG,0x41);
      check_100kgain();
      
  }
     
  }
}



void check_47gain()
{
      if(freq>=9 && freq<=12)
     {
      
       gain = 4.51411326226163*pow(10,-6);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
       gain = 3.04870334073863*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
       gain = 2.37140172987116*pow(10,-6);
      runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
      gain = 2.12978509999788*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
      gain = 2.04610391838517*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain= 2.03279512439781*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
      
      gain = 1.87148178968681*pow(10,-6);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       
        gain = 2.00380631023855*pow(10,-6);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
       
      gain = 2.02128730856742*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
       
        gain = 2.02152352484324*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
       
       gain = 2.02035834287784*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       
       gain = 2.01051111293934*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       
      gain = 2.02421413529783*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       
       gain = 2.02391953106432*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
       
       gain = 2.02806920794284*pow(10,-6);
        runSweep(gain);
     } 
     else if(freq>35000 && freq<=52000)
     {
        gain = 2.2284*pow(10,-6);// gain of 50k

       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
        gain = 2.03690916063434*pow(10,-6);// gain of 5.6234k

       runSweep(gain);
       
     } 
     else if(freq>60000 && freq<=110000)
     {
       gain = 2.06360326550359*pow(10,-6);
        runSweep(gain);
     } 
}

   void check_330gain()
{
  if(freq>=9 && freq<=12)
     {
      
      gain = 2.20002156021129*pow(10,-6);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
     
      gain = 1.4852749838105*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
     gain = 1.15333735463624*pow(10,-6);
      runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
      gain = 1.03276551885107*pow(10,-6);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
      gain = 9.8447095515341*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain = 9.74223981887228*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
      
      gain = 8.94635230377518*pow(10,-7);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       
      gain = 9.58213277194811*pow(10,-7);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
       
      gain = 9.66163971552268*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
       
        gain = 9.66034236253333*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
       
       gain = 9.65308734692617*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       
      gain = 9.59998771201573*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       
       gain = 9.66780456726423*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       
       gain = 9.66999217697633*pow(10,-7);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
 
       gain = 9.69011966328772*pow(10,-7);
        runSweep(gain);
     } 
else if(freq>35000 && freq<=52000)
     {
       gain =9.6819*pow(10,-7);// gain of 50k
       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
        gain = 9.73840691348984*pow(10,-7);// gain of 5.6234k
       runSweep(gain);
       
     } 
     else if(freq>60000 && freq<=110000)
     {
       
       gain = 9.87588969421283*pow(10,-7);
        runSweep(gain);
     }
       
}

void check_1kgain()
{
  if(freq>=9 && freq<=12)
     {
      
      gain = 2.27622956230382*pow(10,-7);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
     
      gain = 1.53610538911854*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
       gain = 1.19117529693022*pow(10,-7);
      runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
      gain = 1.06611399317261*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
     gain = 1.01574093730542*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain= 1.00528276090857*pow(10,-7);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
      
      gain = 9.20600083958728*pow(10,-8);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       gain = 9.88230178573193*pow(10,-8);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
       
        gain = 9.96693966114399*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
       
        gain = 9.96596622534046*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
       
      gain = 9.95860209110727*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       
       gain = 9.90223523155882*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       
       gain = 9.97268481628819*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       
       gain = 9.97394804769941*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
       
       gain = 9.99788044934474*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>35000 && freq<=52000)
     {
       gain =1.00413*pow(10,-7);// gain of 50k
       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
       gain = 1.0048130545312*pow(10,-7);// gain of 5.6234k
       runSweep(gain);
       
     } 
     else if(freq>60000 && freq<=110000)
     {
       
      gain = 1.01942306770906*pow(10,-7);
        runSweep(gain);
     }
       
}

void check_10kgain()
{
  if(freq>=9 && freq<=12)
     {
    
      gain = 2.28183907101768*pow(10,-8);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
     
      gain = 1.54029093015089*pow(10,-8);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
      gain = 1.19402985074627*pow(10,-8);
      runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
     gain = 1.06869690580185*pow(10,-8);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
      gain = 1.01813916740651*pow(10,-8);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain= 1.00761454310222*pow(10,-8);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
      
      gain = 9.2255518263825*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       
       gain = 9.905805693659*pow(10,-9);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
       
      gain = 9.99074856682712*pow(10,-9);
       runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
       
       gain = 9.99034932255441*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
       
        gain = 9.98277970500886*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       
       gain = 9.92512485807071*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       
       gain = 9.99788044934474*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       
      gain = 1.00059835781798*pow(10,-8);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
       
       gain = 1.00314888434797*pow(10,-8);
       runSweep(gain);
     } 
    else if(freq>35000 && freq<=52000)
     {
       gain =1.0117*pow(10,-8);// gain of 50k
       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
      gain = 1.01046232693307*pow(10,-8);// gain of 5.6234k
       runSweep(gain);
       
     }
     else if(freq>60000 && freq<=110000)
     {
       
       gain = 1.03026506659633*pow(10,-8);
        runSweep(gain);
     }
     
}

void check_47kgain()
{
     if(freq>=9 && freq<=12)
     {
     
      gain = 4.87152853658601*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
     
      gain = 3.28942542487699*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
     gain = 2.55135247449808*pow(10,-9);
     runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
      gain = 2.28266631312731*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
      gain = 2.17385841347117*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain = 2.15230344895866*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
      
      gain = 1.97006245807215*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       
        gain = 2.11580051259499*pow(10,-9);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
       
      gain = 2.13414524185809*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
       
        gain = 2.13355672525669*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
       
       gain = 2.13284879558775*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       
       gain = 2.12102077942847*pow(10,-9);
       runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       
      gain = 2.13718286129803*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       
       gain = 2.14200026625063*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
       
       gain = 2.15870170498578*pow(10,-9);
        runSweep(gain);
     } 
else if(freq>35000 && freq<=52000)
     {
       gain =2.2880*pow(10,-9);// gain of 50k
       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
       gain = 2.20392188339935*pow(10,-9);// gain of 5.6234k
       runSweep(gain);
       
     }  
     else if(freq>60000 && freq<=110000)
     {
       
       gain = 2.3204545399171*pow(10,-9);
        runSweep(gain);
     }
     
}

void check_100kgain()
{
  if(freq>=9 && freq<=12)
     {
     
      gain = 2.86992797628751*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>12 && freq<=18)
     {
     
     gain = 1.93607843440953*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>18 && freq<=35)
     {
      
      gain = 1.50052458339435*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>35 && freq<=58)
     {
      
     gain = 1.34409902891533*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>58 && freq<=110)
     {
      
     gain = 1.27886919316654*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>110 && freq<=180)
     {
      
      gain= 1.26626132797384*pow(10,-9);
       runSweep(gain);
     }
     else if(freq>180 && freq<=330)
     {
       gain = 1.15875499655155*pow(10,-9);
      runSweep(gain);
     }
     else if(freq>330 && freq<=580)
     {
       gain = 1.24452037678103*pow(10,-9);
        runSweep(gain);
     }  
     else if(freq>580 && freq<=1100)
     {
      gain = 1.25553817890716*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>1100 && freq<=2000)
     {
        gain = 1.25504402192411*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>2000 && freq<=4000)
     {
        gain = 1.25420786739511*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>4000 && freq<=7000)
     {
       gain = 1.24789604726431*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>7000 && freq<=12000)
     {
       gain = 1.2583947513865*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>12000 && freq<=19000)
     {
       gain = 1.26360906968046*pow(10,-9);
        runSweep(gain);
     } 
     else if(freq>19000 && freq<=35000)
     {
      gain = 1.28045039074224*pow(10,-9);
        runSweep(gain);
     } 
 else if(freq>35000 && freq<=52000)
     {
       gain =1.2148*pow(10,-9);// gain of 50k
       runSweep(gain);
       
     } 
     else if(freq>52000 && freq<=60000)
     {
       gain = 1.33144162107811*pow(10,-9);// gain of 5.6234k
       runSweep(gain);
       
     }  
     else if(freq>60000 && freq<=110000)
     {
      gain = 1.46911566288849*pow(10,-9);
        runSweep(gain);
     }
     
}

void writeData(int addr, int data) {

 Wire.beginTransmission(SLAVE_ADD);
 Wire.write(addr);
 Wire.write(data);
 Wire.endTransmission();
 delay(1);
}


int readData(int addr){
  int data;

  Wire.beginTransmission(SLAVE_ADD);
  Wire.write(ADD_PTR);
  Wire.write(addr);
  Wire.endTransmission();
  delay(1);

  Wire.requestFrom(SLAVE_ADD,1);

  if (Wire.available() >= 1){
    data = Wire.read();
  }
  else {
    data = -1;
  }

  delay(1);
  return data;  
}

 
byte getFrequency(float freq, int n){
  long val = long((freq/(MCLK/4)) * pow(2,27));
  byte code;

    switch (n) {
      case 1:
        code = (val & 0xFF0000) >> 0x10; 
        break;
      
      case 2:
        code = (val & 0x00FF00) >> 0x08;
        break;

      case 3:
        code = (val & 0x0000FF);
        break;

      default: 
        code = 0;
    }

  return code;  
}


double phase_sweep(double img, double re)
{
  double phase;
  if ((re>0)&(img>0))
  {
    phase= atan(double(img)/double(re));
    phase = (phase*180)/pi;  
  }
    else if((re>0)&(img<0))
    {
    phase =  atan(double(img)/double(re));
    phase = ((phase*180)/pi)+360;  
    }
    else if ((re<0)&(img<0))
    {
      phase = atan(double(img)/double(re));
      phase = 180 + ((phase*180)/pi);
    }
    else if ((re<0)&(img>0))
    {
      phase = atan(double(img)/double(re));
      phase = ((phase*180)/pi)+180;
    }

    return phase;
  }



 void freq_25K()
  {
      i2c_write(ds_address, 0x02, 0x01, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0xA4, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }

  void freq_50K()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x80);
  delay(500);
       i2c_write(ds_address, 0x01, 0xA4, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }

    void freq_100K()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0xA4, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }

    void freq_250K()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0x42, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }

    void freq_1M()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0x10, 0x00);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
   delay(500); 
  }

      void freq_2M()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       //i2c_write(ds_address, 0x01, 0x07, 0xC0);
       i2c_write(ds_address, 0x01, 0x06, 0xC80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }

      void freq_4M()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0x03, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }


  void freq_16M()
  {
      i2c_write(ds_address, 0x02, 0x00, 0x00);
  delay(500);
       i2c_write(ds_address, 0x01, 0x00, 0x80);
  delay(500);
       i2c_write(ds_address, 0x0D, 0x08);
delay(500);
       i2c_write(ds_address, 0x3F);
    delay(500);
  }
 



  
  void i2c_write(int device, byte address) {
     Wire.beginTransmission(device); //start transmission to device 
     Wire.write(address);        // send register address
     Wire.endTransmission(); //end transmission
}

void i2c_write(int device, byte address, byte val1) {
     Wire.beginTransmission(device); //start transmission to device 
     Wire.write(address);        // send register address
     Wire.write(val1);        // send value to write
     Wire.endTransmission(); //end transmission
}

void i2c_write(int device, byte address, byte val1, byte val2) {
     Wire.beginTransmission(device); //start transmission to device 
     Wire.write(address);        // send register address
     Wire.write(val1);        // send value to write
     Wire.write(val2);        // send value to write
     Wire.endTransmission(); //end transmission
}

void RFB(int val)
   {
      

 if (val==0)//  CHANNEL 1
{
  
  
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);
    digitalWrite(0, LOW);
    digitalWrite(EN, HIGH);
     foutyseven=1;
     threethirt=0;
     onek=0;
     tenk=0;
     foutysevenk=0;
     hundredk=0;
    
}

       else if (val==1)//  CHANNEL 2
{
   
  //Serial.print("this switch is on");
  digitalWrite(2, LOW);
  digitalWrite(1, LOW);
  digitalWrite(0, HIGH);
  digitalWrite(EN, HIGH);
     foutyseven=0;
     threethirt=1;
     onek=0;
     tenk=0;
     foutysevenk=0;
     hundredk=0;
  
   
}

              else if (val==2)//  CHANNEL 3
{
    
    digitalWrite(2, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(0, LOW);
    digitalWrite(EN, HIGH);
    foutyseven=0;
     threethirt=0;
     onek=1;
     tenk=0;
     foutysevenk=0;
     hundredk=0;
     
}
            else if (val==3)//  CHANNEL 4
{

    digitalWrite(2, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(0, HIGH);
    digitalWrite(EN, HIGH);
    foutyseven=0;
     threethirt=0;
     onek=0;
     tenk=1;
     foutysevenk=0;
     hundredk=0;
   
}
        else if (val==4)//  CHANNEL 5
{

    digitalWrite(0, LOW);
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(EN, HIGH);
    foutyseven=0;
     threethirt=0;
     onek=0;
     tenk=0;
     foutysevenk=1;
     hundredk=0;
}
          else if (val==5)//  CHANNEL 6
{

    digitalWrite(0, HIGH);
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(EN, HIGH);
   foutyseven=0;
     threethirt=0;
     onek=0;
     tenk=0;
     foutysevenk=0;
     hundredk=1;
    
    
}
         else if (val==6)//  CHANNEL 7
{

    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(EN, HIGH);
   
}

        else if (val==7)//  CHANNEL 8
{

     digitalWrite(0, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(EN, HIGH);
     
}
    else if (val==9)
    {
      digitalWrite(EN, LOW);
      
    }
   }
 void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '+') {
      while(1);
    }
  }
}
