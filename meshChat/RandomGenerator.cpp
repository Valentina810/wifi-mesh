#include "RandomGenerator.h"

String animalsList[13] = {
    "hedgehog", "lama", "cat", "frog", "tiger",
    "wolf", "fox", "bear", "otter", "eagle", "hamster", "burrito", "bun"
};

String adjectivesList[10] = {
    "fat", "lazy", "soft", "yellow", "sleepy",
    "happy", "wild", "fast", "angry", "quiet"
};

String RandomGenerator::generateRandomNodeName() {
    String animal = animalsList[random(0, 10)];
    String adj = adjectivesList[random(0, 10)];
    uint16_t num = random(1000,9999);
    return adj + "-" + animal + "-" + String(num);
}

String RandomGenerator::generateRandomText() {
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

String RandomGenerator::getTimeString() {
    uint32_t sec = millis() / 1000;
    uint8_t h = (sec / 3600) % 24;
    uint8_t m = (sec / 60) % 60;
    uint8_t s = sec % 60;

    char buf[9];
    sprintf(buf, "%02u:%02u:%02u", h, m, s);
    return String(buf);
}
