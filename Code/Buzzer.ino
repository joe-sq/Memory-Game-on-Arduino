/* * PROJECT: Mechatronics Memory Game
 * SHEET: buzzer.ino
 * PURPOSE: Manages auditory feedback for game events (Success/Failure).
 * Configures hardware registers for the piezo buzzer and handles frequency generation.
 */

#include "hardware.h"

// ================================================================
// INITIALIZATION
// ================================================================
/**
 * Configures the hardware pin for the buzzer.
 * In your hardware.h, Buzzer_DDR is likely mapped to DDRB (Port B).
 */
void InitializeBuzzer() {
    // --- Direct Register Manipulation ---
    // Set Bit 5 (Pin 13) as Output (1) in the Data Direction Register
    Buzzer_DDR |= (1 << 5);

    // Ensure the buzzer is OFF (0) at startup to prevent a constant hum
    Buzzer_PORT &= ~(1 << 5);
}

// ================================================================
// SOUND EFFECTS
// ================================================================

/**
 * Generates a low-frequency tone to indicate a mistake or game over.
 * Tone: 1000Hz (Low/Dull) for 150ms.
 */
void BuzzerFail(){
    // Pin 13, Frequency: 1000Hz, Duration: 150ms
    tone(13, 1000, 150);
}

/**
 * Generates a high-frequency, multi-tone "Chime" to indicate success.
 * This function uses a "Sequence of Tones" to create a pleasant melody.
 */
void BuzzerPass(){
    // Step 1: Medium-High Pitch
    tone(13, 2000, 150);
    _delay_ms(200); // Wait for the tone to finish before next step

    // Step 2: Higher Pitch (Ascending)
    tone(13, 2500, 150);
    _delay_ms(200);

    // Step 3: Return to Medium-High
    tone(13, 2000, 150);
}
