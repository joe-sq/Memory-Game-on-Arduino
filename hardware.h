/* * PROJECT: Mechatronics Memory Game
 * SHEET: hardware.h
 * PURPOSE: Pin mapping and Low-Level Register Definitions.
 * This file translates logical game components to physical AVR registers.
 */

#ifndef HARDWARE_H
#define HARDWARE_H

// ================================================================
// PLAYER 1 INPUTS (Keypad 1)
// ================================================================
// Pins: Arduino 8, 9, 10, 11
// Register: Port B (Bits 0, 1, 2, 3)
#define P1_BTN_ARRAY {0, 1, 2, 3} 
#define P1_BTN_DDR   DDRB  // Data Direction Register (Input/Output)
#define P1_BTN_PORT  PORTB // Pull-up Resistor Control
#define P1_BTN_PIN   PINB  // Read Input Status

// ================================================================
// PLAYER 2 INPUTS (Keypad 2)
// ================================================================
// Pins: Arduino Analog A0, A1, A2, A3
// Register: Port C (Bits 0, 1, 2, 3)
// Using Analog pins as Digital inputs is an efficient use of hardware.
#define P2_BTN_ARRAY {0, 1, 2, 3} 
#define P2_BTN_DDR   DDRC
#define P2_BTN_PORT  PORTC
#define P2_BTN_PIN   PINC

// ================================================================
// OUTPUT LEDs
// ================================================================
// Pins: Arduino Digital 4, 5, 6, 7
// Register: Port D (Bits 4, 5, 6, 7)
#define LED_ARRAY {4, 5, 6, 7} 
#define LED_DDR   DDRD
#define LED_PORT  PORTD
#define LED_PIN   PIND
#define NUM_OF_LEDS 4

// ================================================================
// SYSTEM BUZZER (Audio Feedback)
// ================================================================
// Pin: Arduino Digital 13
// Register: Port B (Bit 5)
#define Buzzer_ARRAY {5} 
#define Buzzer_DDR   DDRB
#define Buzzer_PORT  PORTB
#define Buzzer_PIN   PINB

// ================================================================
// BLUETOOTH PLAYER 3 (HC-05)
// ================================================================
// Pins: Arduino Digital 2 (RX) and 3 (TX)
// Register: Port D (Bits 2, 3)
#define BT_RX_PIN 2  // Input to Arduino
#define BT_TX_PIN 3  // Output from Arduino
#define BT_DDR    DDRD
#define BT_PORT   PORTD

#endif
