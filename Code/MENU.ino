/* * PROJECT: Mechatronics Memory Game
 * SHEET: Menu.ino (Main Execution Hub)
 * PURPOSE: Handles system initialization, persistent memory loading, 
 * and the primary user interface navigation.
 */

#include "config.h"

// ================================================================
// SETUP: Runs once at startup
// ================================================================
void setup() {
    // --- Hardware Abstraction Layer (HAL) Initialization ---
    intializeP1Keypad();    // Configure Port B for Player 1 inputs
    intializeP2Keypad();    // Configure Port C for Player 2 inputs
    initializeLEDs();       // Set LED pins as outputs
    InitializeBuzzer();     // Configure PWM/Timer for audio feedback
    initializeBluetooth(); // Setup UART communication for Player 3 (HC-05/06)
    
    // --- UI & Display Initialization ---
    setupLCD();             // Initialize I2C communication with 20x4 LCD
    
    // --- Persistent Data Recovery ---
    // EEPROM.get retrieves 2-byte integers. 
    // We use even-numbered addresses to avoid data overlapping.
    EEPROM.get(0, highScore);     // Solo Mode High Score
    EEPROM.get(2, p1Score);       // Duel Mode: Player 1 Wins
    EEPROM.get(4, p2Score);       // Duel Mode: Player 2 Wins
    EEPROM.get(6, p3Score);       // Duel Mode: Player 3 (BT) Wins
    EEPROM.get(8, chalP1Score);   // Challenger Mode: P1 Lifetime Score
    EEPROM.get(10, chalP2Score);  // Challenger Mode: P2 Lifetime Score

    // Capture starting time for the 2-minute sleep timer
    lastActivityTime = millis(); 
}

// ================================================================
// MAIN LOOP: Finite State Machine (FSM)
// ================================================================
void loop() {
    displayMainMenu();      // Refresh the LCD with mode options
    
    int gameStatus;         // Tracks if a game is active (-1), won, or lost
    int choice = -1;        // Stores the user's menu selection

    // --- PHASE 1: INPUT POLLING & POWER MANAGEMENT ---
    // This loop blocks execution until the user interacts with the system.
    while (choice == -1) {
        // Check if idle time exceeds 2 minutes (120,000ms)
        manageSleepMode();

        // Check Player 1 (Main Menu Navigation)
        int p1Check = getP1ButtonPressed();
        if (p1Check != -1) {
            wakeSystem();   // Restore backlight if triggered
            choice = p1Check;
        }

        // Check Player 2 (Secret Leaderboard Access)
        // Bitwise mask checks if PC0 (Pin A0) is pulled LOW (GND)
        if ((P2_BTN_PIN & (1 << 0)) == 0) {
            wakeSystem();
            choice = 4;     // Force choice to Leaderboard mode
            while ((P2_BTN_PIN & (1 << 0)) == 0); // Mechanical debounce (wait for release)
        }
    }

    // --- PHASE 2: MODE EXECUTION BRANCHING ---

    // --- MODE 0: SOLO ---
    if (choice == 0) {
        startSolo();        // Initialize sequence and difficulty
        _delay_ms(1000); 
        do {
            gameStatus = runSoloLoop(); // Main gameplay logic
        } while (gameStatus == -1);     // -1 indicates level cleared; loop continues
    }

    // --- MODE 1: DUEL (2P or 3P Race) ---
    else if (choice == 1) {
        displayDuelSelection(); // Sub-menu for 2 vs 3 player selection
        
        int duelChoice = -1;
        while (duelChoice == -1) {
            duelChoice = getP1ButtonPressed(); // Wait for user choice
        }

        currentLength = 3; // Reset difficulty baseline for competitive fairness

        if (duelChoice == 0) { // Race Mode: 2 Players (Hardware vs Hardware)
            displayDuelStart(2);
            _delay_ms(1000);
            do {
                gameStatus = runDuel2P();
            } while (gameStatus == -1);
        } 
        else if (duelChoice == 1) { // Race Mode: 3 Players (Includes BT)
            displayDuelStart(3);
            _delay_ms(1000);
            do {
                gameStatus = runDuel3P();
            } while (gameStatus == -1);
        }
    }

    // --- MODE 2: CHALLENGER (PvP Pattern Replication) ---
    else if (choice == 2) {
        startChallenger();
        _delay_ms(1000);
        do {
            gameStatus = playChallengerMode();
        } while (gameStatus == -1);
    }

    // --- SYSTEM UTILITY: WIPE DATA ---
    else if(choice == 3) {
        wipedata(); // Clears EEPROM and resets local score variables
    }

    // --- SYSTEM UTILITY: LEADERBOARD ---
    else if (choice == 4) {
        displayLeaderboards(); // Prints all scores from RAM to LCD
        _delay_ms(500);        // Prevent accidental exit from initial button press
        
        // Wait for any button to return to the main menu
        while (getP1ButtonPressed() == -1); 
    }
    
}
