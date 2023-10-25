#if LOG_TIMESTAMP
  #define LOG_LOGTAG(NAME, LVL) Serial.printf("[" NAME "][" LVL "][%10lu] ", millis())
#else
  #define LOG_LOGTAG(NAME, LVL) Serial.print("[" NAME "][" LVL "] ")
#endif

/* OLED LOGGING */
#if OLED_LEVEL > 0
  #define OLED_LOGE(...) LOG_LOGTAG("OLED","ERROR");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define OLED_LOGE(...) do {} while (0)
#endif

#if OLED_LEVEL > 1
  #define OLED_LOGW(...) LOG_LOGTAG("OLED","WARNNING");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define OLED_LOGW(...) do {} while (0)
#endif

#if OLED_LEVEL > 2
  #define OLED_LOGI(...) LOG_LOGTAG("OLED","INFO");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define OLED_LOGI(...) do {} while (0)
#endif

#if OLED_LEVEL > 3
  #define OLED_LOGD(...) LOG_LOGTAG("OLED","DEBUG");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define OLED_LOGD(...) do {} while (0)
#endif

/* WEBSOCKET LOGGING */
#if WEBSOCKET_LEVEL > 0
  #define WEBSOCKET_LOGE(...) LOG_LOGTAG("WEBSOCKET","ERROR");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WEBSOCKET_LOGE(...) do {} while (0)
#endif

#if WEBSOCKET_LEVEL > 1
  #define WEBSOCKET_LOGW(...) LOG_LOGTAG("WEBSOCKET","WARNNING");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WEBSOCKET_LOGW(...) do {} while (0)
#endif

#if WEBSOCKET_LEVEL > 2
  #define WEBSOCKET_LOGI(...) LOG_LOGTAG("WEBSOCKET","INFO");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WEBSOCKET_LOGI(...) do {} while (0)
#endif

#if WEBSOCKET_LEVEL > 3
  #define WEBSOCKET_LOGD(...) LOG_LOGTAG("WEBSOCKET","DEBUG");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WEBSOCKET_LOGD(...) do {} while (0)
#endif

/* WATERFLOW LOGGING */
#if WATERFLOW_LEVEL > 0
  #define WATERFLOW_LOGE(...) LOG_LOGTAG("WATERFLOW","ERROR");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WATERFLOW_LOGE(...) do {} while (0)
#endif

#if WATERFLOW_LEVEL > 1
  #define WATERFLOW_LOGW(...) LOG_LOGTAG("WATERFLOW","WARNNING");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WATERFLOW_LOGW(...) do {} while (0)
#endif

#if WATERFLOW_LEVEL > 2
  #define WATERFLOW_LOGI(...) LOG_LOGTAG("WATERFLOW","INFO");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WATERFLOW_LOGI(...) do {} while (0)
#endif

#if WATERFLOW_LEVEL > 3
  #define WATERFLOW_LOGD(...) LOG_LOGTAG("WATERFLOW","DEBUG");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define WATERFLOW_LOGD(...) do {} while (0)
#endif

/* TEMPERATURE LOGGING */
#if TEMPERATURE_LEVEL > 0
  #define TEMPERATURE_LOGE(...) LOG_LOGTAG("TEMPERATURE","ERROR");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define TEMPERATURE_LOGE(...) do {} while (0)
#endif

#if TEMPERATURE_LEVEL > 1
  #define TEMPERATURE_LOGW(...) LOG_LOGTAG("TEMPERATURE","WARNNING");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define TEMPERATURE_LOGW(...) do {} while (0)
#endif

#if TEMPERATURE_LEVEL > 2
  #define TEMPERATURE_LOGI(...) LOG_LOGTAG("TEMPERATURE","INFO");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define TEMPERATURE_LOGI(...) do {} while (0)
#endif

#if TEMPERATURE_LEVEL > 3
  #define TEMPERATURE_LOGD(...) LOG_LOGTAG("TEMPERATURE","DEBUG");Serial.printf(__VA_ARGS__);Serial.println()
#else
  #define TEMPERATURE_LOGD(...) do {} while (0)
#endif