struct Config {
  // Websocket Server IP
  static const int server_port;
  static const String server_path;
  static const String server_host;

  // WiFi Connection
  static const char* ssid;
  static const char* pass;
  
};

const int Config::server_port = 25565;
const String Config::server_path = "/";
const String Config::server_host = "192.168.XX.XX";

const char* Config::ssid = "XXXX";
const char* Config::pass = "XXXX";
