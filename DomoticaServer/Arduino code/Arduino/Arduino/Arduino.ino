#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

IPAddress server_IP(192, 168, 1, 100);

EthernetClient client;
EthernetServer server(15327);

int  val = 0;
char code[10];
int bytesread = 0;

void SendMessage(String msg) 
{
	delay(100);
	if (client.connect(server_IP, 15326)) {
		Serial.println("connected");
	}
	else {
		Serial.println("connection failed");
		return;
	}

	client.print("RFID: " + msg);

	client.stop();
}

void setup() {
	Ethernet.begin(mac, ip);
	Serial.begin(2400);
	delay(1000);
	pinMode(2,OUTPUT);
	digitalWrite(2, LOW);
}

void loop() {
	EthernetClient serverClient = server.available();
	
	if (client)
	{
		//Client is verbonden
	}

	if (Serial.available() > 0) {          // if data available from reader 
		if ((val = Serial.read()) == 10) {   // check for header 
			bytesread = 0;
			while (bytesread<10) {              // read 10 digit code 
				if (Serial.available() > 0) {
					val = Serial.read();
					if ((val == 10) || (val == 13)) { // if header or stop bytes before the 10 digit reading 
						break;                       // stop reading 
					}
					code[bytesread] = val;         // add the digit           
					bytesread++;                   // ready to read next digit  
				}
			}
			if (bytesread == 10) {
				SendMessage(code);
			}
			//buffer clearen
			while (Serial.available() > 0)
			{
				Serial.read();
			}
			bytesread = 0;
			digitalWrite(2, HIGH);                  // deactivate the RFID reader for a moment so it will not flood
			delay(5000);                       // wait for a bit 
			digitalWrite(2, LOW);                 // Activate the RFID reader
		}
	}


}