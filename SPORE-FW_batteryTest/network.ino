void networkInit() {
  /* connect to WiFi */
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.persistent(false);                   // don't re-write ssid/password to flash every time (avoid degredation)
  WiFi.mode(WIFI_STA);                      //
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);       // BRICKS OTA?? or line 282??   // disable auto modem-sleep
  WiFi.hostname(deviceName);                // DHCP Hostname
  WiFi.begin(ssid, password);               // connect to WiFi

  /* auto reset if it's not connecting (occasionally hangs otherwise) */
  int restartCounter = 0;
  while (!WiFi.isConnected()) {
    delay(100); Serial.print("."); restartCounter++;
    if (restartCounter > 50) ESP.restart();  // if it takes more than ~5 seconds to connect, restart!
  }
  Serial.println("  connected.");            // yay it worked!
  address = WiFi.localIP()[3];                     // get address
  if (address < 10) deviceName.concat("00"); else if (address < 100) deviceName.concat("0"); deviceName.concat(address);    // add 3 digit address to deviceName
  //MDNS.begin(deviceName);
  yield();

  /* initialize OSC */
  //oscAddress = "/" + deviceName;
  Serial.print("\nOpening UDP port for OSC...");
  if (udp.beginMulticast(WiFi.localIP(), mIP, mPort)) {
      Serial.printf("listening on multicast group: ");
      Serial.print(mIP);
      Serial.printf(":%u\n", mPort);
  }
  else {
    Serial.printf("(could not join multicast group at: ");
    Serial.print(mIP);
    Serial.printf(":%u\n", mPort);
  }

  /* setup websockets */
  if (ENABLE_WEBSOCKET) {
    webSocket.begin(serverIP.toString(), wsPort, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
  }

  /* who am I this time?  */
  delay(100);
  //Serial.printf("\nWiFi connected.\n");
  Serial.printf("\nIP address:  ");
  Serial.println(WiFi.localIP());
  Serial.printf("OSC Address: %s\n", oscAddress.c_str());
  if (ENABLE_WEBSOCKET) Serial.printf("Websocket looking for server at: %s:%u\n", serverIP.toString().c_str(), wsPort);
  Serial.printf("%s (%s) ready. \n", deviceName.c_str(), WiFi.macAddress().c_str());
  Serial.printf("\n---\n\n");
  Serial.flush();
}

