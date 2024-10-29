#include <Arduino.h>
#include <nixie.h>

int interval = 1000 / 2 / 4;

bool loop_antipoisoning(){
    for (int z = 0; z < 4; z++){
        for (int i = 0; i < 10; i++){
            displayRepeatingDigit(i);

            delay(interval);
        }

        for (int i = 9; i >= 0; i--) {
            displayRepeatingDigit(i);

            delay(interval);
        }
    }
    

    return true;
}

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