
const String deviceNum = "2";

// if deviceNum == 1
//#define LCD


const String keyLetters[9][4] = {{"A", "B", "C", "1"}, {"D","E","F", "2"}, {"G","H","I", "3"}, {"J","K","L", "4"}, {"M","N","O", "5"}, {"P","Q","R", "6"}, {"S","T","U", "7"}, {"V","W", "8", "-1"}, {"X","Y","Z", "9"}};

const boolean hasKeypad = deviceNum != "3";

// Keypad logic
#include "Adafruit_Keypad.h"
#define KEYPAD_PID3844
#define R1    15
#define R2    23
#define R3    4
#define R4    16
#define C1    17
#define C2    5
#define C3    18
#define C4    19

#include "keypad_config.h"
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


#ifdef LCD
#include <LiquidCrystal.h>

const int rs = 22, en = 21, d4 = 27, d5 = 14, d6 = 32, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#endif

#ifndef LCD
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#endif

// Networking logic
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

const String ssid = "Device " + deviceNum;
const String password = "password";

WiFiServer server(80);

String message = "";

void setup() {
  Serial.begin(9600);


  #ifdef LCD
    lcd.begin(16,2);
    lcd.cursor();
    lcd.blink();
  #endif


  #ifndef LCD
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for (;;); // Don"t proceed, loop forever
    }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  #endif

  // start server host
  beginRouter();

  // custom keypad init
  customKeypad.begin();
}

void loop() {
  networkTick();
  if (hasKeypad) {
    keypadTick();
  }
}

void keypadTick() {
  customKeypad.tick();

  if (message.length() == 0) {
    printToDisplay("Enter your message");
  }
  else {
    printToDisplay(message);
  }

  delay(10);
  customKeypad.tick();
  keypadEvent e = customKeypad.read();
  if (e.bit.EVENT == KEY_JUST_PRESSED) {
    String pressedKey = String((char)e.bit.KEY);

    determineNextStep(pressedKey);
    
    printToDisplay(message);
  }
}

void typeFromLetter(String key) {
  int asInt = key.toInt();
  String arrOptions[4];

  for (int i = 0; i < 4; i++) {
      arrOptions[i] = keyLetters[asInt - 1][i];
  }
  message += arrOptions[0];
  printToDisplay(message);
  String next = getNextInput();
  if (next == key) {
    message = message.substring(0, message.length()-1) + arrOptions[1];
    printToDisplay(message);
    next = getNextInput();
    if (next == key) {
      message = message.substring(0, message.length()-1) + arrOptions[2];
      printToDisplay(message);
      next = getNextInput();
      if (key != "8" && next == key) {
        message = message.substring(0, message.length()-1) + arrOptions[3];
        printToDisplay(message);
      }
      else {
        determineNextStep(next);
      }
    }
    else {
      determineNextStep(next);
    }
  }
  else {
    determineNextStep(next);
  }
}

void determineNextStep(String pressedKey) {
  if (pressedKey == "D") {
    printToDisplay("Enter the device to send to");
    
    String deviceIdentifier = getNextInput();
    printToDisplay("Sending message to device " + deviceIdentifier);
    sendTo(deviceIdentifier, message);
    printToDisplay("Enter your message");
  }
  else if (pressedKey == "A") {
    message += "help";
  }
  else if (pressedKey == "B") {
    message += "zombie";
  }
  else if (pressedKey == "C") {
    message += "rip";
  }
  else if (pressedKey == "*") {
    message += " ";
  }
  else if (pressedKey == "#") {
    message = message.substring(0, message.length()-1);
    printToDisplay(message);
  }
  else if (pressedKey == "0") {
    String next = getNextInput();
    if (next == "0"){
      message = message + "0";
    }
    else {
      determineNextStep(next);
    }
  }
  else {
    typeFromLetter(pressedKey);
  }
}

String getNextInput() {
  customKeypad.tick();
  keypadEvent e = customKeypad.read();

  customKeypad.tick();
  while (!customKeypad.available() && e.bit.EVENT != KEY_JUST_PRESSED) {
    customKeypad.tick();
    e = customKeypad.read();
    delay(10);
  }

  return String((char)e.bit.KEY);
}

void networkTick() {
  WiFiClient client = server.available();

  if (client) {
    String message = "";
    Serial.println("Recieving message...");
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        message += String(c);
      }
    }

    client.stop();
    printToDisplay("Incoming message:");
    if (hasKeypad) {
      delay(2000);
      printToDisplay(message);
      getNextInput();
    }
    else {
      printToDisplay(message);
    }
  }
}

void beginRouter() {
  WiFi.softAP(ssid, password);
  server.begin();
}

void sendTo(String device, String message) {
  WiFi.begin("Device " + device, password);

  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    printToDisplay("Connection failed. Please try again.");
    getNextInput();
  }
  else {
    // Connect to TCP server
    WiFiClient client;
    if (!client.connect(WiFi.gatewayIP(), 80)) {
      Serial.println("Connection failed. Please try again.");
      getNextInput();
    }
    else {
      Serial.println("Sending...");
      // Send data to server
      client.println(message);
      printToDisplay("Sent!");
      message = "";
      getNextInput();
      printToDisplay(message);
    }
  }
  beginRouter();
}

#ifdef LCD
void printToDisplay(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (message.length() > 16) {
    lcd.print(message.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print(message.substring(16));
  }
  else {
    lcd.print(message);
  }
}
#endif

#ifndef LCD
void printToDisplay(String message) {
  Serial.println(message);
  
  display.clearDisplay();    
  display.setCursor(0,0); 
  display.print(message);
  display.display();
}
#endif
