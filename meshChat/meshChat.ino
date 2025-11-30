#include "App.h"
#include "Display.h"
#include "MessageBuffer.h"
#include "RandomGenerator.h"
#include "painlessMesh.h"

Scheduler scheduler;
painlessMesh mesh;
Display display;
MessageBuffer msgBuffer;
RandomGenerator randomGen;

App app(scheduler, mesh, display, msgBuffer, randomGen);

void setup() {
  app.setup();
}

void loop() {
  app.loop();
}
