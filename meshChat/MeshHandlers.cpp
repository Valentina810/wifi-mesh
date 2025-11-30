#include "MeshHandlers.h"
#include "App.h"

void MeshHandlers::onReceive(App& app, uint32_t from, String& msg) {
    Serial.printf("Received from %u: %s\n", from, msg.c_str());
    int sep = msg.indexOf('|');
    String fromName;
    String text;
    if (sep > 0) {
        fromName = msg.substring(0, sep);
        text = msg.substring(sep + 1);
    } else {
        fromName = String(from);
        text = msg;
    }
    String line = app.randomGen.getTimeString() + " " + fromName + ": " + text;
    app.msgBuffer.addWrapped(line);
    app.display.drawScreen(app.nodeName, app.msgBuffer);
}

void MeshHandlers::onNewConnection(uint32_t id) {
    Serial.printf("New connection, nodeId = %u\n", id);
}

void MeshHandlers::onChangedConnections() {
    Serial.printf("Changed connections\n");
}

void MeshHandlers::onNodeTimeAdjusted(painlessMesh& mesh, int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
