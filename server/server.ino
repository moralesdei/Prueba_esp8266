/* Creado por : Deimer Andres Morales
cambiar las credenciales en certificates.h 
por las propias */

#include "certificates.h"
#include "WiFiEsp.h"
#include<Adafruit_GFX.h>
#include<MCUFRIEND_kbv.h>

#define NUM_DEVICES  12

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

MCUFRIEND_kbv tft;
WiFiEspServer server(80);
IPAddress ip(192, 168, 1, 70);  

int states[12];
int room=0, current_state=0;
int control[NUM_DEVICES];

void setup()
 {

  char ssid[] = ssd;
  char passw[] = pass;
  int status = WL_IDLE_STATUS;
    
  memset(control, 0, NUM_DEVICES*sizeof(control[0]));
	Serial.begin(115200);
	Serial3.begin(115200);
  
  tft.reset();
  uint16_t identifier = tft.readID();
  Serial.print(identifier, HEX);
  if(identifier == 0XEFEF) identifier = 0X9486;
  tft.begin(identifier);
  tft.setTextSize(1); 
  
	WiFi.init(&Serial3);
	
	if(WiFi.status() == WL_NO_SHIELD)
	{
		while(true);
	}
 
	// Configurando la conexion ip por la fija 192.168.1.70
	WiFi.config(ip);

	while(WiFi.status() != WL_CONNECTED)
	{	
		// Estableciendo la conexion con la red.
		status = WiFi.begin(ssid,passw);
	}
	// Iniciando el servidor http.
	server.begin();
 
  ShowResult();
 }

void loop()
 {
    String buff;
    boolean reference = false;
	  // Obtiene un cliente que este conectado al server y envie datos.
	  WiFiEspClient client = server.available();	
    
    if(client)
    {
        buff = ResponseClient(client);
        ParserRequest(buff);
        ShowResult();
    }
 } 
void ShowResult()
 {
        tft.fillScreen(BLACK);
        tft.setCursor(0,0);
        tft.setTextWrap(true);
        for(int i=0;i<NUM_DEVICES;i++)
            {
                tft.println();
                tft.println();
                tft.print("habitacion No ");
                tft.print(i+1);
                control[room] = current_state;

                    if(i < 9) tft.print(" ");
                    if(control[i] == 0)
                    {
                        tft.print(" ");
                        tft.print("Sin novedad");
                    }
                    else if(control[i] == 1)
                    {
                        tft.print(" ");
                        tft.print("LLamada normal"); 
                    }
                    else if(control[i] == 2)
                    {
                        tft.print(" ");
                        tft.print("Emergencia");
                    }
            }
 }
void ParserRequest(String str)
 {
    int indexStart;
    int indexFinish;
    char tmp[4];
    String strinTmp;

    indexStart = str.indexOf("r");
    indexFinish = str.indexOf("s");
    strinTmp = str.substring(indexStart + 2, indexFinish - 1);
    strinTmp.toCharArray(tmp, sizeof(tmp));
    room = atoi(tmp);


    indexStart = str.indexOf("s");
    indexFinish = str.length();
    strinTmp = str.substring(indexStart + 2, indexFinish);
    strinTmp.toCharArray(tmp, sizeof(tmp));
    current_state = atoi(tmp);
 }

String ResponseClient(WiFiEspClient client)
 {
  String buff;
  boolean reading = false;
  boolean LineBlank;
  char tmp;
	buff = "";
	LineBlank = true;
	while(client.connected())
    	{
    		if(client.available())
    			{
    				// Leyendo el request del cliente
    				tmp = client.read();
            
    				if(reading && tmp ==' ') reading = false;
    				
    				// empieza a guardar la info cuando se en cuentra a ?
    				if(tmp == '?') reading = true; 
    				
    				if(reading)
    				{
    					if(tmp != '?')
    					{
    						buff += tmp;
    					}
    				}
                    // enviar el response para terminar la comunicacion.
    				if(tmp == '\n' && LineBlank) 
    				{
            client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "\r\n");
              break;
    				}
    				
    				if(tmp == '\n')
    				{
    					LineBlank = true;
    				}
    				else if(tmp != '\r')
    				{
    					LineBlank = false;
    				}
    			}
		}
	  client.stop();
    return buff;
 }
