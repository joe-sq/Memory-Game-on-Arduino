/* * PROJECT: Mechatronics Memory Game
 * SHEET: leds.ino
 * PURPOSE: Manages visual feedback through LED control and handles
 * the generation of the randomized game sequence.
 */

#include "hardware.h"
#include "config.h"

// Array mapping logic indices (0-3) to physical bit positions on the Port
int leds[] = LED_ARRAY;

// ================================================================
// HARDWARE INITIALIZATION
// ================================================================
/**
 * Configures the LED pins as Outputs and ensures they start in the OFF state.
 * Uses bitwise OR to set bits in DDR and bitwise AND NOT to clear PORT bits.
 */
void initializeLEDs() {
    for(int i = 0; i < NUM_OF_LEDS; i++) {
        // Set specific bit in Data Direction Register to 1 (Output)
        LED_DDR |= (1 << leds[i]);
        // Set specific bit in Port Register to 0 (LOW/Off)
        LED_PORT &= ~(1 << leds[i]);
    }
}

// ================================================================
// BASIC LED CONTROL
// ================================================================

void turnOffAll() {
    for(int i = 0; i < NUM_OF_LEDS; i++) {
        LED_PORT &= ~(1 << leds[i]);
    }
}

void turnOnAll() {
    for(int i = 0; i < NUM_OF_LEDS; i++) {
        LED_PORT |= (1 << leds[i]);
    }
}

/**
 * Activates a single LED.
 * @param index The array position (0 to NUM_OF_LEDS-1).
 */
void turnOnLED(int index) {
    LED_PORT |= (1 << leds[index]);
}

/**
 * Custom delay function that accepts a variable input.
 * AVR's built-in _delay_ms() requires a compile-time constant.
 * This wrapper allows for dynamic gameplay speeds.
 */
void variable_delay_ms(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1); // Execute 1ms delay repeatedly
    }
}

// ================================================================
// GAME LOGIC: SEQUENCE GENERATION
// ================================================================
/**
 * Appends a new random step to the sequence and plays the full pattern
 * with an increasing speed (decreasing delay) to raise difficulty.
 */
void generateAndShowSequence() {
    // 1. ADD NEW STEP
    // Prevents memory overflow by capping the sequence at 50
    if (currentLength < 50) {
        gameSequence[currentLength] = random(NUM_OF_LEDS);
        currentLength++;
    }

    // 2. PLAYBACK SEQUENCE
    for (int i = 0; i < currentLength; i++) {
        // --- DYNAMIC DIFFICULTY CALCULATION ---
        // Formula: Start at 600ms, subtract 50ms for every successful step.
        // max(100, ...) ensures the game never flashes faster than 100ms.
        int dynamicDelay = max(100, 600 - (i * 50)); 

        turnOffAll();
        _delay_ms(150); // Small "dark" gap to make consecutive identical LEDs distinct
        
        // Light up the LED stored at this step of the sequence
        turnOnLED(gameSequence[i]);
        
        // Keep LED on for the calculated duration
        variable_delay_ms(dynamicDelay); 
        
        turnOffAll();
    }
}
