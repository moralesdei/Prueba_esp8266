/* Creado por : Deimer Andres Morales
cambiar las credenciales en certificates.h 
por las propias */

#include "certificates.h"
#include "WiFiEsp.h"
#define NUM_DEVICES  12

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
    String buffer;
	// Obtiene un cliente que este conectado al server y envie datos.
	WiFiEspClient client = server.available();	
    
    if(client)
    {
        buffer = ResponseClient(client); 
        ParserRequest(buffer);
        ShowResult();
    }
    
 } 
void ShowResult()
 {

        for(int i=0;i<NUM_DEVICES;i++)
            {
                Serial.println();
                Serial.print("habitacion No ");
                Serial.print(i+1);
                control[room] = current_state;
 
                    if(control[i] == 0)
                    {
                        Serial.print(" Sin novedad");
                    }
                    else if(control[i] == 1)
                    {
                        Serial.print(" LLamada normal"); 
                    }
                    else if(control[i] == 2)
                    {
                        Serial.print(" Emergencia");
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
    indexFinish = str.indexOf("e");
    strinTmp = str.substring(indexStart + 2, indexFinish - 1);
    strinTmp.toCharArray(tmp, sizeof(tmp));
    current_state = atoi(tmp);
 }

String ResponseClient(WiFiEspClient client)
 {
    String buffer;
    boolean reading = false;
    boolean LineBlank;
    char tmp;
	buffer = "";
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
						buffer += tmp;
					}
				}
                // enviar el response para terminar la comunicacion.
				if(tmp == '\n' && LineBlank) 
				{
                    client.println("HTTP/1.1 200 OK");
                    client.println("Connection: close");
                    client.println();
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
    return buffer;
 }
