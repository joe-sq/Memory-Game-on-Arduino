/* * PROJECT: Mechatronics Memory Game
 * SHEET: challenger.ino
 * PURPOSE: Implements a "Maker vs. Breaker" mode. 
 * One player creates a pattern, and the other must solve it.
 */

#include "config.h"

/**
 * Executes the Challenger game logic.
 * Phase 1: Creation (P1)
 * Phase 2: Replication (P2)
 * Phase 3: Validation/Proof (P1) - Only if P2 fails.
 * * @return int -1 to continue another round, 0 to return to Main Menu.
 */
int playChallengerMode() {

    // --- STEP 1: PATTERN CREATION (Player A) ---
    // Player A inputs a secret sequence that is stored in RAM, not displayed on LCD.
    displayP1SecretPrompt();
    
    for(int i = 0; i < CHALLENGER_SEQ_LENGTH; i++) {
        int pressed = -1;
        // Blocking loop: Wait for physical button interaction
        while(pressed == -1) { 
            pressed = getP1ButtonPressed(); 
        }
        
        // Store the button ID in the secret array
        secretSequence[i] = pressed;
        
        // Visual/Audio confirmation for the creator
        turnOnLED(pressed);
        _delay_ms(200);
        turnOffAll();
    }

    // --- STEP 2: REPLICATION PHASE (Player B) ---
    // The LCD prompts Player B to try and match what they just saw (or didn't see).
    displayP2ReplicatePrompt();
    
    // recordPlayerInputP2 is a specific function for the second keypad/input set
    recordPlayerInputP2(CHALLENGER_SEQ_LENGTH);

    // LOGIC: Compare Player B's recorded attempt against Player A's secret sequence
    bool p2Correct = compareSequences(secretSequence, playerSequence, CHALLENGER_SEQ_LENGTH);

    if (p2Correct) {
        // SUCCESS: Player B gets a point
        chalP2Score++;          
        EEPROM.put(10, chalP2Score); // Persistent storage at Address 10
        displayP2Success();
        _delay_ms(2000);
        return -1; // Round over, P2 wins point
    } 
    else {
        // --- STEP 3: THE PROOF PHASE (Player A) ---
        // Player B failed. To prevent P1 from making an impossible pattern,
        // P1 must now prove they can actually perform the pattern they created.
        displayP1ProofPrompt();
        _delay_ms(1000);
        
        // Reuse the Solo input recorder to fill playerSequence with P1's hardware inputs
        recordPlayerInput(CHALLENGER_SEQ_LENGTH); 
        
        bool p1Proved = compareSequences(secretSequence, playerSequence, CHALLENGER_SEQ_LENGTH);

        if (p1Proved) {
            // P1 proved it! P1 gets the point for a successful challenge.
            chalP1Score++;           
            EEPROM.put(8, chalP1Score); // Persistent storage at Address 8
            displayP1Success();
            return -1; // Round over, P1 wins point
        } 
        else {
            // P1 failed their own pattern. No points awarded, and the game ends in shame.
            displayP1Fail();
            _delay_ms(2000);
            displayGameOver();
            _delay_ms(2000);
            return 0; // Exit to Main Menu
        }
        _delay_ms(2000);
    }
}
