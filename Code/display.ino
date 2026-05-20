/* * PROJECT: Mechatronics Memory Game
 * SHEET: display.ino
 * PURPOSE: Manages the LiquidCrystal I2C interface, UI menu screens, 
 * and system power-saving (Sleep Mode) logic.
 */

#include <LiquidCrystal_I2C.h>
#include "config.h"

// Initialize the 20x4 LCD with I2C address 0x27. 
// 0x27 is the standard address for PCF8574 I2C backpacks.
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ================================================================
// SYSTEM INITIALIZATION
// ================================================================
/**
 * Starts the I2C bus and enables the LCD backlight.
 */
void setupLCD() {
    lcd.init();
    lcd.backlight();
}

// ================================================================
// POWER MANAGEMENT (Sleep Mode)
// ================================================================

/**
 * Monitors system idleness. If no buttons are pressed for 120 seconds,
 * the backlight and LEDs are disabled to save power.
 */
void manageSleepMode() {
    // Check if 2 minutes (120,000 milliseconds) have passed since lastActivityTime
    if (!isAsleep && (millis() - lastActivityTime > 120000)) {
        lcd.noBacklight();   // Turn off LCD lamp
        turnOffAll();        // Ensure all game LEDs are off
        isAsleep = true;
        Serial.println(F("System entered sleep mode."));
    }
}

/**
 * Restores system power state upon user interaction.
 */
void wakeSystem() {
    if (isAsleep) {
        lcd.backlight();
        isAsleep = false;
        // Content remains in LCD memory; we simply restore visibility.
    }
    lastActivityTime = millis(); // Reset the inactivity timer
}


void displayMainMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Select Mode:"));
    lcd.setCursor(0, 1);
    lcd.print(F("1: Solo "));
    lcd.setCursor(0, 2);
    lcd.print(F("2: Duel"));
    lcd.setCursor(0, 3);
    lcd.print(F("3: Challenge"));
    lcd.setCursor(8, 1);
    lcd.print(F("4: Wipe data"));
    lcd.setCursor(8, 2);
    lcd.print(F("5: leaderboard"));
}

void displayLeaderboards() {
    lcd.clear();
    // Row 0: Solo & Player 1 Duel Wins
    lcd.print(F("S:")); lcd.print(highScore);
    lcd.print(F(" D1:")); lcd.print(p1Score);
    
    // Row 1: Player 2 & Player 3 Duel Wins
    lcd.setCursor(0, 1);
    lcd.print(F("D2:")); lcd.print(p2Score);
    lcd.print(F(" D3:")); lcd.print(p3Score);
    
    // Row 2: Challenger Mode Career Scores
    lcd.setCursor(0, 2);
    lcd.print(F("C1:")); lcd.print(chalP1Score);
    lcd.print(F(" C2:")); lcd.print(chalP2Score);
}

// ================================================================
// GAME STATE NOTIFICATIONS
// ================================================================

void updateDisplay() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("--- SOLO GAME ---"));
    lcd.setCursor(0, 1);
    lcd.print(F("Level: "));
    lcd.print(currentLength);
    lcd.setCursor(0, 2);
    lcd.print(F("High Score: "));
    lcd.print(highScore);
}

void displayGameOver() {
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print(F("GAME OVER!"));
}

// --- Challenger Mode UI ---
void displayP1SecretPrompt()  { lcd.clear(); lcd.print(F("P1: Enter Secret")); }
void displayP2ReplicatePrompt() { lcd.clear(); lcd.print(F("P2: Replicate!")); }
void displayP2Success()       { lcd.clear(); lcd.print(F("P2 Scores!")); }
void displayP1ProofPrompt()   { lcd.clear(); lcd.print(F("Fail! P1: Prove it")); }
void displayP1Success() {
    lcd.clear();
    lcd.print(F("P1 Proved It!"));
    lcd.setCursor(0, 1);
    lcd.print(F("P1 gets the point"));
}

// --- Duel Mode UI ---
void displayDuelSelection() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Select Duel Type:"));
    lcd.setCursor(0, 1);
    lcd.print(F("B0: 2 Players"));
    lcd.setCursor(0, 2);
    lcd.print(F("B1: 3 Players (BT)"));
}

void displayDuelWinner(int player) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("WINNER: PLAYER "));
    lcd.print(player);
    BuzzerPass();
    lcd.setCursor(0, 1);
    lcd.print(F("P1:")); lcd.print(p1Score);
    lcd.print(F(" P2:")); lcd.print(p2Score);
    lcd.print(F(" P3:")); lcd.print(p3Score);
    _delay_ms(2000);
}

void displayDuelGo() {
    lcd.clear();
    lcd.setCursor(7, 1);
    lcd.print(F("GO!!!"));
}

void cleardata() {
    lcd.clear();
    lcd.print(F("Data Wiped!"));
}

// Basic Mode Start Indicators
void startSolo()      { lcd.clear(); lcd.print(F("Starting Solo...")); }
void startChallenger(){ lcd.clear(); lcd.print(F("Challenger Mode")); }
void displayDuelDraw(){ lcd.clear(); lcd.setCursor(4, 1); lcd.print(F("BOTH FAILED!")); }
