/* include ESP Wi-Fi library */
#include <ESP8266WiFi.h>

/* WiFi network where this ESP based relay will be connected to */
const char* ssid     = "WiFi-SSID";
const char* password = "WiFi-Password";

/* set web server port number to 8080 */
WiFiServer server(8080);

/* variable to store the HTTP request */
String header;

/* to store the current cpu state - default is off */
String cpustate = "OFF";

/* the relay is connected to GPIO-0
 * use GPIO-2 for cpustate (on/off) detection 
 */
const int gpio0 = 0; /* Relay board's LED connected here, as well as Relay control pin */
const int gpio2 = 2; /* ESP LED connected here */

/* current time */
unsigned long currentTime = millis();
/* previous time */
unsigned long previousTime = 0; 
/* define timeout time in milliseconds (example: 2000ms = 2s) */
const long timeoutTime = 2000;

void setup() {
  /* set UART baudrate as 115200 */
  Serial.begin(115200);

  /* set GPIO pins set as output */
  pinMode(gpio0, OUTPUT);
  pinMode(gpio2, OUTPUT);
  
  /* set default outputs of GPIO pins
   * 
   * gpio0 as HIGH: relay's COM (common) and NO (normally-open) functions appropirately if gpio-0 is HIGH
   * Also, the Relay LED connected to GPIO-0 will become OFF here.
   * Here, NO and COM should be connected to CPU power buttons.
   * 
   * gpio2 as HIGH: default CPU is considered as OFF. Put LED to off to show CPU is OFF.
   */
  digitalWrite(gpio0, HIGH);
  digitalWrite(gpio2, HIGH);

  delay(2000);
  /* connect to Wi-Fi network with SSID and password */
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  /* print a dot every second while WiFi is in connecting */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); /* 1000ms delay */
    Serial.print(".");
  }
  /* inform wifi is connected on serial */
  Serial.println("");
  Serial.println("WiFi connected..");
  /* print URL of Boot-controller server */
  Serial.print("URL to access Boot-Controller Server: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println(":8080");

  /* start the server */
  server.begin();
}

void loop(){
  /* wait for a client */
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;

    /* wait for a client to send data within 2000ms */
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      /* ensure client availability */
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            /* HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
             * and a content-type so the client knows what's coming, then a blank line:
             */
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            /* turn CPU on/off */
            if (header.indexOf("GET /cpu/on") >= 0 && cpustate == "OFF") {
              /* indicate by GPIO-2 that CPU is ON */
              digitalWrite(gpio2, LOW);
              
              /* drive NO pin (= !gpio0)of relay low and then high with 1000ms delay
               * this is required to turn on CPU
               * Relay LED will be ON for 1000ms
               */
              digitalWrite(gpio0, LOW);
              delay(1000);
              digitalWrite(gpio0, HIGH);

              /* print on console */
              Serial.println("CPU is ON");
              cpustate = "ON";
            } else if (header.indexOf("GET /cpu/off") >= 0 && cpustate == "ON") {
              /* indicate by GPIO-2 that CPU is OFF */
              digitalWrite(gpio2, HIGH);
              
              /* drive NO pin (= !gpio0)of relay low and then high with 10s delay
               * this is required to turn on CPU
               */
              digitalWrite(gpio0, LOW);
              delay(10000);
              digitalWrite(gpio0, HIGH);

              /* print on console */
              Serial.println("CPU is OFF");
              cpustate = "OFF";
            }
     
            /* display the HTML web page */
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            /* CSS style for the CPU on/off button */
            client.println("<style> html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #006600; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #B32D00;}</style></head>");
            
            /* Web Page Heading */
            client.println("<body><h1>Boot Controller Server</h1>");
                           
            /* display the current CPU state */
            client.println("<p>CPU is " + cpustate + "</p>");
            
            /* display the ON button if cpu is OFF */
            if (cpustate == "OFF") {
              client.println("<p><a href=\"/cpu/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/cpu/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            /* The HTTP response ends with another blank line */
            client.println();
            /* terminate the while loop */
            break;
          } else { /* clear currentLine */
            currentLine = "";
          }
        } else if (c != '\r') { /* if we get anything else but a carriage return character */
          currentLine += c; /* and add it to the end of the currentLine */
        }
      }
    }
    /* clear the header variable */
    header = "";
    /* close the connection */
    client.stop();
  }
}
