
// GRUPO DIP
// ANDRE MARDEGAM
//EDURADO WILLIAM
//JUNIOR GOLTARA
//ERICK CRUZ
//MARCELO MARQUUES
//MIKE G
//RAFAEL 
//REENAID FREITAS  
#include <HCSR04.h> // Biblioteca do sensor ultrasonico.
#include <Wire.h>  // Biblioteca do sensor HMC 5883 GY-271
#include <MechaQMC5883.h> // Biblioteca do sensor HMC 5883 GY-271
#include <BluetoothSerial.h> // Biblioteca da comunicação via Bluetooth
#include <RoboCore_Vespa.h> //Biblioteca Vespa
 VespaMotors motores;
#define PINO_TRIGGER  25 
#define PINO_ECHO  26
UltraSonicDistanceSensor distancesensor( PINO_TRIGGER,PINO_ECHO); // criação do objeto do sensor ultrasonico
int distancia;
int passogiro,posfutura=0;
MechaQMC5883 bussola; //Criacao do objeto para o sensor  
int x = 0, y = 0, z = 0,angulo=0;
int angulo1 = 0;
int posatual=1;
BluetoothSerial SerialBT; // criação do obejeto da comunicação Bluetooth

int xi,xd,yi,yd = 0;
const int velocidade = 100;
int tempo=600;
int passo,laco= 0;
void re();
void frente();
void viraesquerda();
void viradireita();
void entrada_coordenadas();
void decisao();
void rotacao();
void setup() 
{
  
   Wire.begin();
  Serial.begin(9600);
  bussola.init(); //Inicializando o Sensor QMC5883
  SerialBT.begin("Carrinho");// Inicializando comunicação Bluetooth
  delay(2000);
}

void loop()
{
   entrada_coordenadas();
 while(xi!=xd || yi!=yd)
 {
   decisao();
 } 
}
void rotacao() // programa que direciona o robo para posição de 1 a 8
{
  
   if (passo == 0)
   {
    delay(200);
   }
    if ( passo > 4)
    {
      passogiro = (passo - 8) * -1;
      if (passogiro <= 3)
      {
        angulo1= 360;
        angulo = (45 * passogiro);
        angulo= angulo1-angulo;
        while ( angulo1 > angulo)
        {
                viraesquerda();
                 delay(150);
                bussola.read(&x,&y,&z); //Obter o valor dos eixos X, Y e Z do Sensor
                angulo1 = atan2(x, y)/0.0174532925; //Calculo do angulo usando os eixos X e Y atraves da formula  
                if(angulo1 < 0) //Ajuste do angulo entre 0 e 360 graus
                angulo1+=360;  
                angulo1 = 360-angulo1;  
                angulo1=angulo1;
        }
                motores.stop();
      }

    }
    

    if ( passo >= 1 && passo <= 4)
    {
      passogiro = passo;
      if (passogiro <= 4)
      {
         angulo =  (45 * passogiro);
         angulo1=1;
         while (angulo1 < angulo)
        {
                viradireita();
                delay(150);
                bussola.read(&x,&y,&z); //Obter o valor dos eixos X, Y e Z do Sensor
                angulo1 = atan2(x, y)/0.0174532925; //Calculo do angulo usando os eixos X e Y atraves da formula  
                if(angulo1 < 0) //Ajuste do angulo entre 0 e 360 graus
                angulo1+=360;  
                angulo1 = 360-angulo1;  
                angulo1=angulo1;
        }
                motores.stop();
      }
    }

  

     if(passo < -3) 
  {
       passogiro= (8 + passo);
     if(passogiro<=4) 
      {
       angulo =  (45 * passogiro);
       angulo1=angulo1;
       angulo= angulo1 + angulo;
        while (angulo1 < angulo)
        {
           
                viradireita();
                delay(150);
                bussola.read(&x,&y,&z); //Obter o valor dos eixos X, Y e Z do Sensor
                angulo1 = atan2(x, y)/0.0174532925; //Calculo do angulo usando os eixos X e Y atraves da formula  
                if(angulo1 < 0) //Ajuste do angulo entre 0 e 360 graus
                angulo1+=360;  
                angulo1 = 360-angulo1;  
                angulo1=angulo1;
          
        }  
                 motores.stop();
      }
  
   }  
      
   if(passo >= -3 && passo < 1)  
   {
     if(passogiro<= 3) 
     passogiro= ( passo * -1);
      {
         angulo =  (45 * passogiro);
         angulo1=angulo1;
         angulo= angulo1-angulo;
         
         while (angulo1 > angulo)
        {
         
                viraesquerda();
                delay(150);
                bussola.read(&x,&y,&z); //Obter o valor dos eixos X, Y e Z do Sensor
                angulo1 = atan2(x, y)/0.0174532925; //Calculo do angulo usando os eixos X e Y atraves da formula  
                if(angulo1 < 0) //Ajuste do angulo entre 0 e 360 graus
                angulo1+=360;  
                angulo1 = 360-angulo1;  
                angulo1=angulo1;
        }  
                motores.stop();
      }
     
   }
 }
 

void frente()
{
  
    distancia = distancesensor.measureDistanceCm();
      while( distancia<40)
       {
         motores.stop();
         distancia = distancesensor.measureDistanceCm();   
      }
  
  if(posfutura%2==0)
  {
     motores.forward(velocidade);
     delay (tempo*1.44);
     motores.stop();
  }
  else
     motores.forward(velocidade);
     delay (tempo);
     motores.stop();
}


void viraesquerda()
{
      motores.setSpeedLeft(-100);
      motores.setSpeedRight(100);
      delay(190);
      motores.stop();
         
}

void viradireita()
{
      motores.setSpeedLeft(100);
      motores.setSpeedRight(-100);
      delay(290);
      motores.stop();
}
void re()
{
     motores.backward(velocidade);
     delay (tempo);
     motores.stop();
}

void entrada_coordenadas()
{
 while(SerialBT.available()==0){}

    xi= SerialBT.parseInt();
    delay(3000);       
    xd= SerialBT.parseInt();
    delay(3000);
    yi= SerialBT.parseInt();
    delay(3000);
    yd= SerialBT.parseInt();
    delay(3000);
   
    Serial.print("xi");
    Serial.print(xi);
     Serial.print("xd");
      Serial.print(xd);
      Serial.print("yi");
      Serial.print(yi);
       Serial.print("yd");
      Serial.print(yd);
 
  delay(8000);
}

void decisao()
{
  if(yd >= yi)
 {
     if(xd == xi)
       {
         if(yd == yi)
           {
            
             motores.stop();
           } 

            else 
             {
                posfutura = 1;
                rotacao();
                frente();
                yd = yd - 1;
                posatual = posfutura;
             }
       } 
   
   else if (yd == yi)
   {  
      if(xd > xi)
       {       
        
          posfutura = 3;
          rotacao();  
          frente();
          xd = xd - 1;
          posatual = posfutura; 
       }
       else 
        {
          posfutura = 7;
          rotacao();  
          frente();
          xd = xd + 1;
          posatual = posfutura; 
            
        }
   }

   

   else if(xd > xi )
     {
       posfutura = 2;
       rotacao();  
       frente();
       xd= xd - 1;
       yd = yd - 1;
       posatual = posfutura; 
    
     }

   else
    {
        posfutura = 8;
       rotacao();  
       frente();
       xd= xd + 1;
       yd = yd - 1;
       posatual = posfutura;
    } 
 }
   else
    {
       if(xd == xi)
         {
             posfutura = 5;
             rotacao();  
             frente();
             yd = yd + 1;
             posatual = posfutura;  
         } 

       else 
         {
           if( xd < xi)
             {
              
                 posfutura = 6;
                 rotacao();  
                 frente();
                 xd = xd + 1;
                 yd = yd + 1;    
                 posatual = posfutura;  
             }
           else 
             {
                 posfutura = 4;
                 rotacao();  
                 frente();
                 xd = xd - 1;
                 yd = yd + 1;
                 posatual = posfutura;  
             }
             
          }
           
    }
   
      
}
    
