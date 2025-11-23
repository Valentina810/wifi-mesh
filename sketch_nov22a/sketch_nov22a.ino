#include <SPI.h>
#include <TFT_eSPI.h>
#include "painlessMesh.h"

#define   MESH_PREFIX     "ntldr"
#define   MESH_PASSWORD   "5f325f79-c6ff-4792-b34c-6019d4070a72"
#define   MESH_PORT       5555

static const uint8_t MAX_LINES = 16;

class App {
public:

  static App* instance;
  Scheduler scheduler;
  painlessMesh mesh;
  Task taskSendMessage;
  TFT_eSPI tft;
  String nodeName;
  String messages[MAX_LINES];
  uint8_t messageCount = 0;

  String animals[13] = {
    "hedgehog", "lama", "cat", "frog", "tiger",
    "wolf", "fox", "bear", "otter", "eagle","hamster", "burrito", "bun"
  };

  String adjectives[10] = {
    "fat", "lazy", "soft", "yellow", "sleepy",
    "happy", "wild", "fast", "angry", "quiet"
  };

  App()
    : scheduler()
    , mesh()
    , taskSendMessage(TASK_SECOND * 30, TASK_FOREVER, &App::taskSendMessageCallback)
    , tft()
    , nodeName("")
  {}

  String generateRandomNodeName() {
    String animal = animals[random(0, 10)];
    String adj = adjectives[random(0, 10)];
    uint16_t num = random(1000,9999);
    return adj + "-" + animal + "-" + String(num);
  }

  void setup() {
    instance = this;

    Serial.begin(115200);
    delay(1000);

    tft.init();
    tft.setRotation(1);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    mesh.setDebugMsgTypes(ERROR | STARTUP);
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &scheduler, MESH_PORT);

    uint32_t seed = mesh.getNodeId() ^ millis();
    randomSeed(seed);
    nodeName = generateRandomNodeName();

    clearMessages();
    drawScreen();

    mesh.onReceive( [this](uint32_t from, String &msg) {
      this->onReceive(from, msg);
    });

    mesh.onNewConnection( [this](uint32_t nodeId) {
      this->onNewConnection(nodeId);
    });

    mesh.onChangedConnections( [this]() {
      this->onChangedConnections();
    });

    mesh.onNodeTimeAdjusted( [this](int32_t offset) {
      this->onNodeTimeAdjusted(offset);
    });

    scheduler.addTask(taskSendMessage);
    taskSendMessage.enable();
  }

  void loop() {
    scheduler.execute();
    mesh.update();
  }

  String getTimeString() {
    uint32_t sec = millis() / 1000;
    uint8_t h = (sec / 3600) % 24;
    uint8_t m = (sec / 60) % 60;
    uint8_t s = sec % 60;

    char buf[9];
    sprintf(buf, "%02u:%02u:%02u", h, m, s);
    return String(buf);
  }

  void clearMessages() {
    for (uint8_t i = 0; i < MAX_LINES; i++) {
      messages[i] = "";
    }
    messageCount = 0;
  }

  void addMessage(const String& line) {
    if (messageCount < MAX_LINES) {
      messages[messageCount++] = line;
    } else {
      for (uint8_t i = 1; i < MAX_LINES; i++) {
        messages[i - 1] = messages[i];
      }
      messages[MAX_LINES - 1] = line;
    }
  }

  void addWrappedMessage(const String& line) {
    const uint8_t maxWidth = 50;
    int len = line.length();
    int start = 0;

    while (start < len) {
      int end = start + maxWidth;
      if (end > len) end = len;
      addMessage(line.substring(start, end));
      start = end;
    }
  }

  void drawScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.println("Node: "+nodeName);
    tft.println("----------------");

    int y = tft.getCursorY() + 2;

    for (uint8_t i = 0; i < messageCount; i++) {
      tft.setCursor(0, y);
      tft.println(messages[i]);
      y += 16;
    }
  }

  String generateRandomText() {
    int len = random(20, 150);
    String result = "";

    for (int i = 0; i < len; i++) {
      int r = random(0, 100);

      if (r < 15 && result.length() > 0 && result[result.length() - 1] != ' ') {
        result += ' ';
      } else {
        char c = 'a' + random(0, 26);
        result += c;
      }
    }

    while (result.length() > 0 && result[result.length() - 1] == ' ') {
      result.remove(result.length() - 1);
    }

    return result;
  }

  void sendMessageTask() {
    String payloadText = generateRandomText();
    String msg = nodeName + "|" +payloadText;

    mesh.sendBroadcast(msg);
    Serial.println("Sent: " + msg);

    String line = getTimeString() + "  " + nodeName + ": "+payloadText;
    addWrappedMessage(line);
    drawScreen();
  }

  void onReceive(uint32_t from, String &msg) {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());
    int sep = msg.indexOf('|');
    String fromName;
    String text;
    if (sep> 0) {
      fromName = msg.substring(0, sep);
      text = msg.substring(sep + 1);
    } else {
      fromName = String(from);
      text = msg;
    }
    String line = getTimeString() + " " + fromName + ": " + text;
    addWrappedMessage(line);
    drawScreen();
  }

  void onNewConnection(uint32_t nodeId) {
    Serial.printf("New connection, nodeId = %u\n", nodeId);
  }

  void onChangedConnections() {
    Serial.printf("Changed connections\n");
  }

  void onNodeTimeAdjusted(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
  }

  static void taskSendMessageCallback() {
    if (instance != nullptr) {
      instance->sendMessageTask();
    }
  }
};

App* App::instance = nullptr;
App app;

void setup() {
  app.setup();
}

void loop() {
  app.loop();
}
