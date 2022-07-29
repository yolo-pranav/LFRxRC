// Infrared Ray sensor
int ir1=5;
int ir2=26;

// Motor driver
int ena=21;
int in1=19;
int in2=18;
int in3=25;
int in4=33;
int enb=32;

// Speed controls
int ena_ch = 0;
int enb_ch = 1;
int res = 4;
int freq = 1000;

// Access Point Name credentials
const char* ssid = "LFRxRC";
const char* password = NULL;

// Load Wi-Fi library
#include <WiFi.h>

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current state
String lfr = "off";
String prompt = "stopped";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() { 
  // Create Access Point Name
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();

  // Define the pinmodes for microcontroller
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(2, OUTPUT);

  // Speed Control setup
  ledcAttachPin(ena, ena_ch);
  ledcAttachPin(enb, enb_ch);
  ledcSetup(ena_ch, freq, res);
  ledcSetup(enb_ch, freq, res);
}

// Moving Forward
void f() {
  ledcWrite(0,15);
  ledcWrite(1,15);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  prompt = "forward";
}

// Moving Backward
void b() {
  ledcWrite(0,15);
  ledcWrite(1,15);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  prompt = "back";
}


// Moving Left
void l() {
  ledcWrite(0,15);
  ledcWrite(1,15);
  if (prompt=="forward" || prompt=="back") {-
    ledcWrite(1,10);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
}

// Moving Right
void r() {
  ledcWrite(0,15);
  ledcWrite(1,15);
  if (prompt=="forward" || prompt=="back") {
    ledcWrite(0,10);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
}


// Stopping
void stop() {
  ledcWrite(0,0);
  ledcWrite(1,0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  prompt = "stopped";
}

void loop() {
  // Creating the web server
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("");
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #333;}");
            client.println("h1 { color: aliceblue;}");
            client.println("p { color: aliceblue;}");
            client.println("#button {width: 150px; border: none; color: #04da97; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border: solid 6px #04da97; border-radius: 20px; background-color: #333; text-shadow: 0 0 0.125em #04da97; box-shadow: 0 0 0.5em #04da97; position: relative;}");
            client.println("#button::before { content: ""; pointer-events: none; position: absolute; background: #04da97; top: 110%; left: 0; width: 100%; height: 100%; transform: perspective(1em) rotateX(40deg) scale(1, 0.45); filter: blur(1em); opacity: 0.4;}");
            client.println("#button::after { content: ""; position: absolute; top: 0; left: 0; bottom: 0; right: 0; box-shadow: 0 0 2em 0.5em #04da97; opacity: 0; transition: opacity 100ms linear;}");
            client.println("#button:hover { background-color: #04da97; color: #333; text-shadow: none;}");
            client.println("#button:hover::after { opacity: 0.6;}");
            client.println("#button2 { width: 160px; border: none; color: #fc5c34; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; border: solid 6px #fc5c34; border-radius: 20px; background-color: #333; text-shadow: 0 0 0.125em #fc5c34; box-shadow: 0 0 0.5em #fc5c34; position: relative;}");
            client.println("#button2::before { pointer-events: none; content: ""; position: absolute; background: #fc5c34; top: 110%; left: 0; width: 100%; height: 100%; transform: perspective(1em) rotateX(40deg) scale(1, 0.45); filter: blur(1em); opacity: 0.4;}");
            client.println("#button2:hover, #button2:focus { background-color: #fc5c34; color: #333; text-shadow: none;}");
            client.println("#button2::after { content: ""; position: absolute; top: 0; left: 0; bottom: 0; right: 0; box-shadow: 0 0 2em 0.5em #fc5c34; opacity: 0; transition: opacity 100ms linear;}");
            client.println("#button2:hover { background-color: #fc5c34; color: #333; text-shadow: none;}");
            client.println("#button2:hover::after { opacity: 0.6;}");
            client.println(".container { margin-top: 100px; position: relative; width: 100%;}");
            client.println(".forward { top: 37%; right: 59%; width: 40px; height: 60px; background-color:  #FFFFFF; border-radius: 50px 50px 100px 100px ;}");
            client.println(".forward2 { top: 37%; right: 59%; width: 40px; height: 60px; background-color:  #202129; border-radius: 50px 50px 100px 100px ;}");
            client.println(".backward { top: 55%; right: 59%; width: 40px; height: 60px; background-color:  #FFFFFF; border-radius: 100px 100px 50px 50px ;}");
            client.println(".backward2 { top: 55%; right: 59%; width: 40px; height: 60px; background-color:  #202129; border-radius: 100px 100px 50px 50px ;}");
            client.println(".left { margin-right: 20px; top: 49%; right: 40%; width: 63px; height: 37px; background-color:  #FFFFFF; border-radius: 50px 100px 100px 50px;}");
            client.println(".left2 { margin-right: 20px; top: 49%; right: 40%; width: 63px; height: 37px; background-color:  #202129; border-radius: 50px 100px 100px 50px;}");
            client.println(".right { margin-left: 20px; top: 49%; right: 34%; width: 63px; height: 37px; background-color:  #FFFFFF; border-radius: 100px 50px 50px 100px ;}");
            client.println(".right2 { margin-left: 20px; top: 49%; right: 34%; width: 63px; height: 37px; background-color:  #202129; border-radius: 100px 50px 50px 100px ;}");
            client.println("</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Control your Bot</h1>");
            
            // Display current state, and ON/OFF buttons for lfr
            client.println("<p>Line Follower Robot is " + lfr + "</p>");

            // Showing controls on webapp according to the requests and responses
            if (header.indexOf("GET /lfr/on") >= 0) {
              lfr="on";
              digitalWrite(2, HIGH);
              client.println("<span><a href=\"/lfr/off\"><button id=\"button2\">OFF</button></a></span>");
            } else {
              lfr="off";
              digitalWrite(2, LOW);
              client.println("<span><a href=\"/lfr/on\"><button id=\"button\">ON</button></a></span>");
              client.println("<br><div class=\"container\">");

              if (header.indexOf("GET /forward/on") >= 0) {
                client.println("<span><a href=\"/forward/off\"><button class=\"forward2\">/\\</button>></a></span><br>");
                client.println("<span><a href=\"/left/on\"><button class=\"left\">&lt;</button>></a></span>");
                client.println("<span><a href=\"/right/on\"><button class=\"right\">&gt;</button></a></span><br>");
                client.println("<span><a href=\"/backward/on\"><button class=\"backward\">\\/</button>></a></span>");
                f();
              } else if (header.indexOf("GET /backward/on") >= 0) {
                client.println("<span><a href=\"/forward/on\"><button class=\"forward\">/\\</button>></a></span><br>");
                client.println("<span><a href=\"/left/on\"><button class=\"left\">&lt;</button>></a></span>");
                client.println("<span><a href=\"/right/on\"><button class=\"right\">&gt;</button></a></span><br>");
                client.println("<span><a href=\"/backward/off\"><button class=\"backward2\">\\/</button>></a></span>");
                b();
              } else if (header.indexOf("GET /left/on") >= 0) {
                client.println("<span><a href=\"/forward/on\"><button class=\"forward\">/\\</button>></a></span><br>");
                client.println("<span><a href=\"/left/off\"><button class=\"left2\">&lt;</button>></a></span>");
                client.println("<span><a href=\"/right/on\"><button class=\"right\">&gt;</button></a></span><br>");
                client.println("<span><a href=\"/backward/on\"><button class=\"backward\">\\/</button>></a></span>");
                l();
              } else if (header.indexOf("GET /right/on") >= 0) {
                client.println("<span><a href=\"/forward/on\"><button class=\"forward\">/\\</button>></a></span><br>");
                client.println("<span><a href=\"/left/on\"><button class=\"left\">&lt;</button>></a></span>");
                client.println("<span><a href=\"/right/off\"><button class=\"right2\">&gt;</button></a></span><br>");
                client.println("<span><a href=\"/backward/on\"><button class=\"backward\">\\/</button>></a></span>");
                r();
              } else {
                client.println("<span><a href=\"/forward/on\"><button class=\"forward\">/\\</button>></a></span><br>");
                client.println("<span><a href=\"/left/on\"><button class=\"left\">&lt;</button>></a></span>");
                client.println("<span><a href=\"/right/on\"><button class=\"right\">&gt;</button></a></span><br>");
                client.println("<span><a href=\"/backward/on\"><button class=\"backward\">\\/</button>></a></span>");
                stop();
              }
              
              client.println("</div>");
            }
            client.println("<p>" + prompt + "</p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }

  // Controlling the LFR (0 - White, 1 - Black)
  if (lfr=="on") {
    // Reading IR sensors
    int a=digitalRead(ir1);
    int b=digitalRead(ir2);

    // Triggering Actuators
    if (a==0 && b==0) {
      f();
    } else if (a==0 && b==1) {
      l();
    } else if (a==1 && b==0) {
      r();
    }
  }
}
