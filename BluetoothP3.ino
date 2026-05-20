/* * PROJECT: Mechatronics Memory Game
 * SHEET: Bluetooth.ino
 * PURPOSE: Manages wireless data reception for Player 3 using UART.
 * Uses SoftwareSerial to keep the hardware RX/TX free for debugging.
 */

#include <SoftwareSerial.h>
#include "hardware.h"

// Define SoftwareSerial pins: RX = Pin 2, TX = Pin 3
// This allows us to use Player 3 via Bluetooth while still using 
// the USB Serial Monitor for debugging on Pins 0 and 1.
SoftwareSerial BTSerial(2, 3); 

// ================================================================
// INITIALIZATION
// ================================================================
/**
 * Configures the hardware registers and baud rate for Bluetooth.
 */
void initializeBluetooth() {
    // --- Direct Register Manipulation (ATMega328P) ---
    // Set Pin 2 (RX) as Input by clearing the bit in DDRD
    BT_DDR &= ~(1 << BT_RX_PIN);
    
    // Set Pin 3 (TX) as Output by setting the bit in DDRD
    BT_DDR |= (1 << BT_TX_PIN);
    
    // Begin Serial communication at 9600 Baud (standard for HC-05 modules)
    BTSerial.begin(9600);
    
    // Feedback to Serial Monitor
    Serial.println(F("BT System Active. DDRD configured for Pins 2 & 3."));
}

// ================================================================
// INPUT PROCESSING
// ================================================================
/**
 * Non-blocking check for Bluetooth data.
 * This is the "Instant" version used in the synchronous race loops.
 * @return int The button index (0-3) if data is available, otherwise -1.
 */
int getP3Instant() {
    // Check if the internal serial buffer has data waiting
    if (BTSerial.available() > 0) {
        // Read the incoming byte and cast it to a character
        char data = (char)BTSerial.read();
        
        // Debugging: View the raw character sent from the BT app
        Serial.print(F("BT Data: "));
        Serial.println(data);
        
        // --- CHARACTER MAPPING ---
        // Converts ASCII characters ('0', '1', '2', '3') into 
        // raw integers (0, 1, 2, 3) for array indexing.
        // Logic: '0' has an ASCII value of 48. If data is '0', 
        // 48 - 48 = 0.
        if (data >= '0' && data <= '3') {
            return data - '0';
        } else {
            // Error handling for stray characters or noise
            Serial.println(F("Error: Invalid BT input"));
        }
    }
    
    // Return -1 if no valid data was processed in this loop cycle
    return -1; 
}
