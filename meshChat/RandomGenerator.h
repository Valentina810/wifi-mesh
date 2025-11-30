#pragma once
#include <Arduino.h>

class RandomGenerator {
public:
    String generateRandomNodeName();
    String generateRandomText();
    String getTimeString();
};
