#include "certificates.h"
#include "WiFiEsp.h"

int status = WL_IDLE_STATUS;

WiFiEspServer server(80);
IPAddress ip(192, 168, 1, 70);  

void setup()
 {
	Serial.begin(115200);
	Serial3.begin(115200);

	WiFi.init(&Serial3);
	
	if(WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println("Escudo wifi no presente");
		while(true);
	}
	
	WiFi.config(ip);

	while(WiFi.status() != WL_CONNECTED)
	{
		Serial.println("Intentando la conexion ...");
		
		// Estableciendo la conexion con la red.
		status = WiFi.begin(ssd,pass);
	}

	server.begin();
 }

void loop()
 {

	WiFiEspClient client = server.available();
	
	if(client)
	{
		Serial.println("Nuevo cliente conectado.");
		while(client.connected())
		{
			if(client.available())
			{
				char c = client.read();
				Serial.write(c);
			}
		}		

	client.stop();
	Serial.println("Cliente desconectado.");

	}
 }
