/* * PROJECT: Mechatronics Memory Game
 * SHEET: SynchronousDuel.ino
 * PURPOSE: Manages real-time competitive racing between 2 or 3 players.
 * Includes Bluetooth integration and exclusive winner validation.
 */

#include "config.h"
#include <EEPROM.h>

// ================================================================
// MODE 1: 2-PLAYER DUEL (Hardware vs Hardware)
// ================================================================
/**
 * Executes a head-to-head race between Player 1 and Player 2.
 * Players attempt to complete the sequence simultaneously.
 * @return int -1 to continue, 0 to return to menu.
 */
int runDuel2P() {
    // Progress counters (indices for gameSequence)
    int p1Prog = 0, p2Prog = 0;
    // Failure flags to stop processing input for a player who makes a mistake
    bool p1Fail = false, p2Fail = false;

    // Baseline sequence length check
    if (currentLength == 0) currentLength = 3;

    // --- PRE-GAME PHASE ---
    displayDuel2PSequencePrompt(); // UI: "2P Duel - Watch!"
    generateAndShowSequence();      // Play the pattern for both players
    displayDuelGo();               // UI: "GO!"
    BuzzerPass();                  // Audio start signal

    // --- SYNCHRONOUS RACE LOOP ---
    // Condition: Continue only while BOTH players are still below the target length
    // If one person finishes, the loop terminates immediately.
    while (p1Prog < currentLength && p2Prog < currentLength) {
        
        // 1. Check Player 1 (Port B Hardware)
        int p1In = getP1Instant(); 
        if (p1In != -1 && !p1Fail) {
            if (p1In == gameSequence[p1Prog]) { 
                p1Prog++; 
                turnOnLED(p1In); 
            } else { 
                p1Fail = true; 
                Serial.println(F("P1 Fail")); 
            }
        }

        // 2. Check Player 2 (Port C Hardware)
        int p2In = getP2Instant();
        if (p2In != -1 && !p2Fail) {
            if (p2In == gameSequence[p2Prog]) { 
                p2Prog++; 
                turnOnLED(p2In); 
            } else { 
                p2Fail = true; 
                Serial.println(F("P2 Fail")); 
            }
        }

        // 3. Visual Feedback & Loop Management
        // If anyone pressed a button, wait briefly to show LED then clear all
        if (p1In != -1 || p2In != -1) { 
            variable_delay_ms(100); 
            turnOffAll(); 
        }

        // 4. Mutual Failure Check
        if (p1Fail && p2Fail) break;
    }

    // --- WINNER VALIDATION PHASE (Exclusive Check) ---
    // P1 Wins: Reached target length AND P2 did not
    if (p1Prog == currentLength && p2Prog != currentLength) {
        p1Score++;
        EEPROM.put(2, p1Score); // Save P1 win count
        displayDuelWinner(1);
        return -1;
    } 
    // P2 Wins: Reached target length AND P1 did not
    else if (p1Prog != currentLength && p2Prog == currentLength) {
        p2Score++;
        EEPROM.put(4, p2Score); // Save P2 win count
        displayDuelWinner(2);
        return -1;
    } 
    // Draw: Both finished in the same loop cycle, or both failed
    else {
        displayDuelDraw();
        return 0; 
    }
}

// ================================================================
// MODE 1 (EXTENDED): 3-PLAYER DUEL (Includes Bluetooth)
// ================================================================
/**
 * Executes a 3-way race. 
 * Player 3 uses Bluetooth (getP3Instant) via UART communication.
 */
int runDuel3P() {
    int p1Prog = 0, p2Prog = 0, p3Prog = 0;
    bool p1Fail = false, p2Fail = false, p3Fail = false;

    if (currentLength == 0) currentLength = 3;

    displayDuel3PSequencePrompt(); 
    generateAndShowSequence(); 
    displayDuelGo();
    BuzzerPass();

    // The loop continues only if ALL THREE players have not finished yet.
    while (p1Prog < currentLength && p2Prog < currentLength && p3Prog < currentLength) {
        
        // P1 Hardware (Keypad 1)
        int p1In = getP1Instant();
        if (p1In != -1 && !p1Fail) {
            if (p1In == gameSequence[p1Prog]) { p1Prog++; turnOnLED(p1In); }
            else { p1Fail = true; Serial.println(F("P1 Fail")); }
        }

        // P2 Hardware (Keypad 2)
        int p2In = getP2Instant();
        if (p2In != -1 && !p2Fail) {
            if (p2In == gameSequence[p2Prog]) { p2Prog++; turnOnLED(p2In); }
            else { p2Fail = true; Serial.println(F("P2 Fail")); }
        }

        // P3 Bluetooth (External serial device)
        int p3In = getP3Instant();
        if (p3In != -1 && !p3Fail) {
            if (p3In == gameSequence[p3Prog]) { p3Prog++; turnOnLED(p3In); }
            else { p3Fail = true; Serial.println(F("P3 Fail")); }
        }

        // Simultaneous Input Handling
        if (p1In != -1 || p2In != -1 || p3In != -1) { 
            variable_delay_ms(100); 
            turnOffAll(); 
        }

        // Break if everyone has failed
        if (p1Fail && p2Fail && p3Fail) break;
    }

    // --- 3-PLAYER WINNER LOGIC ---
    if (p1Prog == currentLength && p2Prog != currentLength && p3Prog != currentLength) {
        p1Score++;
        EEPROM.put(2, p1Score);
        displayDuelWinner(1);
        return -1;
    } else if (p1Prog != currentLength && p2Prog == currentLength && p3Prog != currentLength) {
        p2Score++;
        EEPROM.put(4, p2Score);
        displayDuelWinner(2);
        return -1;
    } else if (p1Prog != currentLength && p2Prog != currentLength && p3Prog == currentLength) {
        p3Score++;
        EEPROM.put(6, p3Score); // Save P3 win count to address 6
        displayDuelWinner(3);
        return -1;
    } else {
        displayDuelDraw();
        return 0;
    }
}
