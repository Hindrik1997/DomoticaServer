#include <SPI.h>
#include <Ethernet.h>
#include <string.h>

byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 0, 0, 177);

IPAddress server_IP(10,0,0,1);

EthernetClient client;
EthernetServer server(15327);

int  val = 0;
char code[10];
int bytesread = 0;

void ProcessMessage(String msg) 
{
	if (msg == "1")
	{
		digitalWrite(9, HIGH);
		delay(1500);
		digitalWrite(9, LOW);
	}
	if (msg == "0")
	{
		digitalWrite(8, HIGH);
		delay(1500);
		digitalWrite(8, LOW);
	}
	Serial.println(msg);
}

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

	pinMode(8,OUTPUT);
	pinMode(9,OUTPUT);
	Ethernet.begin(mac, ip);
	Serial.begin(2400);
	delay(1000);
	pinMode(2,OUTPUT);
	digitalWrite(2, LOW);
}

void loop() {
	//Buffer cleanen voor de zekerheid
	while (Serial.available() > 0)
	{
		Serial.read();
	}
	EthernetClient serverClient = server.available();
	
	if (serverClient)
	{
		Serial.println("Incoming client!");
		int i = 0;
		char buf[128];
		//Client is verbonden
		delay(500);
		while (serverClient.available() > 0)
		{
			buf[i] = serverClient.read();
			++i;
		}
		buf[i] = '\0';
		String data(buf);
		Serial.println(data);
		ProcessMessage(data);
	}
	serverClient.stop();

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
			delay(1500);                       // wait for a bit 
			digitalWrite(2, LOW);                 // Activate the RFID reader
		}
	}


}