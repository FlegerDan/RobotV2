#include "Sabertooth.h"
#include "mbed.h"

const uint8_t fullSpeed = 127;
const uint8_t searchSpeed = 95;
const uint8_t mediumSpeed = 63;
const uint8_t slowSpeed = 31;
const uint8_t stopSpeed=0;

// > calibrated line -> black
// < calibrated line -> white
  

DigitalIn sensor5(p16, PullUp);
DigitalIn sensor4(p17, PullUp);
DigitalIn sensor3(p18, PullUp);
DigitalIn sensor2(p19, PullUp);
DigitalIn sensor1(p20, PullUp);
DigitalIn sensors[] = {sensor1,sensor2,sensor3,sensor4,sensor5};

int8_t vector_blana_fata_stanga []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
slowSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-slowSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-searchSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-mediumSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
};
int8_t vector_blana_fata_dreapta []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-searchSpeed,
fullSpeed,
fullSpeed,
slowSpeed,
-mediumSpeed,
-slowSpeed,
fullSpeed,
};
int8_t vector_step_stanga1 []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
stopSpeed,
stopSpeed,
stopSpeed,
fullSpeed,
};
int8_t vector_step_dreapta1 []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-searchSpeed,
fullSpeed,
fullSpeed,
slowSpeed,
-mediumSpeed,
-slowSpeed,
fullSpeed,
};
int8_t vector_step_stanga2 []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
slowSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-slowSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-searchSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
-mediumSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
};
int8_t vector_step_dreapta2 []=
{
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
fullSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
fullSpeed,
stopSpeed,
stopSpeed,
stopSpeed,
stopSpeed,
fullSpeed,
};
DigitalIn start(p6);

Timer t1;
Timer t2;
Timer timer_strat;
Timer calibrare;


// senzori de linie
DigitalInOut line1(p11);
DigitalInOut line2(p10);
int resLine1 = 1000;
int resLine2 = 1000;
int prevResLine1 = 1000;
int prevResLine2 = 1000;

const int calibratedLine = 800;      // mai mare de 800 negru, mai mic alb ? ok   
const int maxLineTime = 1200;         
const int maxLineTime1 = 1200; 
const int maxLineTime2 = 1200; 
//senzori led
DigitalOut myled(LED1);
DigitalOut myled2(LED2);

//comunicare saber cu uart
Sabertooth s(p9, 128, 9600);
Serial pc(USBTX, USBRX,NULL,19200);

int valoriSenzori;

int optiune_start=1;
void citireSenzori()
{
  
    valoriSenzori=0; // asta ii tot regiune critica, daca se intrerupe aici ii o sansa sa fie 0 si asa detecteaza pe toti senzorii si nu ii ok
    for(int i=0;i<5;i++)
    {
    valoriSenzori |=sensors[i].read()<<i;
    }
    
}
void citireLinie()
{
    line1.output();
    line1.mode(PullUp);
    wait_us(5);
    line1.mode(PullNone);
    line1 = 1;
    line2.output();
    line2.mode(PullUp);
    wait_us(5);
    line2.mode(PullNone);
    line2 = 1;
    wait_us(10);// o setat pe amandoi unu dupa altu
    line1.input();
    line2.input();
    t1.start();
    while(line1==1 && line2==1 && t1.read_us() < maxLineTime)
    {
        if(line1==0 && t1.read_us() < maxLineTime)
        {
        resLine1 = t1.read_us();
        }
        if(line2==0 && t1.read_us() < maxLineTime)
        {
        resLine2 = t1.read_us();
        }   
    }
    t1.stop();
    t1.reset();

}
void citireLinie1()
{
    /*
    void readWhite() {
    unsigned char i;
    
    for(i = 0; i < 2; i++)
    {
    
       lineSensors[i] = maxLineTime;
       pinMode(linePins[i], OUTPUT); 
       pinSetFast(linePins[i]);
    }
       delayMicroseconds(10);              // charge lines for 10 us
     for(i = 0; i < 2; i++)
    {   
       pinMode(linePins[i], INPUT); 
    }
       unsigned long startTime = micros();
      while (micros() - startTime < maxLineTime)
      {
        unsigned int time = micros() - startTime;
         for(i = 0; i < 2; i++)
    {   
      
            if (pinReadFast(linePins[i]) == LOW && time < lineSensors[i]) lineSensors[i] = time;
    }
        
      }
     if (lineSensors[0] > calibratedLine) leftLine = 0; else leftLine = 1; 
     if (lineSensors[1] > calibratedLine) rightLine = 0; else rightLine = 1;    
    }
 */
    line1.output();
    line1.mode(PullUp);
    wait_us(5);
    line1.mode(PullNone);
    line1 = 1;// o setat pe amandoi unu dupa altu
    
    wait_us(10);
    
    line1.input(); // o setat pe amandoi de input inainte de timer
    t1.start();
    prevResLine1 = resLine1;
    while(line1 == 1 &&  t1.read_us() < maxLineTime);
    resLine1 = t1.read_us();
    t1.stop();
    t1.reset();
    
}
void citireLinie2()
{
    line2.output();
    line2.mode(PullUp);
    wait_us(5);
    line2.mode(PullNone);
    line2 = 1;
    wait_us(10);
    t2.start();
    line2.input();
    prevResLine2 = resLine2;
    while(line2 == 1 &&  t2.read_us() < maxLineTime);
    resLine2 = t2.read_us();
    t2.stop();
    t2.reset();
    
}

void modul_blanaFata()
{
            uint8_t test=valoriSenzori;
            s.go(vector_blana_fata_stanga[test],vector_blana_fata_dreapta[test]);
            
}
void modul_toparceanu()
{
            uint8_t test=valoriSenzori;
               if(timer_strat.read_ms()<50)
                    {
                    s.go(vector_step_stanga1[test],vector_step_dreapta1[test]);
                    }
                else
                    {
                    s.go(vector_step_stanga2[test],vector_step_dreapta2[test]);
                    }
            
}
int ok_jap=0;
int ok_toparceanu=0;
void japoneza() // start 
{
    if(resLine1 < calibratedLine && prevResLine1 < calibratedLine)
        {
            if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(fullSpeed,-fullSpeed);
            }
        }

        //line is on the right
        else if(resLine2 < calibratedLine && prevResLine2 < calibratedLine)
        {
             if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(-fullSpeed,fullSpeed);
            }
               
        }
        else if(valoriSenzori<31)
        {
            modul_blanaFata();
        }
        else if(ok_jap==0)
        {
            if(timer_strat.read_ms()<2000)
            {
            s.go(stopSpeed,stopSpeed);
            }
            else
            {
            s.go(mediumSpeed,fullSpeed);
            } 
        }
        
        
}
void step_by_step()
{
     if(resLine1 < calibratedLine && prevResLine1 < calibratedLine)
        {
            if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(fullSpeed,-fullSpeed);
            }
        }

        //line is on the right
        else if(resLine2 < calibratedLine && prevResLine2 < calibratedLine)
        {
            if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(-fullSpeed,fullSpeed);
            }
        }
        else if(valoriSenzori<31)
        {
            modul_toparceanu();
            ok_toparceanu=1;
        }
        else if(ok_jap==0)
        {
            if(timer_strat.read_ms()<2000)
            {
            s.go(stopSpeed,stopSpeed);
            }
            else
            {
            s.go(mediumSpeed,fullSpeed);
            } 
        }
}

void blanaFata()
{
        if(resLine1 < calibratedLine && prevResLine1 < calibratedLine)
        {
            if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(fullSpeed,-fullSpeed);
            }
        }

        //line is on the right
        else if(resLine2 < calibratedLine && prevResLine2 < calibratedLine)
        {
            if(calibrare.read_ms()<100)
            {
            s.go(-fullSpeed,-fullSpeed);
            }
            else
            {
             s.go(-fullSpeed,fullSpeed);
            }
        }
        /*
        else if()
        {
           modul_toparceanu();
           ok_toparceanu=1; 
        }
         */
        else
        {
            //decisions
            modul_blanaFata();
        }
}

int main()
{
   
   for (int i = 0; i < 3; i++) 
    {
       calibrare.start();
       while(calibrare.read_ms()<100)
       {
        myled2 = 1;
       }
       calibrare.stop();
       calibrare.reset();           
       calibrare.start();
       while(calibrare.read_ms()<100)
       {
        myled2 = 0;
       }
        
       calibrare.stop();
       calibrare.reset();
    }
    calibrare.stop();
    calibrare.reset();
    s.setTimeout(0);
    s.setRamping(8);
    s.go(0,0);
    myled = 1;

    pc.printf("hello");
    bool run;
    pc.printf("hello");
    do
    {
      
        run = start.read();
        citireSenzori();
        citireLinie();
        pc.printf("values %d %d %d %d %d ", valoriSenzori & 1<<0,valoriSenzori & 1<<1,valoriSenzori & 1<<2,valoriSenzori & 1<<3,valoriSenzori & 1<<4);
        pc.printf("line %d \t%d\t", resLine1,resLine2);
        pc.printf("prev line %d \t%d\r\n", prevResLine1, prevResLine2);
    }while(!run);
     timer_strat.start(); 
     while (1)
     {
        citireSenzori();
        citireLinie();
        if((resLine1 < calibratedLine && prevResLine1 < calibratedLine) || (resLine2 < calibratedLine && prevResLine2 < calibratedLine)) // cand ii in cazul de calibrare se activeaza timer-ul
        {
        calibrare.start();
        }
        else
        {
        calibrare.stop();
        calibrare.reset();
        }
        switch(optiune_start)
        {
           case 1:
           {
                japoneza(); // start japoneza-blana fata
                 if(timer_strat.read_ms()>2020)
                {
                timer_strat.stop();
                timer_strat.reset();
                timer_strat.start();
                }
                break;
           }
           case 2:
           {
                step_by_step(); // start japoneza-toparceanu
                if(ok_toparceanu==0) // asta ca sa se schimbe functionalitatea de timer de pe japoneza pe toparceanu
                {
                    if(timer_strat.read_ms()>2020)
                    {
                     timer_strat.stop();
                     timer_strat.reset();
                     timer_strat.start();
                    }
                }
                else
                {
                    if(timer_strat.read_ms()>100)
                    {
                     timer_strat.stop();
                     timer_strat.reset();
                     timer_strat.start();
                    }
                }
                 
                break;
           }
           case 3:
           {
                break;
           }
           case 4:
           {
                blanaFata(); // blana fata si daca gaseste tanc toparceanu;
                if(ok_toparceanu==1)
                {
                    if(timer_strat.read_ms()>100)
                    {
                     timer_strat.stop();
                     timer_strat.reset();
                     timer_strat.start();
                    }
                }
                break;
           }
        }
       if(calibrare.read_ms()>300)
       {
           calibrare.stop();
           calibrare.reset();
           calibrare.start();
       }
        // maxim 30 de minute in microsecunde
    }

    return 0;
}