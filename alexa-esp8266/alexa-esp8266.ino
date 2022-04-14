/* include headers */
#include <ESP8266WiFi.h>
#include <Espalexa.h>

/* GPIO connected to relay */
#define relay_pin 0

/* variable to store current state of device */
int is_on = false;

/* WiFi Credentials to connect to */
const char* ssid = "WiFi-SSID";
const char* password = "WiFi-Password";

/* device name for which alexa will listen to */
String device_name = "Lamp";

/* Esp-Alexa library */
Espalexa espalexa;

/* connect to WiFi
 * returns true if successful or false if not
 */
boolean connect_wifi()
{
	boolean state = true;
	int i = 0;

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println("");
	Serial.println("Connecting to WiFi");

	Serial.print("Connecting...");
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
		if (i > 20) {
			state = false;
			break;
		}
		i++;
	}
	Serial.println("");
	if (state) {
		Serial.print("Connected to ");
		Serial.println(ssid);
		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	} else {
		Serial.println("Connection failed.");
	}
	return state;
}

/* callback function for on/off command */
void command_handler(uint8_t val)
{
	/* device control */
	if (is_on == false && val == 0xFF) { /* ON */
		digitalWrite(relay_pin, LOW);
		Serial.print(device_name);
		Serial.println(" ON");
		is_on = true;
	} else if (is_on == true && val != 0XFF) { /* OFF */
		digitalWrite(relay_pin, HIGH);
		Serial.print(device_name);
		Serial.println(" OFF");
		is_on = false;
	}
}

void setup()
{
	boolean is_wifi_connected;

	Serial.begin(115200);

	pinMode(relay_pin, OUTPUT);
	digitalWrite(relay_pin, HIGH);

	/* connect to WiFi */
	is_wifi_connected = connect_wifi();

	if (is_wifi_connected) {
		espalexa.addDevice(device_name, command_handler);
		espalexa.begin();
	} else {
		while (1) {
			Serial.println("Cannot connect to WiFi..");
			delay(2000);
		}
	}
}

void loop()
{
	espalexa.loop();
	delay(1);
}
