#include <Esp32WifiManager.h>
#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>
#include <DFRobot_DHT11.h>
#include <WiFi.h> 
#include <PubSubClient.h>

// DECLARAÇÃO DE VARIÁVEIS GLOBAIS
#define pinsensor 14
DFRobot_DHT11 DHT;
int temperatura;
int idrobo=1;
int cheguei;
// DECLARAÇÃO DE VARIÁVEIS PARA WIFI
char ssid[] = "ADRIANA 2,4g";        // your network SSID (name)
char pass[] = "familia10";        // your network password
int status = WL_IDLE_STATUS;
// DECLARAÇÃO DE VARIÁVEIS PARA MySQL
IPAddress server_addr(85, 10, 205, 173);  // IP of the MySQL *server* here
char user[] = "robofatec7678";              // MySQL user login username
char password[] = "m76j78t98";          // MySQL user login password

char INSERT_SQL[] = "INSERT INTO robofatec76.robofatec (idrobo,temperatura) VALUES ('%d','%d')";
char query[128];// status
// INSTANCIANDO OBJETOS
WiFiClient client;
WiFiClient wifiClient;                        
 
//MQTT Server
const char* BROKER_MQTT = "test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT
#define ID_MQTT  "ROBOOFATEC"              //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_SUBSCRIBE "ELEVA/CHEGO"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define ID_MQTT  "ROBOFATEC"            //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_PUBLISH "ROBO/CHEGO"    //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient


MySQL_Connection conn((Client *)&client);
// DECLARAÇÃO DE FUNÇÕES
void conectaWifi();
void enviaDados();
void leituratemp();
void mantemConexoes();
void conectaMQTT();     //Faz conexão com Broker MQTT     //
void recebePacote(char* topic, byte* payload, unsigned int length);
void enviaValores();

void setup() 
 {
   Serial2.begin(9600);
   Serial.begin(9600);
   MQTT.setServer(BROKER_MQTT, BROKER_PORT);     
   delay(3000);
   while (!Serial)
    {
      ; // ESPERA O SERIAL ESTAR PRONTO
    }
    conectaWifi();
    // CONECTA NO MySQL
  while (!conn.connect(server_addr, 3306, user, password)) 
    {
      Serial.println("Conexão SQL falhou.");
      conn.close();
      delay(1000);
      Serial.println("Conectando SQL novamente.");
   }
   Serial.println("Conectado ao servidor SQL.");

 }

void loop() 
 {
  enviaDados();
  leituratemp();
  mantemConexoes();
  enviaValores();   
  MQTT.loop();
   while(cheguei>0)
  {
    MQTT.setCallback(recebePacote);    
    mantemConexoes();
    MQTT.loop();
  }
}
  
  




void conectaWifi() 
 {
   if (WiFi.status() == WL_CONNECTED)
     {
       Serial.println("Communication with WiFi module failed!");
       while (true);
     }
   while (status != WL_CONNECTED) 
   {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
 }

void enviaDados() 
 {
  sprintf(query, INSERT_SQL,idrobo,temperatura);
  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(query);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
  Serial.println("Informações Enviadas");
 }

void leituratemp()
 {
   DHT.read(pinsensor);
   temperatura= DHT.temperature;
   Serial.println(temperatura);
   delay(2000);
 }
void mantemConexoes() 
 {
    if (!MQTT.connected())
    {
       conectaMQTT(); 
    }
    
    conectaWifi(); //se não há conexão com o WiFI, a conexão é refeita
 }
void conectaMQTT() 
 { 
    while (!MQTT.connected())
    {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado ao Broker com sucesso!");
        } 
        else 
        {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
 }
 
void enviaValores()
   { 
          
           if(Serial1.available()>0)
            {
               char recebo= Serial1.read();
                int cheguei=(recebo);   
                if(cheguei==1)
                {   
                   cheguei=1;            
                   MQTT.publish(TOPIC_PUBLISH,"1");

                }

            }
          
              
      }

  void recebePacote(char* topic, byte* payload, unsigned int length) 
  {
     String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
     {
       char c = (char)payload[i];
       msg = c;
     }
     Serial.println(msg);
  }    
              
              
           


   
