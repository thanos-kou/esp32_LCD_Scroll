void configureWebServers() {
   server.onFileUpload(handleUpload);
server.on("/upload",HTTP_GET,[](AsyncWebServerRequest * request){
 if(!request->authenticate(username, pass))
      return request->requestAuthentication();    
  request->send_P(200, "text/html", index_html,processor);
 });
server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest * request) {   
     if(!request->authenticate(username, pass))
      return request->requestAuthentication();
    request->send_P(401, "text/html", logout_html,processor);
  });
server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest * request){
    if(!request->authenticate(username, pass))
      return request->requestAuthentication();
     request->send(200, "text/plain", listFiles(true));
  });
server.on("/file", HTTP_GET, [](AsyncWebServerRequest * request) {
   if(!request->authenticate(username, pass))
      return request->requestAuthentication();
  if (request->hasParam("name") && request->hasParam("action")) {
        const char *fileName = request->getParam("name")->value().c_str();
        const char *fileAction = request->getParam("action")->value().c_str();
if (!SPIFFS.exists(fileName)) {
          
          request->send(400, "text/plain", "ERROR: file does not exist");
        } else {
         
          if (strcmp(fileAction, "download") == 0) {
            logmessage += " downloaded";
            request->send(SPIFFS, fileName, "application/octet-stream");
          } else if (strcmp(fileAction, "delete") == 0) {
            logmessage += " deleted";
            SPIFFS.remove(fileName);
            request->send(200, "text/plain", "Deleted File: " + String(fileName));
          } else {
            logmessage += " ERROR: invalid action param supplied";
            request->send(400, "text/plain", "ERROR: invalid action param supplied");
          }
          Serial.println(logmessage);
        }
      } else {
        request->send(400, "text/plain", "ERROR: name and action params required");
      }
     });
server.on("/", HTTP_GET,[](AsyncWebServerRequest * request){
 if(!request->authenticate(username, pass))
      return request->requestAuthentication(); 
  request->send(SPIFFS,"/ptixiaki_main.html");
 });
server.on("/BootLoaderIndex", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if(!request->authenticate(username, pass))
      return request->requestAuthentication();   
    request->send(SPIFFS, "/ptixiaki_boot.html");
  });
server.on("/doUpdate", HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  [](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data,
     size_t len, bool final) {
    handleDoUpdate(request, filename, index, data, len, final);
  });
server.on("/update", HTTP_POST,
  [](AsyncWebServerRequest * request) {},
  [](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data,
     size_t len, bool final) {
    handleDoUpdate(request, filename, index, data, len, final);
  });
server.on("/mc", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send_P(200, "text/plain", readMAC().c_str());
  });
server.on("/ip", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send_P(200, "text/plain", readIP().c_str());
  });
server.on("/txt", HTTP_GET, [](AsyncWebServerRequest *request){
  
    request->send_P(200, "text/plain", readText().c_str());
  });
  server.on("/photo", HTTP_GET, [](AsyncWebServerRequest *request){
  
    request->send_P(200, "text/plain", readLDR().c_str());
  });
server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
 String inputMessage;   
 String inputMessage1;
    String message1= "[Message]" "\n";
    String message2= "Text= ";
    if (request->hasParam(PARAM_STRING1)){
      inputMessage1 = request->getParam(PARAM_STRING1)->value();
      inputMessage  = message1 +message2+inputMessage1 ;
      writeFile(SPIFFS, "/config.ini", inputMessage.c_str());
    }
     request->send(200, "text/text", inputMessage);
   });
   server.on("/restart1", HTTP_GET, [](AsyncWebServerRequest * request){
    if(!request->authenticate(username, pass))
      return request->requestAuthentication(); 
     request->send(200, "text/plain","restarting");
     Serial.println("Restarting ESP32.Please wait for 10 sec");
     delay(10000);
     ESP.restart();
  });
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request){
   if(!request->authenticate(username, pass))
      return request->requestAuthentication(); 
     request->send(200, "text/plain","restarting");
     Serial.println("Restarting ESP32.Please wait for 5 sec");
     delay(5000);
     ESP.restart();
     
  });
   server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(404);
  });
}
