/* * PROJECT: Mechatronics Memory Game
 * SHEET: Solo_Mode.ino
 * PURPOSE: Manages the single-player game logic, sequence validation, 
 * and persistent high-score tracking.
 */

#include <EEPROM.h>
#include "config.h"

// ================================================================
// LOGIC: Sequence Comparison
// ================================================================
/**
 * Compares two integer arrays using Pointer Arithmetic.
 * Pointer arithmetic is faster and more memory-efficient than array indexing 
 * for the ATmega328P processor.
 * * @param arrA   Pointer to the start of the system-generated sequence
 * @param arrB   Pointer to the start of the player-input sequence
 * @param length The number of elements (current round depth) to compare
 * @return bool  Returns true if sequences match perfectly; false if any mismatch
 */
bool compareSequences(int* arrA, int* arrB, int length) {
    for (int i = 0; i < length; i++) {
        // Dereference the pointer at the current offset (i) and compare
        if (*(arrA + i) != *(arrB + i)) {
            return false; // Instant exit on first mismatch
        }
    }
    return true; // Entire sequence matched
}

// ================================================================
// UTILITY: Wipe Data
// ================================================================
/**
 * Resets the game's progress and clears persistent storage (EEPROM).
 * This function returns the game to a "Factory Reset" state.
 */
void wipedata() {
    // 1. Clear the volatile (RAM) variables
    highScore = 0;
    p1Score = 0;
    p2Score = 0;
    p3Score = 0;
    chalP1Score = 0; 
    chalP2Score = 0;

    // 2. Clear the non-volatile (EEPROM) storage
    // Using EEPROM.put(address, 0) ensures that the full 16-bit integer 
    // width is overwritten at each specific memory slot.
    EEPROM.put(0, 0);  // Clear High Score
    EEPROM.put(2, 0);  // Clear P1 Duel Wins
    EEPROM.put(4, 0);  // Clear P2 Duel Wins
    EEPROM.put(6, 0);  // Clear P3 (BT) Duel Wins
    EEPROM.put(8, 0);  // Clear Challenger P1 Score
    EEPROM.put(10, 0); // Clear Challenger P2 Score

    // 3. User Feedback
    cleardata();   // Call the UI function to clear LCD/Labels
    BuzzerPass();  // Play "Success" chime to confirm wipe
    _delay_ms(1000);
}

// ================================================================
// MAIN GAME LOOP: Solo Mode
// ================================================================
/**
 * The primary execution loop for a single round of Solo Mode.
 * Returns -1 to continue to the next level, or 0 to exit back to Menu.
 */
int runSoloLoop() {
    
    // UI Update: Refresh the LCD with the current level number
    updateDisplay();
    
    // --- 1. SYSTEM PHASE ---
    // Generate a new random step in the sequence and playback to the user via LEDs
    generateAndShowSequence(); 

    // --- 2. USER PHASE ---
    // Block execution until the user inputs 'currentLength' number of button presses
    recordPlayerInput(currentLength); 

    // --- 3. LOGIC PHASE ---
    // Verify the user's memory against the system sequence using pointer comparison
    bool isCorrect = compareSequences(gameSequence, playerSequence, currentLength);

    // --- 4. OUTPUT PHASE (Result Handling) ---
    if (isCorrect) {
        // Check for new High Score
        if (currentLength > highScore) {
            highScore = currentLength;
            EEPROM.put(0, highScore); // Save to persistent memory immediately
        }

        // Success Feedback
        BuzzerPass();
        turnOnAll();     // Visual "Flash"
        _delay_ms(300);
        turnOffAll();
        _delay_ms(500);  // Brief "Inter-round" pause
        
        return -1; // SIGNAL: Continue to next level
    } 
    else {
        // Failure Feedback
        BuzzerFail();
        displayGameOver(); // Show final score on LCD
        _delay_ms(2000);

        // Reset game state for next session
        currentLength = 0; 
        return 0; // SIGNAL: Exit to Main Menu
    }
}
