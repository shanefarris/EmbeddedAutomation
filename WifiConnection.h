#ifndef wifi_connection_h
#define wifi_connection_h

class SimpleTimer;
class ESP8266WiFiClass;

class WifiConnection
{
private:
	uint16_t          attempt = 0;
	bool              _isStandAlone;
	SimpleTimer*      _timer = nullptr;
	int               _connectTimer = 0;
	ESP8266WiFiClass* _wifi;
	static WifiConnection* _instance;

	WifiConnection();
	bool setupAp();
	bool setupClient();
	void array_to_string(byte array[], unsigned int len, char buffer[]);

	static void tryNetworkConnection();

public:
	static WifiConnection* Instance();
	bool isStandAlone();
	bool setup();
	bool isConnected();
	String getIp();
	String getMac();
	void disconnect();

};

#endif // wifi_connection_h