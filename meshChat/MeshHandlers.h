#pragma once
#include <Arduino.h>
#include "painlessMesh.h"

class App;

class MeshHandlers {
public:
    static void onReceive(App& app, uint32_t from, String& msg);
    static void onNewConnection(uint32_t id);
    static void onChangedConnections();
    static void onNodeTimeAdjusted(painlessMesh& mesh, int32_t offset);
};
