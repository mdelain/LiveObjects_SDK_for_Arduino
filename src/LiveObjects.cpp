#define DO_NOT_DEFINE_LO
#include "LiveObjects.h"


LiveObjectsBase::LiveObjectsBase()
    :
     m_bDebug(false)
    ,m_bInitialized(false)
    ,m_bInitialMqttConfig(false)
    ,m_bCertLoaded(false)
    ,m_pClient(nullptr)
    ,m_pMqttclient(nullptr)
{
}
LiveObjectsBase::~LiveObjectsBase()
{
  delete m_pMqttclient;
  delete m_pClient;
}


/******************************************************************************
   PARAM TYPERS
 ******************************************************************************/

void LiveObjectsBase::paramTyper(const String& name, bool* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, UNSIGNED_INTEGER, T_BOOL, callback);
  else
    addTypedParam(name, variable, type, T_BOOL, callback);
}
void LiveObjectsBase::paramTyper(const String& name, char* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, INTEGER, T_CHAR, callback);
  else
    addTypedParam(name, variable, type, T_CHAR, callback);
}
void LiveObjectsBase::paramTyper(const String& name, int* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, INTEGER, T_INT, callback);
  else
    addTypedParam(name, variable, type, T_INT, callback);
}
void LiveObjectsBase::paramTyper(const String& name, int8_t*variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, INTEGER, T_INT8, callback);
  else
    addTypedParam(name, variable, type, T_INT8, callback);
}
void LiveObjectsBase::paramTyper(const String& name, int16_t* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, INTEGER, T_INT16, callback);
  else
    addTypedParam(name, variable, type, T_INT16, callback);
}
void LiveObjectsBase::paramTyper(const String& name, int32_t* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, INTEGER, T_INT32, callback);
  else
    addTypedParam(name, variable, type, T_INT32, callback);
}
void LiveObjectsBase::paramTyper(const String& name, unsigned int* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, UNSIGNED_INTEGER, T_UINT, callback);
  else
    addTypedParam(name, variable, type, T_UINT, callback);
}
void LiveObjectsBase::paramTyper(const String& name, uint8_t* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, UNSIGNED_INTEGER, T_UINT8, callback);
  else
    addTypedParam(name, variable, type, T_UINT8, callback);
}
void LiveObjectsBase::paramTyper(const String& name, uint16_t* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, UNSIGNED_INTEGER, T_UINT16, callback);
  else
    addTypedParam(name, variable, type, T_UINT16, callback);
}
void LiveObjectsBase::paramTyper(const String& name, uint32_t* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, UNSIGNED_INTEGER, T_UINT32, callback);
  else
    addTypedParam(name, variable, type, T_UINT32, callback);
}
void LiveObjectsBase::paramTyper(const String& name, double* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, DECIMAL, T_DOUBLE, callback);
  else
    addTypedParam(name, variable, type, T_DOUBLE, callback);
}
void LiveObjectsBase::paramTyper(const String& name, float* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, DECIMAL, T_FLOAT, callback);
  else
    addTypedParam(name, variable, type, T_FLOAT, callback);
}
void LiveObjectsBase::paramTyper(const String& name, String* variable, LiveObjects_parameterType type, onParameterUpdateCallback callback) {
  if (type == IMPLICIT)
    addTypedParam(name, variable, STRING, T_STRING, callback);
  else
    addTypedParam(name, variable, type, T_STRING, callback);
}
void LiveObjectsBase::ptrTyper(const LiveObjects_parameter param, const JsonDocument& configIn, JsonDocument& configOut) {
  switch (param.variableType) {
    case T_BOOL:
      updateParameter(param, (bool*)param.value, configIn, configOut);
      break;
    case T_CHAR:
      updateParameter(param, (char*)param.value, configIn, configOut);
      break;
    case T_INT:
      updateParameter(param, (int*)param.value, configIn, configOut);
      break;
    case T_INT8:
      updateParameter(param, (int8_t*)param.value, configIn, configOut);
      break;
    case T_INT16:
      updateParameter(param, (int16_t*)param.value, configIn, configOut);
      break;
    case T_INT32:
      updateParameter(param, (int32_t*)param.value, configIn, configOut);
      break;
    case T_UINT:
      updateParameter(param, (unsigned int*)param.value, configIn, configOut);
      break;
    case T_UINT8:
      updateParameter(param, (uint8_t*)param.value, configIn, configOut);
      break;
    case T_UINT16:
      updateParameter(param, (uint16_t*)param.value, configIn, configOut);
      break;
    case T_UINT32:
      updateParameter(param, (uint32_t*)param.value, configIn, configOut);
      break;
    case T_DOUBLE:
      updateParameter(param, (double*)param.value, configIn, configOut);
      break;
    case T_FLOAT:
      updateParameter(param, (float*)param.value, configIn, configOut);
      break;
    case T_STRING:
      String* ptr = (String*)param.value;
      if (!configIn.isNull())
      *ptr = configIn[JSONCFG][param.label][JSONCFGVALUE].as<String>();
      configOut[JSONCFG][param.label][JSONCFGVALUE] = (*ptr).c_str();
      break;
  }
}


/******************************************************************************
   CONFIGURATION MANAGER
 ******************************************************************************/

void LiveObjectsBase::configurationManager(int messageSize) {
  StaticJsonDocument<PAYLOAD_DEVMGT_SIZE> configOut;
  if (messageSize >= 0)
  { // config update received
    StaticJsonDocument<PAYLOAD_DEVMGT_SIZE> configIn;
    deserializeJson(configIn, *m_pMqttclient);

    serializeJsonPretty(configIn, Serial);
    outputDebug('\n');

    configOut=configIn;
    JsonObject obj =  configOut[JSONCFG];
    LiveObjects_parameter tmp("",nullptr,BINARY,T_BOOL,nullptr);
    for(auto el : obj)
    {
      tmp.label = el.key().c_str();
      int index = parameters.find(&tmp);
      if(index > -1)
      {
        LiveObjects_parameter* p = parameters[index];
        ptrTyper(*p, configIn, configOut);
        if (p->callback != NULL) p->callback();
      }
      else
      {
        configOut["cfg"][el.key()]["v"]="Nan";
        configOut["cfg"][el.key()]["t"]="str";
      }
    }
    publishMessage(MQTT_PUBCFG, configOut);
  }
  else if(parameters.size()>0 && messageSize == -1)
  { // messageSize==-1, compose & send initial config
    StaticJsonDocument<0> configIn;
    for (uint8_t i = 0; i < parameters.size(); i++) {
      switch (parameters[i]->type) {
        case INTEGER:
          configOut[JSONCFG][parameters[i]->label.c_str()][JSONCFGTYPE] = F("i32");
          break;
        case UNSIGNED_INTEGER:
          configOut[JSONCFG][parameters[i]->label.c_str()][JSONCFGTYPE] = F("u32");
          break;
        case STRING:
          configOut[JSONCFG][parameters[i]->label.c_str()][JSONCFGTYPE] = F("str");
          break;
        case BINARY:
          configOut[JSONCFG][parameters[i]->label.c_str()][JSONCFGTYPE] = F("bin");
          break;
        case DECIMAL:
          configOut[JSONCFG][parameters[i]->label.c_str()][JSONCFGTYPE] = F("f64");
          break;
      }
      ptrTyper(*parameters[i], configIn, configOut);
    }
    publishMessage(MQTT_PUBCFG, configOut);
  }
}

/******************************************************************************
   COMMAND MANAGEMENT
 ******************************************************************************/

void LiveObjectsBase::addCommand(const String name, onCommandCallback callback) {
  commands.push(new LiveObjects_command(name, callback));
}

void LiveObjectsBase::commandManager() {
  StaticJsonDocument<PAYLOAD_DEVMGT_SIZE> cmdIn;
  StaticJsonDocument<PAYLOAD_DEVMGT_SIZE> cmdOut;
  deserializeJson(cmdIn, *m_pMqttclient);

  serializeJsonPretty(cmdIn, Serial);
  outputDebug('\n');
  for (uint8_t i = 0; i < commands.size(); i++) // only with MQTT or SMS !!
    if (cmdIn[F("req")] == commands[i]->label) {
      cmdOut[JSONCID] = cmdIn[JSONCID];
      String response;
      commands[i]->callback(cmdIn[F("arg")].as<String>(), response);
      if (response.length() != 0)
        cmdOut[F("res")] = serialized(response);
      break;
    }
  publishMessage(MQTT_PUBCMD, cmdOut);
}

/******************************************************************************
   CONNECTION MANAGER
 ******************************************************************************/

void LiveObjectsBase::connect()
{
  connectNetwork();
  connectMQTT();
}

void LiveObjectsBase::networkCheck() {
  unsigned long now = millis();
  if (now - lastKeepAliveNetwork > KEEP_ALIVE_NETWORK) {
    checkNetwork();
    checkMQTT();
    lastKeepAliveNetwork = now;
  }
}

void LiveObjectsBase::disconnect() {
  disconnectMQTT();
  disconnectNetwork();
  outputDebug("Offline.\n");
  networkStatus = DISCONNECTED;
}

/******************************************************************************
   MQTT FUNCTIONS
 ******************************************************************************/

void LiveObjectsBase::onMQTTmessage(int messageSize) {
  String topic = m_pMqttclient->messageTopic();
  outputDebug("Received a message on topic '");
  outputDebug(topic);

  if (topic == MQTT_SUBCFG)
    configurationManager(messageSize);
  else if (topic == MQTT_SUBCMD)
    commandManager();
}

void LiveObjectsBase::sendData() {
  easyDataPayload[JSONMODEL] = JSONMODELNAME;
  publishMessage(MQTT_PUBDATA, easyDataPayload);
  easyDataPayload.clear();
}

void LiveObjectsBase::sendData(const String customPayload) {
  StaticJsonDocument<PAYLOAD_DATA_SIZE> payload;
  deserializeJson(payload, customPayload);
  if (!payload.containsKey(JSONMODEL))
    payload[JSONMODEL] = JSONMODELNAME;
  publishMessage(MQTT_PUBDATA, payload);
}

void LiveObjectsBase::publishMessage(const String& topic, JsonDocument& payload) {
  outputDebug("Publishing message");
  outputDebug(topic);
  serializeJsonPretty(payload, Serial);

  if( measureJson(payload) >= PAYLOAD_DATA_SIZE )
  {
    outputDebug("Message size is to big, aborting send command.");
    return;
  }

  m_pMqttclient->beginMessage(topic);
  serializeJson(payload, *m_pMqttclient);
  m_pMqttclient->endMessage();
}

/******************************************************************************
   LIVE OBJECTS
 ******************************************************************************/

void LiveObjectsBase::loop() {
  if (networkStatus == CONNECTED) {
    networkCheck();
    m_pMqttclient->poll();
  }
}
void LiveObjectsBase::setProtocol(Protocol p)
{
//TODO
}
void LiveObjectsBase::setSecurity(Security s)
{
//TODO
}
void LiveObjectsBase::enableDebug(bool b)
{
  m_bDebug = b;
}
void LiveObjectsBase::setClientID(const String id)
{
  m_sMqttid = id;
}
void LiveObjectsBase::addTimestamp(time_t timestamp)
{
  timestamp-=504921600;
  char bufer[sizeof("2011-10-08T07:07:09Z")];
  strftime(bufer, sizeof(bufer), "%Y-%m-%dT%H:%M:%SZ",gmtime(&timestamp));
  easyDataPayload["timestamp"]=bufer;
}
void LiveObjectsBase::addLocation(double lat, double lon, float alt)
{
  JsonObject obj = easyDataPayload.createNestedObject("location");
  obj["lat"] = lat;
  obj["lon"] = lon;
  obj["alt"] = alt;
}

void LiveObjectsBase::clearPayload()
{
  easyDataPayload.clear();
}

bool LiveObjectsBase::debugEnabled()
{
  return m_bDebug;
}

void LiveObjectsBase::checkMQTT()
{
  if(!m_pMqttclient->connected())
    connectMQTT();
}
void LiveObjectsBase::connectMQTT()
{
  if (sizeof(SECRET_LIVEOBJECTS_API_KEY) != 33) // check if API key seems correct
  {
    outputDebug("Please check your Live Objects API key (arduino_secrets.h file).\nStopping here.");
    while (1) ;
  }

  if (!m_bInitialMqttConfig) {
    m_pMqttclient->setTxPayloadSize(PAYLOAD_DATA_SIZE);
    m_pMqttclient->setId(m_sMqttid.c_str());
    m_pMqttclient->setUsernamePassword(MQTT_USER, SECRET_LIVEOBJECTS_API_KEY);
  }

  outputDebug("Connecting to MQTT broker '");
  outputDebug(MQTT_BROKER);
  outputDebug(m_nPort);


  while (!m_pMqttclient->connect(MQTT_BROKER, m_nPort)) outputDebug(".");
  outputDebug("\nYou're connected to the MQTT broker");

  networkStatus = CONNECTED;
  m_pMqttclient->subscribe(MQTT_SUBCFG);
  m_pMqttclient->subscribe(MQTT_SUBCMD);

  m_pMqttclient->poll();

  if (!m_bInitialMqttConfig) {
    configurationManager();
    m_bInitialMqttConfig = true;
  }

}
void LiveObjectsBase::disconnectMQTT()
{
  outputDebug("\nClosing MQTT connection...");
  m_pMqttclient->stop();
}









 /******************************************************************************
   GSM CLASS
 ******************************************************************************/
#ifdef ARDUINO_SAMD_MKRNB1500
LiveObjectsGSM::LiveObjectsGSM()
  :
   LiveObjectsBase()
  ,m_NBAcces()
{}

LiveObjectsGSM::~LiveObjectsGSM()
{}

void LiveObjectsGSM::begin(Protocol p, Security s, bool bDebug)
{
  m_bDebug = bDebug;
  switch(s)
  {
    case TLS:
    m_pClient = new NBSSLClient();
    m_pMqttclient = new MqttClient(m_pClient);
    m_nPort = 8883;
    break;
    case NONE:
    m_pClient = new NBClient();
    m_pMqttclient = new MqttClient(m_pClient);
    m_nPort = 1883;
    break;
    default:
    outputDebug("Wrong security type!");
  }
  m_bInitialized = true;
  m_pMqttclient->onMessage(messageCallback);
}

void LiveObjectsGSM::connectNetwork()
{
  //Set client id as IMEI
  if (!m_bInitialized)
  {
    outputDebug("missing begin() call, calling with default protcol=MQTT, security protcol=TLS, debug=true");
    begin();
  }

  NBModem modem;
  if(modem.begin())
  {
    if(m_sMqttid.length()==0)
    {
      String imei="";
      for(int i=1;i<=3;i++)
      {
        imei=modem.getIMEI();
        if(imei.length()!=0) break;
        delay(100*i);
      }
      m_sMqttid = imei;
    }
  }
  else
  {
    outputDebug("Failed to initialize modem!");
    while(true){}
  }

   outputDebug("Connecting to cellular network");
   while (m_NBAcces.begin(SECRET_PINNUMBER, SECRET_APN, SECRET_APN_USER, SECRET_APN_PASS) != NB_READY)
     outputDebug(".");
  outputDebug("\nYou're connected to the network");

  if(m_nPort==8883){
    if (!m_bCertLoaded) {
      outputDebug("Loading DigiCert Root CA certificate");
      MODEM.sendf("AT+USECMNG=0,0,\"%s\",%d", LO_ROOT_CERT.name, LO_ROOT_CERT.size);
      if (MODEM.waitForPrompt() != 1) {
        outputDebug("Problem loading certificate!\nStopping here.");
        while (1) ;
      }
      else {
        MODEM.write(LO_ROOT_CERT.data, LO_ROOT_CERT.size);
        int ready;
        while (!MODEM.ready()) ;
        m_bCertLoaded = true;
        outputDebug("Certificate loaded");
      }
    }
  }
}

void LiveObjectsGSM::checkNetwork()
{
  if(m_NBAcces.status()!= NB_READY)
    connectNetwork();
}

void LiveObjectsGSM::disconnectNetwork()
{
  outputDebug("Disconnecting from cellular network...");
  m_NBAcces.shutdown();
}

void LiveObjectsGSM::messageCallback(int msg)
{
  LiveObjects::get().onMQTTmessage(msg);
};

void LiveObjectsGSM::addNetworkInfo()
{
  JsonObject obj = easyDataPayload[JSONVALUE].createNestedObject("networkInfo");
  obj["connection_status"] = m_NBAcces.status() == NB_NetworkStatus_t::NB_READY ? "connected":"disconnected";
  obj["strength"] = m_NBScanner.getSignalStrength();
  obj["carrier"]=m_NBScanner.getCurrentCarrier();
}

#endif
 /******************************************************************************
   WiFi CLASS
 ******************************************************************************/
#ifdef WIFI
LiveObjectsWiFi::LiveObjectsWiFi()
  :
   LiveObjectsBase()
   ,m_sMac()
{
}
LiveObjectsWiFi::~LiveObjectsWiFi()
{}



void LiveObjectsWiFi::begin(Protocol p, Security s, bool bDebug)
{
  m_bDebug = bDebug;
  switch(s)
  {
    case TLS:
    m_pClient = new WiFiSSLClient();
    m_pMqttclient = new MqttClient(m_pClient);
    m_nPort = 8883;
    break;
    case NONE:
    m_pClient = new WiFiClient();
    m_pMqttclient = new MqttClient(m_pClient);
    m_nPort = 1883;
    break;
    default:
    outputDebug("Wrong security type! Stopping...");
    while(true);
  }

  uint8_t mac[6];
  char buff[10];
  WiFi.macAddress(mac);
  for(int i=0;i<6;++i)
  {
    memset(buff,'\0',10);
    itoa(mac[i],buff,16);
    m_sMac += buff;
    if(i!=5) m_sMac += ':';
  }
  m_sMqttid = m_sMac;
  m_bInitialized = true;
  m_pMqttclient->onMessage(messageCallback);
}

void LiveObjectsWiFi::connectNetwork()
{
  if(!m_bInitialized)
  {
    outputDebug("missing begin() call, calling with default protcol=MQTT, security protcol=TLS, debug=true");
    begin();
  }
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  outputDebug("WiFi firmware version ");
  outputDebug(WiFi.firmwareVersion());

  outputDebug("Attempting to connect to SSID: ");
  outputDebug(SECRET_SSID);
  outputDebug(" ");

  while (WiFi.begin(SECRET_SSID, SECRET_WIFI_PASS) != WL_CONNECTED) {
    // failed, retry
    outputDebug(".");
    delay(3000);
  }


}
void LiveObjectsWiFi::checkNetwork()
{
  if(WiFi.status()== WL_DISCONNECTED)
    connectNetwork();
}
void LiveObjectsWiFi::disconnectNetwork()
{
  outputDebug("Disconnecting WiFi");
  WiFi.disconnect();
}
void LiveObjectsWiFi::messageCallback(int msg)
{
  LiveObjects::get().onMQTTmessage(msg);
}

void LiveObjectsWiFi::addNetworkInfo()
{
  JsonObject obj = easyDataPayload[JSONVALUE].createNestedObject("networkInfo");
  IPAddress ip = WiFi.localIP();
  obj["mac"] = m_sMac;
  obj["ssid"] = SECRET_SSID;
  String addres;
  for(int i=0;i<4;++i)
  {
    addres+=ip[i];
    if(i!=3) addres+='.';
  }
  obj["ip"] = addres;
  addres = WiFi.RSSI();
  addres += " dbm";
  obj["strength"] = addres;

}
#endif