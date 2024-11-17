#include <Arduino.h>
#include <nixie.h>

static int interval = 1000 / 2 / 4;

static int currentDigit;                // Chiffre actuel à afficher
static int direction;                   // Direction (1 pour incrémenter, -1 pour décrémenter)
static int cycleCount;                  // Compteur de cycles complets (4 au total)
static unsigned long previousMillis;    // Dernier temps enregistré

void init_antipoisoning(){
    cycleCount = 0;
    direction = 1;
    currentDigit = 0;
    previousMillis = 0;
}

bool loop_antipoisoning() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        currentDigit += direction;

        if (currentDigit > 9) {
            currentDigit = 9;
            direction = -1; // Descending order direction
        } else if (currentDigit < 0) {
            currentDigit = 0;
            direction = 1; // Ascending order direction
            cycleCount++;
        }
    }

    displayRepeatingDigit(currentDigit);

    if (cycleCount >= 4) {
        return true;
    }

    return false;
}



//Another antipoisoning pattern

/*
#include <Arduino.h>
#include <nixie.h>

bool loop_antipoisoning(){
    for (int z = 0; z < 170; z++){
        for (int i = 0; i < 10; i++){
            displayRepeatingDigit(i);

            delay(6);
        }
    }

    return true;
}

*/