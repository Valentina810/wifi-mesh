#include "App.h"
#include "MeshHandlers.h"

App::App(Scheduler& scheduler,
         painlessMesh& mesh,
         Display& display,
         MessageBuffer& msgBuffer,
         RandomGenerator& randomGen)
    : scheduler(scheduler)
    , mesh(mesh)
    , display(display)
    , msgBuffer(msgBuffer)
    , randomGen(randomGen)
    , nodeName("")
    , lastSendMs(0)
{}

void App::setup() {
    Serial.begin(115200);
    delay(1000);

    display.init();
    display.drawStartupImage();

    mesh.setDebugMsgTypes(ERROR | STARTUP);
    mesh.init(MESH_PREFIX, MESH_PASSWORD, &scheduler, MESH_PORT);

    uint32_t seed = mesh.getNodeId() ^ millis();
    randomSeed(seed);

    nodeName = randomGen.generateRandomNodeName();

    msgBuffer.clear();
    display.drawScreen(nodeName, msgBuffer);

    mesh.onReceive([this](uint32_t f, String &m) {
        MeshHandlers::onReceive(*this, f, m);
    });

    mesh.onNewConnection([](uint32_t id) {
        MeshHandlers::onNewConnection(id);
    });

    mesh.onChangedConnections([]() {
        MeshHandlers::onChangedConnections();
    });

    mesh.onNodeTimeAdjusted([this](int32_t off) {
        MeshHandlers::onNodeTimeAdjusted(mesh, off);
    });
}

void App::loop() {
    scheduler.execute();
    mesh.update();

    unsigned long now = millis();
    if (now - lastSendMs >= sendIntervalMs) {
        lastSendMs = now;
        sendMessageOnce();
    }
}

void App::sendMessageOnce() {
    String payloadText = randomGen.generateRandomText();
    String msg = nodeName + "|" + payloadText;

    mesh.sendBroadcast(msg);
    Serial.println("Sent: " + msg);

    String line = randomGen.getTimeString() + "  " + nodeName + ": " + payloadText;
    msgBuffer.addWrapped(line);
    display.drawScreen(nodeName, msgBuffer);
}
