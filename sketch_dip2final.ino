 // GRUPO DIP
 // ANDRE MARDEGAM
 //ERICK CRUZ
 //MARCELO MARQUUES
 //RAFAEL 
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
 bool ciclo = true;
 int xi,xd,yi,yd,ap,envia, trajetoria,sub, desceu = 0;
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
 void elevador();
 void aguardando_elevador();
 void entrandoelevador();
 void aguardando_elevador2();
 void quarto();
 void setup() 
 {
  
    Wire.begin();
    Serial.begin(9600);
    Serial2.begin(9600);
    bussola.init(); //Inicializando o Sensor QMC5883
    SerialBT.begin("Carrinho");// Inicializando comunicação Bluetooth
    delay(2000);
 }

 void loop()
  {
        Apartamento();
        elevador();        
        chego();     
        aguardando_elevador(); 
        entrandoelevador();
        aguardando_elevador2();
        quarto();
        
  }
 void elevador()
   {
      while(ciclo)
         {
            entrada_coordenadas();
            while(xi!=xd || yi!=yd)
             {
               decisao();
             }

            trajetoria++ ;           
    
        }
  }

 void Apartamento()

  {
     while(SerialBT.available()==0){}
     ap= SerialBT.parseInt();
  
  }
 void chego()
  {       
      if (trajetoria==3)
         {
           Serial2.write(1);
         }  
  }
 void aguardando_elevador()
  {
     while(Serial2.available()==0){}
  
     char chegou= Serial2.read();
     desceu=(chegou);
  } 
 void aguardando_elevador2()
  {
     while(Serial2.available()==0){}
  
     char subiu= Serial2.read();
     sub=(subiu);
     ciclo=true;
  } 

 void entrandoelevador()
  {
     if(desceu==1)
       {
           entrada_coordenadas();
           
           while(xi!=xd || yi!=yd)
             {
               decisao();
             }
           if(ap>=10 && ap<=19)
             {  
               Serial2.write(1);

             }  
           if(ap>=20 && ap<=29)
             {  
               Serial2.write(2);

             }
           if(ap>=30 && ap<=39)
             {  
               Serial2.write(3);

             }        
        }
   }
 void quarto()
  { 
     if(sub==1)
       {
          while(ciclo)
            {
              entrada_coordenadas();
              while(xi!=xd || yi!=yd)
               {
                 decisao();
               }

              trajetoria++ ;           
    
           }
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
       passogiro= ( passo * -1);
       if(passogiro<= 3)       
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
     delay(1000);
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
      delay(230);
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
    switch(trajetoria)

     {
       case 0:

       xi=8;
       xd=1;
       yi=1;
       yd=1;
       break;

      case 1:

      xi=1;
      xd=1;
      yi=6;
      yd=1;
      break;
  
     case 2:

      xi=1;
      xd=4;
      yi=1;
      yd=1;
      ciclo= false;
      envia=1;
      break;

     case 3:

     xi=0;
     xd=1;
     yi=0;
     yd=0;
     trajetoria++;
     break;
      case 4:
     xi=3;
     xd=0;
     yi=0;
     yd=0;
     break;

      case 5:

     xi=1;
     xd=8;
     yi=6;
     yd=2;
     ciclo= false;
     break;
  
  }
   
      Serial.print("xi");
      Serial.println(xi);
     Serial.print("xd");
      Serial.println(xd);
      Serial.print("yi");
      Serial.println(yi);
       Serial.print("yd");
      Serial.println(yd);
 
  
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
    
