

/*
  Διαμόρφωση ακροδεκτών LCD
  
 * LCD RS pin to digital pin 23
 * LCD Enable pin to digital pin 22
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 2
 * LCD D7 pin to digital pin 18
 * LCD R/W pin to ground
 * 10K potentiometer:
*/

//====================Βιβλιοθήκες================//
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "mainpage.h"
#include "SPIFFSIniFile.h"
#include"SPIFFS.h"
//==============================================//
AsyncWebServer server(80);  
#define FIRMWARE_VERSION "v1.0.1"
//===================Ορισμός ssid και password τοπικού δικτύου=================//
const char* ssid = "FiNa";
const char* password = "Myt1l3ne9598";
//=============================================================================//

//=============================Ορισμός username και password για την σύνδεση στην ιστοσελίδα===================//
const char* username = "ele516059"; 
const char* pass = "134740"; 
//============================================================================================================//

//=======================================Ορισμός μεταβλητών================================================//
const char* PARAM_STRING1 = "inputString1"; //text input απο website
const char* PARAM_INPUT = "value";
const char *filename = "/config.ini"; //ονομασια αρχειου ini σε config
const int rs = 23, en = 22, d4 = 5, d5 = 4, d6 = 2, d7 = 18; //pins LCD οθόνης
static String getMacAddress(void);
uint8_t baseMac[6];
String  MAC; // αποθήκευση δεύθυνσης MAC
String message;
String logmessage;
float LDR; // pin LDR 32 LDR = AnalogRead(34)
int x; // μεταβλητή mapping των τιμών του LDR απο 0-4095 σε 0-100
const size_t bufferLen = 80;  //μεγεθος του buffer
char buffer1[bufferLen];  // αποθηκευση μηνύματος LCD στο buffer
size_t content_len;
size_t fwUpdateProgress;
//==========================================================================================================//  
  
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //αρχικοποίηση των pins της LCD

//======================ρουτίνα εμφάνισης σφαλμάτων σε περίπτωση που δεν υπάρχει το ini αρχείο στην μνήμη =============//
void printErrorMessage(uint8_t e, bool eol = true)
{
  switch (e) {
  case SPIFFSIniFile::errorNoError:
    Serial.print("no error");
    break;
  case SPIFFSIniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case SPIFFSIniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case SPIFFSIniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case SPIFFSIniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case SPIFFSIniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case SPIFFSIniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case SPIFFSIniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}
//===============================================================================================================//

//====================ρουτίνα ανάγνωσης αρχείου ini απο την μνήμη=================================//
void readFile(){
   
  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
   SPIFFSIniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    // Cannot do anything else
    while (1);
  }
  
    Serial.println("Ini file exists"); 
   Serial.println("geting settings from ini file...");  
     
   if(ini.getValue("Message", "Text", buffer1, bufferLen)){
    Serial.print("Text= ");
    Serial.println(buffer1); 
  }
  
  else{
    Serial.print("Could not read 'Text' from section 'Message', error was ");
    printErrorMessage(ini.getError());
  }
}
//=================================================================================================//

//==================================ρουτίνα εμφάνισης MAC διεύθυνσης==================================//
static String getMacAddress(void)
{
  esp_read_mac( baseMac, ESP_MAC_WIFI_STA ); 
  char macStr[18] = { 0 };
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return String(macStr);
}
//====================================================================================================//

//=========================================αποθήκευση διεύθυνσης MAC και επιστροφή της τιμής σε μορφή String==================================//
String readMAC() {
  MAC = getMacAddress();
  return String (MAC);
}
//=========================================================================================================================================//

//==============================================Αποθήκευση διεύθυνσης IP και επιστροφή σε μορφή String====================================//
String readIP() {
  String ipadd = WiFi.localIP().toString().c_str();
  return String (ipadd);
}
//=======================================================================================================================================//

//======================================================AnalogRead pin LDR και mapping τιμής=============================================//
String readLDR(){
  LDR = analogRead(32);
  x = map(LDR,4095,3580,0,100);
  return String(x);
  }
//=====================================================================================================================================//

//=============================================αποθήκευση της τιμής του buffer στην μεταβλητή msg======================================//
String readText(){
  String msg = buffer1;
  return msg;
  }
//====================================================================================================================================//

//===============================================ρουτίνα ενεργοποίησης και σύνδεσης στο τοπικό δίκτυο=================================//
void wifiInit(){
WiFi.mode(WIFI_STA); //Λειτουργία του ESP ως Web Server
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
 
 
  while(WiFi.waitForConnectResult() != WL_CONNECTED){ //Αναμονή για σύνδεση στο δίκτυο   
      Serial.print(".");
    }
    
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Εκτύπωση IP διεύθυνσης
  }
//==========================================================================================================================================//

//==========================================ρουτίνα ενεργοποίησης LCD και εμφάνιση μυνήματος===============================================//
 void readLCD(){
  lcd.begin(16, 2); 
      message = lcd.print(buffer1);  
      delay(1000);
  }
//===========================================================================================================================================//

//======================================ρουτίνα Setup===================================// 
void setup() {
Serial.begin(115200);
pinMode(32,INPUT); //ορισμός pin 32 ώς είσοδος 
//ενεργοποίηση SPIFFS
 if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
 wifiInit();  //ρουτίνα ενεργοποίησης WiFi
 readFile(); //ρουτίνα ανάγνωσης αρχείου
 readLCD(); //ρουτίνα μηνύματος LCD
 
 //OTA Update//
 ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else 
      type = "filesystem";
    SPIFFS.end(); 
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin( );
   Update.onProgress(printProgress);
//=========================================== Τέλος OTA Update========================================//

//===========================================Ενεργοποίηση το DNS=====================================//
if (MDNS.begin("dipae")) {
    Serial.println("MDNS responder started");
  }    
configureWebServers();//κάλεσμα ιστοσελιδών 
server.begin(); //ενεργοποίηση Server
}

void loop() {
//======================================κύλιση μυνήματος προς τα αριστερά κατα μία θέση με ταχύτητα 150ms=====================//
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
   
   lcd.setCursor(0,1);
     lcd.print("Brightness=   %");  
     lcd.setCursor(11,1);
     lcd.print(x);  //εμφάνιση ποσοστού φωτεινότητας στην LCD
     Serial.println(LDR);
  }
 
  delay(2500);  
}
//=========================================ρουτίνες εμφάνισης MAC,IP,Brightness και Text στην ιστοσελίδα==========================//
String functn(const String& var) {
 if (var == "mac") {
    return readMAC();
  }
  else if (var == "Ip") {
    return readIP();
  }
 else if (var=="VALUEP"){
  return readLDR();
  }
  else if (var == "TEXT"){
    return readText();
  }
  return String();
}
//==================================================================================================================================//

//===============================================ρουτίνα εμφάνισης δεδομένων αποθηκευτικού χώρου SPIFFS και διαχείρηση της μνήμης============================//
String processor(const String& var) {
  if (var == "FIRMWARE") {
    return FIRMWARE_VERSION;
  }

  if (var == "FREESPIFFS") {
    return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  }

  if (var == "USEDSPIFFS") {
    return humanReadableSize(SPIFFS.usedBytes());
  }

  if (var == "TOTALSPIFFS") {
    return humanReadableSize(SPIFFS.totalBytes());
  }
}
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}
void handleDoUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.println("Update");
    content_len = request->contentLength();
    
    int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
    {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
#ifdef ESP8266
  }
  else
  {
    Serial.printf("Progress: %d%%\n", (Update.progress() * 100) / Update.size());
#endif
  }

  if (final)
  {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
    response->addHeader("Refresh", "20");
    response->addHeader("Location", "/");
    request->send(response);
    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      Serial.println("Update complete");
      Serial.flush();
      ESP.restart();
    }
  }
}
static void printProgress(size_t prg, size_t sz)
{
  Serial.printf("Progress: %d%%\r\n", (prg * 100) / content_len);
  fwUpdateProgress = (prg * 100) / content_len;
}
String listFiles(bool ishtml) {
  String returnText = "";
  Serial.println("Listing files stored on SPIFFS");
  File root = SPIFFS.open("/");
  File foundfile = root.openNextFile();
  if (ishtml) {
    returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
  }
  while (foundfile) {
    if (ishtml) {
      returnText += "<tr align='left'><td>" + String("<a href='") + String(foundfile.name()) + String("'>") + String(foundfile.name()) + String("</a>") + String("</td><td>") + humanReadableSize(foundfile.size()) + "</td>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'download\')\">Download</button>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
    } else {
      returnText += "File: "  + String(foundfile.name()) + " Size: " + humanReadableSize(foundfile.size()) + "\n";
    }
    foundfile = root.openNextFile();
  }
  if (ishtml) {
    returnText += "</table>";
  }
  root.close();
  foundfile.close();
  return returnText;
}


String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

//======================================handler για το ανέβασμα αρχείων στο SPIFFS μέσω της default ιστοσελίδας============================//

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  

  if (!index) {
    logmessage = "Upload Start: " + String(filename);
    
    request->_tempFile = SPIFFS.open("/" + filename, "w");
    Serial.println(logmessage);
  }

  if (len) {
    
    request->_tempFile.write(data, len);
    logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
    Serial.println(logmessage);
  }

  if (final) {
    logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
    // close the file handle as the upload is now done
    request->_tempFile.close();
    Serial.println(logmessage);
    request->redirect("/");
  }
}
//========================================================================================================================//
