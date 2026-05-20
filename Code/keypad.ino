/* * PROJECT: Mechatronics Memory Game
 * SHEET: keypads.ino
 * PURPOSE: Manages physical button interactions, edge detection, 
 * and mechanical debouncing for two independent player keypads.
 */

#include "hardware.h"
#include "config.h"

// Mapping logic to hardware bits defined in hardware.h
int btnsP1[] = P1_BTN_ARRAY;
int btnsP2[] = P2_BTN_ARRAY;

// State tracking for Synchronous (Non-blocking) Mode
uint8_t p1LastState[4] = {1, 1, 1, 1}; 
uint8_t p2LastState[4] = {1, 1, 1, 1};
unsigned long p1LastDebounceTime[4] = {0, 0, 0, 0};
unsigned long p2LastDebounceTime[4] = {0, 0, 0, 0};

// ================================================================
// INITIALIZATION
// ================================================================

void intializeP1Keypad() {
    for(int i = 0; i < NUM_OF_LEDS; i++) {
        P1_BTN_DDR &= ~(1 << btnsP1[i]); // Set as Input
        P1_BTN_PORT |= (1 << btnsP1[i]);  // Enable Internal Pull-up Resistor
    }
}

void intializeP2Keypad() {
    for(int i = 0; i < NUM_OF_LEDS; i++) {
        P2_BTN_DDR &= ~(1 << btnsP2[i]); // Set as Input
        P2_BTN_PORT |= (1 << btnsP2[i]);  // Enable Internal Pull-up Resistor
    }
}

// ================================================================
// NON-BLOCKING INPUT (Used for Dual/Race Mode)
// ================================================================
/**
 * Detects a "Falling Edge" (Button Press) without stopping the CPU.
 * This allows the Arduino to monitor multiple players at once.
 */
int getP1Instant() {
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        uint8_t currentState = (P1_BTN_PIN & (1 << btnsP1[i])) ? 1 : 0;
        unsigned long currentTime = millis();

        // FALLING EDGE DETECTION: Last state was HIGH (released), current is LOW (pressed)
        if (p1LastState[i] == 1 && currentState == 0) { 
            // TIME-BASED DEBOUNCE: Check if enough time passed since last press
            if ((currentTime - p1LastDebounceTime[i]) > DEBOUNCE_DELAY) {
                p1LastState[i] = 0;
                p1LastDebounceTime[i] = currentTime; 
                return i; // Return the button index immediately
            }
        }
        p1LastState[i] = currentState;
    }
    return -1;
}

int getP2Instant() {
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        uint8_t currentState = (P2_BTN_PIN & (1 << btnsP2[i])) ? 1 : 0;
        unsigned long currentTime = millis();

        if (p2LastState[i] == 1 && currentState == 0) {
            if ((currentTime - p2LastDebounceTime[i]) > DEBOUNCE_DELAY) {
                p2LastState[i] = 0;
                p2LastDebounceTime[i] = currentTime;
                return i;
            }
        }
        p2LastState[i] = currentState;
    }
    return -1;
}

// ================================================================
// BLOCKING INPUT (Used for Solo/Challenger Mode)
// ================================================================
/**
 * Waits for a button to be pressed AND released.
 * Simple and reliable for single-player turns.
 */
int getP1ButtonPressed() {
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        if ((P1_BTN_PIN & (1 << btnsP1[i])) == 0) {
            // BUSY-WAIT: While the button is still held down, do nothing
            while ((P1_BTN_PIN & (1 << btnsP1[i])) == 0); 
            _delay_ms(50); // Small buffer to ensure mechanical stability on release
            return i; 
        }
    }
    return -1; 
}

void recordPlayerInput(int targetLength) {
    playerInputCount = 0; 
    while (playerInputCount < targetLength) {
        int pressedIndex = getP1ButtonPressed();
        if (pressedIndex != -1) {
            playerSequence[playerInputCount] = pressedIndex;
            turnOnLED(pressedIndex);
            _delay_ms(200);
            turnOffAll();
            playerInputCount++;
        }
    }
}

// Similar implementation for Player 2 (Port C)
int getP2ButtonPressed() {
    for (int i = 0; i < NUM_OF_LEDS; i++) {
        if ((P2_BTN_PIN & (1 << btnsP2[i])) == 0) {
            while ((P2_BTN_PIN & (1 << btnsP2[i])) == 0);
            _delay_ms(50);
            return i; 
        }
    }
    return -1; 
}

void recordPlayerInputP2(int targetLength) {
    playerInputCount = 0; 
    while (playerInputCount < targetLength) {
        int pressedIndex = getP2ButtonPressed();
        if (pressedIndex != -1) {
            playerSequence[playerInputCount] = pressedIndex;
            turnOnLED(pressedIndex);
            _delay_ms(200);
            turnOffAll();
            playerInputCount++;
        }
    }
}
