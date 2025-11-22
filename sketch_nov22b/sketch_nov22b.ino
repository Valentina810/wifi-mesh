#include "painlessMesh.h"

Scheduler userScheduler;
painlessMesh mesh;

#define   MESH_PREFIX     "ntldr"
#define   MESH_PASSWORD   "5f325f79-c6ff-4792-b34c-6019d4070a72"
#define   MESH_PORT       5555

#define NODE_NAME "NODE_1"

Task taskSendMessage(TASK_SECOND * 5, TASK_FOREVER, []() {
  String msg = String("Hello from ") + NODE_NAME;
  mesh.sendBroadcast(msg);
  Serial.println("Sent: " + msg);
});

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("Received from %u: %s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("New connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  userScheduler.execute();
  mesh.update();
}
