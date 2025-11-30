#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>
#include "painlessMesh.h"
#include "Display.h"
#include "MessageBuffer.h"
#include "RandomGenerator.h"

#define MESH_PREFIX     "ntldr"
#define MESH_PASSWORD   "5f325f79-c6ff-4792-b34c-6019d4070a72"
#define MESH_PORT       5555

class App {
public:
    App(Scheduler& scheduler,
        painlessMesh& mesh,
        Display& display,
        MessageBuffer& msgBuffer,
        RandomGenerator& randomGen);

    void setup();
    void loop();

    void sendMessageOnce();  

    String nodeName;

    Scheduler& scheduler;
    painlessMesh& mesh;
    Display& display;
    MessageBuffer& msgBuffer;
    RandomGenerator& randomGen;

private:
    unsigned long lastSendMs = 0;
    const unsigned long sendIntervalMs = 30 * 1000; 
};
