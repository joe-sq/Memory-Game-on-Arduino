
# Multiplayer Memory Game Console

An interactive, microcontroller-based embedded system designed to challenge and test players' memory and reaction speed. Built on the Arduino Uno platform using AVR programming, this console generates innovative multiplayer memory game challenges!

![Memory Game Demo](https://github.com/user-attachments/assets/6ae138a8-4cae-4cb9-8363-480d744d5a68)


https://github.com/user-attachments/assets/7fdf469f-80aa-42e2-8428-b92479bc0ef8


---

## 🎓 Academic Metadata

**Institution:** University of Hertfordshire – School of Engineering and Computer Science

**Module Title:** Computer Programming  
**Module Code:** 4FTC2168

**Academic Supervisors:**
- Abdelgawad Elashry
- Ahmed Abdelbasit

**Submission Date:** 29 April

**Student Contributors:**
| Student Name | UH ID |
|---|---|
| Maryam Gamal Alahmady | 24080584 |
| Mohamed Abdelhamid | 24079047 |
| Youssif Saqr | 24081414 |
| Youssef Shady | 24080686 |
| Samir Hany | 24080549 |

---

## 📋 Table of Contents
- [Academic Metadata](#-academic-metadata)
- [Features](#-features)
- [Tech Stack & Components](#-tech-stack--components)
- [Gameplay Modes](#-gameplay-modes)
- [MIT App Inventor Integration](#-mit-app-inventor-integration)
- [Project Structure](#-project-structure)
- [Memory Management](#-eeprom-memory-management)
- [Hardware Design](#-hardware-design)
- [Testing & Results](#-testing--results)
- [Key Learnings](#-challenges-faced--key-learnings)
- [Getting Started](#-how-to-run-and-flash-the-project)
- [Hardware Wiring](#-hardware-wiring-guide)
- [Future Improvements](#-future-improvements)
- [Comparison with Existing Products](#-comparison-with-existing-products)
- [License](#-license)

---

## ✨ Features

* **Real-Time Multiplayer Gameplay** - Synchronous 2-3 player races with instant winner validation
* **State Machine Architecture** - Fast, modular, and responsive menu transitions with no blocking calls during competitive modes
* **Software Debouncing** - Eliminates mechanical button noise for reliable input detection
* **Optimized Memory Usage** - Pointer arithmetic instead of heavy array indexing for constrained ATmega328P processor
* **Persistent Leaderboards** - Non-volatile EEPROM storage for career high scores across power cycles
* **Wireless Expansion** - HC-05 Bluetooth module enables 3-player competitive gaming
* **Audio & Visual Feedback** - Piezo buzzer with distinct success/failure tones and dynamic LED sequences
* **Power Management** - 2-minute idle sleep mode conserves battery during inactive periods

---

## 🛠️ Tech Stack & Components

### Software & Architecture
* **Language:** AVR C / C++ (Arduino IDE)
* **Architecture:** State Machine Architecture (for fast response, modularity, and smooth menu transitions)
* **Key Techniques:** 
  * Software debouncing (eliminates mechanical button noise)
  * Pointer Arithmetic (optimized memory and execution efficiency for the ATmega328P processor)
  * Non-blocking input polling for synchronous multiplayer races

### Hardware Components
| Component | Quantity | Cost | Function |
|-----------|----------|------|----------|
| **Arduino Uno (ATmega328P)** | 1 | University | Main microcontroller & CPU |
| **LEDs (RGB Set)** | 4 | University | Visual game sequence display |
| **Push Buttons** | 8 | 40 EGP | Player 1 & Player 2 inputs (4 each) |
| **LCD Display (20x4)** | 1 | 220 EGP | Menu, scores, and game information |
| **Piezo Buzzer** | 1 | 30 EGP | Audio feedback (success/failure tones) |
| **Resistors (100Ω)** | 4 | University | LED current limiting protection |
| **Resistors (1kΩ)** | 3 | University | Signal stabilization & protection |
| **Lithium-ion Batteries (3.7V)** | 2 | University | Power supply for portable operation |
| **Dual Battery Holder** | 1 | 50 EGP | Secure battery mounting & connections |
| **HC-05 Bluetooth Module** | 1 | 235 EGP | Wireless Player 3 communication |
| **I2C LCD Backpack** | 1 | Included | LCD protocol interface |
| **Breadboard + Perfboard** | 1 | University | Circuit assembly & soldering |

**Total Out-of-Pocket Cost:** ~575 EGP (excluding university-provided components)


---

## 🎮 Gameplay Modes

### 1. **Solo Sprint Mode** ⚡
A single-player challenge where the system generates increasingly complex LED sequences. The player must replicate each sequence exactly.

**Mechanics:**
- The game starts with a 1-step sequence
- Each successful round adds one more LED step
- The duration each LED remains lit decreases dynamically based on your score, creating a difficulty curve that accelerates
- **Difficulty Formula:** LED on-time = `max(100ms, 600ms - 50ms × step_number)`
- Game ends immediately upon the first mistake
- High scores are permanently saved to EEPROM

**Scoring:** Your score equals the longest sequence you successfully replicate.

---

### 2. **Synchronous Duel Mode** 🏁
Two or three players compete simultaneously against the same generated sequence. The system evaluates inputs and awards points based on both correctness **and** speed.

**2-Player Race:**
- Both players watch the same sequence
- Upon "GO!", both attempt to replicate it simultaneously (non-blocking)
- The first player to complete the sequence correctly wins the round
- If one player makes a mistake, they're locked out; only a perfect replication counts
- Scores persist across multiple rounds and are saved to EEPROM

**3-Player Race (with Bluetooth):**
- Player 1 & 2 use hardware keypads (Ports B & C)
- Player 3 sends input via Bluetooth (HC-05 module on Pins 2 & 3)
- Same race logic applies: first to finish perfectly wins
- All three players' win counts are tracked independently

**Exclusive Winner Validation:**
- A player only wins if they reach the target length while others have not
- If multiple players finish simultaneously, it's declared a draw
- Draws grant no points

---

### 3. **Challenger Mode ("Maker vs. Breaker")** 🎯
A creative PvP mode where Player 1 designs a challenge and Player 2 attempts to solve it.

**Phase 1 - Creation (Player 1):**
- Player 1 secretly inputs a 4-button sequence (not displayed on LCD)
- Each button press is confirmed with a brief LED flash
- The sequence is stored in RAM (not revealed to Player 2)

**Phase 2 - Replication (Player 2):**
- Player 2 attempts to replicate Player 1's secret pattern from memory
- Player 2 inputs 4 buttons; their attempt is validated against the original

**Phase 3 - Proof (Player 1 only if P2 fails):**
- If Player 2's replication is incorrect, Player 1 must re-enter their original sequence to prove it was valid
- This prevents "impossible" sequences from being claimed

**Scoring:**
- **P2 Succeeds:** Player 2 earns 1 point
- **P2 Fails & P1 Proves:** Player 1 earns 1 point
- **P2 Fails & P1 Cannot Prove:** Game Over; no points awarded (shame on P1!)

All challenger mode wins are saved to EEPROM for career tracking.

---

​📱 MIT App Inventor Integration
​A companion Android/iOS mobile controller application designed in MIT App Inventor allows Player 3 to compete wirelessly against the local hardware stations via the HC-05 module.
​🔗 MAC Address Mapping Configuration
​To configure the application interface for your specific hardware platform:

​Power up the console and check your phone's standard Bluetooth discovery list to locate your 12-character physical hardware address (e.g., 00:21:13:02:66:B8).
​Open the project inside the MIT App Inventor interface, navigate to the Blocks Editor, and update the connection trigger blocks (when Connect_Button.Click) to send connection requests directly to that verified target string.

​🛑 Critical Core Protocol: Avoid BLE Components

​Hardware Profile: The HC-05 module utilized in this layout belongs to the Bluetooth Classic (BR/EDR) protocol family. It does not utilize or support Bluetooth Low Energy (BLE).
​Component Settings: Within the App Inventor Designer interface, do not import or execute methods from the custom edu.mit.appinventor.ble extension. Instead, drop the native BluetoothClient object component into the workspace layout to handle standard Serial Port Profile (SPP) streams.

​🍏 Cross-Platform Handling: iOS Device Restrictions
​When deploying and testing the workspace design through the native MIT AI2 Companion for iOS environment, note the following Apple sandboxing protocols:

​Hardware Address Obfuscation: To protect user privacy, iOS devices completely hide the static MAC address of external Bluetooth Classic peripherals.
​Dynamic Software UUID Translation: iOS maps discoverable peripherals to a randomized software ID code called a UUID (Universally Unique Identifier).
​Per-Device Instability: This generated UUID code is distinct to each specific iOS hardware unit. Two separate iPhones tracking the same console will show two completely different alphanumeric identifiers.
​Development Workaround: Avoid embedding rigid, hardcoded string addresses within your execution blocks. Instead, integrate a dynamic ListPicker component. This forces the application to scan open frequencies dynamically, present the visible string identifier name (HC-05), and safely acquire the active connection reference token at execution runtime.

---

## 📁 Project Structure

```
Memory-Game-on-Arduino/
├── MENU.ino                 # Main execution hub, FSM, and system initialization
├── Solo_Mode.ino            # Single-player game logic and sequence validation
├── SynchronousDuel.ino      # 2P/3P competitive racing logic
├── Challenger.ino           # PvP "Maker vs. Breaker" mode
├── LEDs.ino                 # LED control and sequence generation
├── keypad.ino               # Button input handling with debouncing
├── display.ino              # LCD UI and sleep mode management
├── Buzzer.ino               # Audio feedback (success/failure tones)
├── BluetoothP3.ino          # Wireless data reception for Player 3 (HC-05)
├── hardware.h               # Pin mapping and AVR register definitions
├── config.h                 # Global variables, arrays, and game constants
└── README.md                # This file
```

### Key File Descriptions

| File | Purpose |
|------|---------|
| **MENU.ino** | Finite State Machine that orchestrates mode selection and coordinates between all game modules. Loads persistent EEPROM data at startup. |
| **Solo_Mode.ino** | Implements single-player logic: sequence generation, user input recording, comparison via pointer arithmetic, and high-score persistence. |
| **SynchronousDuel.ino** | Non-blocking multiplayer racing: monitors 2-3 players simultaneously, detects exclusive winners, and updates leaderboards. |
| **Challenger.ino** | Custom pattern creation/replication with a validation phase to ensure fair play. |
| **LEDs.ino** | Manages visual feedback with hardware register manipulation. Includes dynamic difficulty calculations for the LED on-time. |
| **keypad.ino** | Handles input from Port B (P1) and Port C (P2) with falling-edge detection and software debouncing. |
| **display.ino** | LCD interface, leaderboard display, and 2-minute idle sleep mode (saves battery). |
| **Buzzer.ino** | PWM-based audio feedback: low 1000Hz tone for failure, multi-tone "chime" for success. |
| **BluetoothP3.ino** | Non-blocking Bluetooth serial communication for Player 3 input via HC-05 module. |
| **hardware.h** | Central registry mapping logical game components to physical AVR ports and bit positions. |
| **config.h** | Game state arrays, scoring variables, timing constants (debounce delay), and EEPROM address layout. |

---

## 💾 EEPROM Memory Management

To elevate the device beyond a basic prototype, the system utilizes the ATmega328P's non-volatile EEPROM memory. Leaderboard rankings and individual high scores are permanently stored directly to memory.

### EEPROM Address Layout
| Address | Variable | Size | Purpose |
|---------|----------|------|---------|
| 0–1 | highScore | int | Solo Mode high score |
| 2–3 | p1Score | int | Duel Mode: Player 1 wins |
| 4–5 | p2Score | int | Duel Mode: Player 2 wins |
| 6–7 | p3Score | int | Duel Mode: Player 3 (BT) wins |
| 8–9 | chalP1Score | int | Challenger: Player 1 wins |
| 10–11 | chalP2Score | int | Challenger: Player 2 wins |

**Key Implementation Details:**
- Addresses are spaced in 2-byte increments to accommodate future expansion to larger data types
- `EEPROM.get()` and `EEPROM.put()` are used for safe read/write operations
- Data is automatically loaded from EEPROM during `setup()` (see MENU.ino)
- Scores are updated to EEPROM immediately after each game conclusion to prevent data loss from power cycles

**EEPROM Capacity:** 1KB total. Current usage: ~24 bytes (96% available for future features).

---

## 🔧 Hardware Design

### Circuit Architecture
The hardware design focuses on creating a robust, interactive gaming platform while ensuring all components integrate seamlessly with the microcontroller. The Arduino Uno serves as the central processor.

**Component Layout:**
- **Input Section:** 8 tactile push buttons (4 per player) connected to Ports B and C with pull-up resistors
- **Output Section:** 4 LEDs on Port D (pins 4-7) with 100Ω current-limiting resistors, and a piezo buzzer on Port B pin 13
- **Display Section:** 20x4 LCD connected via I2C protocol (SDA on A4, SCL on A5) with standard address 0x27
- **Wireless Section:** HC-05 Bluetooth module on Pins 2 (RX) and 3 (TX) using SoftwareSerial

### Power Supply Configuration
The system uses **two 3.7V Lithium-ion cells** housed in a dual battery holder for stable console operation and extended portable runtime. The dual-cell configuration ensures:
- **Voltage Stability:** 3.7V maintains consistent microcontroller performance
- **Hot-Swapping:** Players can replace batteries without system reset
- **Extended Runtime:** Two-cell capacity supports 4-6 hours of continuous gameplay
- **Portability:** Lightweight, rechargeable design for on-the-go competition

---

## 🔌 Hardware Wiring Guide

### Port B (Player 1 Buttons & Buzzer)
| Arduino Pin | ATmega328P Bit | Component | Direction |
|-------------|----------------|-----------|-----------|
| 8  | PB0 | P1 Button 0 (Red) | Input |
| 9  | PB1 | P1 Button 1 (Blue) | Input |
| 10 | PB2 | P1 Button 2 (Green) | Input |
| 11 | PB3 | P1 Button 3 (White) | Input |
| 13 | PB5 | Piezo Buzzer | Output |

### Port C (Player 2 Buttons)
| Arduino Pin | ATmega328P Bit | Component | Direction |
|-------------|----------------|-----------|-----------|
| A0 | PC0 | P2 Button 0 (Red) | Input |
| A1 | PC1 | P2 Button 1 (Blue) | Input |
| A2 | PC2 | P2 Button 2 (Green) | Input |
| A3 | PC3 | P2 Button 3 (White) | Input |

**Note:** Analog pins (A0-A3) are used as digital inputs, providing efficient hardware utilization on the ATmega328P.

### Port D (LEDs & Bluetooth)
| Arduino Pin | ATmega328P Bit | Component | Direction |
|-------------|----------------|-----------|-----------|
| 2  | PD2 | Bluetooth RX (HC-05) | Input |
| 3  | PD3 | Bluetooth TX (HC-05) | Output |
| 4  | PD4 | LED 0 (Red) | Output |
| 5  | PD5 | LED 1 (Blue) | Output |
| 6  | PD6 | LED 2 (Green) | Output |
| 7  | PD7 | LED 3 (White) | Output |

### I2C (LCD Display)
| Arduino Pin | Component | Protocol |
|-------------|-----------|----------|
| A4 (SDA)    | 20x4 LCD I2C Module | I2C Data Line |
| A5 (SCL)    | 20x4 LCD I2C Module | I2C Clock Line |

**I2C Address:** 0x27 (standard for PCF8574 I2C backpacks)

### Bluetooth Module (HC-05)
- **VCC:** 5V
- **GND:** GND
- **RX (receives from Arduino TX):** Arduino Pin 3 (PD3)
- **TX (sends to Arduino RX):** Arduino Pin 2 (PD2)
- **Baud Rate:** 9600
- **Pairing:** Default PIN 1234 for wireless connectivity

---

## 🧪 Testing & Results

### Breadboard Phase
The system was initially assembled and tested on a breadboard to verify component functionality:
- ✅ All LEDs respond to microcontroller signals
- ✅ Push buttons register presses accurately with debouncing
- ✅ LCD displays text and menu options correctly
- ✅ Buzzer produces distinct success/failure tones
- ✅ Bluetooth module pairs and receives data
- ✅ EEPROM successfully stores and retrieves high scores

### Perfboard Implementation
After breadboard validation, components were transferred to a perfboard (PCB-like substrate) for durability and portability:
- All connections carefully soldered for reliability
- System operated flawlessly during extended gameplay sessions
- No signal degradation or connection failures observed
- Compact form factor suitable for tournament play

### Functional Testing Results
| Feature | Test Case | Result | Status |
|---------|-----------|--------|--------|
| Button Input | Press each button 5 times per mode | All detected correctly | ✅ Pass |
| LED Sequence | Generate 20+ step sequences | Playback accurate | ✅ Pass |
| Debouncing | Rapid button taps (< 50ms apart) | Single input registered | ✅ Pass |
| LCD Display | Update scores in real-time | Refresh rate 60Hz | ✅ Pass |
| Audio Feedback | Buzzer during win/loss | Correct frequencies | ✅ Pass |
| EEPROM Persistence | Restart after high score | Data recovered correctly | ✅ Pass |
| Multiplayer Sync | 3-player simultaneous inputs | Winner detected first | ✅ Pass |
| Bluetooth | Send Player 3 input via HC-05 | Integration seamless | ✅ Pass |
| Power Idle | No interaction for 2 minutes | System enters sleep mode | ✅ Pass |

**Overall Assessment:** All systems operational. Ready for production deployment.

---

## ⚡ Challenges Faced & Key Learnings

### Hardware Debugging - LCD Failure
**Challenge:** The LCD module displayed nothing—no characters, no backlight—despite correct wiring and code.

**Root Cause:** The LCD module was physically damaged during shipping/storage.

**Resolution:** Component was replaced with a functioning unit. Display immediately worked after replacement.

**Learning:** In embedded systems, always verify hardware before debugging software. Use systematic component isolation and test each module independently before integration.

---

### Resource Optimization - Memory Constraints
**Challenge:** The ATmega328P has only 2KB of SRAM. Naive array indexing and string handling quickly consumed precious memory, leaving insufficient space for game arrays and competitive race logic.

**Resolution:** Implemented pointer arithmetic (`*(ptr + i)` instead of `arr[i]`) for sequence comparison in `compareSequences()`. This reduced CPU cycles per operation and allowed the compiler to generate tighter code.

**Learning:** On resource-constrained microcontrollers, low-level optimizations (bit manipulation, pointer arithmetic) are not premature optimization—they're essential. Every byte counts.

---

### Simultaneous Input Polling - Race Condition
**Challenge:** Managing 3 players' inputs simultaneously required non-blocking logic. Initial attempts used blocking `delay()` calls, which prevented real-time race detection and allowed unfair advantages.

**Resolution:** Rewrote input handlers (`getP1Instant()`, `getP2Instant()`, `getP3Instant()`) using falling-edge detection and state tracking. The main race loop now polls all inputs in parallel without blocking.

**Learning:** Real-time systems demand non-blocking architectures. Blocking calls kill responsiveness and fairness.

---

### Bluetooth Integration - Serial Port Conflict
**Challenge:** Integrating a Bluetooth module for Player 3 without sacrificing the hardware serial port (needed for debugging and development).

**Resolution:** Used `SoftwareSerial` on Pins 2 & 3 to create a secondary UART for the HC-05 module. The primary UART (Pins 0 & 1) remains available for the Serial Monitor, allowing real-time debugging.

**Learning:** Software serial communication is viable for low-bandwidth applications (9600 baud game input). Plan your hardware resources early to avoid future conflicts.

---

### PCB Manufacturing Delays
**Challenge:** Custom PCB production was delayed due to high demand at manufacturing facilities.

**Resolution:** Opted to solder components onto an unmapped perfboard substrate, creating a stable, production-ready circuit without waiting for PCB fabrication.

**Learning:** In embedded systems projects, have backup manufacturing options ready. Perfboards are viable alternatives when custom PCBs are unavailable.<img width="854" height="636" alt="WhatsApp Image 2026-04-29 at 9 38 26 PM" src="https://github.com/user-attachments/assets/55e2fe0e-a390-4f9f-98d0-d562cbd5d9a4" />
<img width="745" height="566" alt="WhatsApp Image 2026-04-28 at 10 05 07 PM" src="https://github.com/user-attachments/assets/c82cdfd9-9a2f-4e8a-9516-4e32909d7a99" />
<img width="565" height="475" alt="WhatsApp Image 2026-04-28 at 10 04 29 PM" src="https://github.com/user-attachments/assets/303fdd56-6585-45b8-9b41-8dbd0b536b0c" />
<img width="644" height="498" alt="WhatsApp Image 2026-04-28 at 10 04 07 PM" src="https://github.com/user-attachments/assets/6b1ce52d-1e31-4f91-bce4-0e096136a0e4" />


---

## 🚀 How to Run and Flash the Project

### Prerequisites
- **Arduino IDE** (version 1.8.0 or later) installed on your computer
- **Required Libraries:**
  - `LiquidCrystal_I2C` (for LCD communication via I2C)
  - `EEPROM` (standard Arduino library for persistent storage)
  - `SoftwareSerial` (standard Arduino library for secondary UART)
- **Hardware:** Arduino Uno with all components wired according to the [Hardware Wiring Guide](#-hardware-wiring-guide)
- **USB Cable:** Type A to Type B for Arduino Uno

### Installation Steps

#### 1. Clone the Repository
```bash
git clone https://github.com/joe-sq/Memory-Game-on-Arduino.git
cd Memory-Game-on-Arduino
```

#### 2. Install Required Libraries (if not already present)
- Open Arduino IDE → Sketch → Include Library → Manage Libraries
- Search for `LiquidCrystal_I2C` and install the library by Frank de Brabander
- Verify that `EEPROM` and `SoftwareSerial` are available (they ship with Arduino)

#### 3. Open the Project
- In Arduino IDE, open the **MENU.ino** file (this is your main entry point)
- All other .ino files will load as tabs in the same sketch

#### 4. Verify Your Hardware Configuration
- Review **hardware.h** to confirm pin assignments match your physical wiring
- Update `#define` directives if you've used different pins (e.g., different I2C address for LCD)

#### 5. Configure the LCD I2C Address (if needed)
In **display.ino**, locate this line:
```cpp
LiquidCrystal_I2C lcd(0x27, 20, 4);
```
- `0x27` is the standard I2C address for PCF8574 backpacks
- If your LCD doesn't respond, use an I2C scanner sketch to find the correct address and update this line

#### 6. Connect Arduino & Upload
- Connect your Arduino Uno to your computer via USB cable
- In Arduino IDE, go to Tools → Board → select "Arduino Uno"
- Select Tools → Port → choose the COM port for your Arduino (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
- Click the **Upload** button (checkmark icon) or press Ctrl+U
- Wait for the message `Done uploading` to appear

#### 7. Verify Operation
- Open Tools → Serial Monitor (Ctrl+Shift+M)
- Set baud rate to 9600
- You should see startup messages like `"BT System Active. DDRD configured for Pins 2 & 3."`
- Press a button on Player 1's keypad; you should see `"P1 Button Pressed"` in the Serial Monitor
- The 20x4 LCD should display the main menu

---

## 🔮 Future Improvements

### Short Term
- [ ] **Difficulty Presets:** Add menu options for Easy/Normal/Hard mode affecting initial sequence length and LED timing
- [ ] **Sound Effects Library:** Expand buzzer feedback with more tones (e.g., round transition sounds, countdown beeps)
- [ ] **Score Display Animations:** Add scrolling text or ASCII art animations for leaderboard transitions
- [ ] **Custom Sequence Length:** Allow players to set their own target difficulty in Challenger mode (currently fixed at 4 steps)

### Medium Term
- [ ] **Wireless Multi-Device:** Enhance Bluetooth to support multiple simultaneous HC-05 connections (requires protocol layer)
- [ ] **OLED Upgrade:** Replace 20x4 LCD with a 128x64 OLED for richer graphics and animations
- [ ] **Statistics Tracking:** Record per-player statistics (win rate, average sequence length, fastest time) in EEPROM
- [ ] **Power Optimization:** Implement sleep modes for the microcontroller itself (deeper than display backlight dimming)

### Long Term
- [ ] **Game Replay System:** Record and playback sequences for practice or streaming
- [ ] **LED Matrix Upgrade:** Replace 4 discrete LEDs with an 8x8 addressable RGB matrix for visual variety
- [ ] **Custom Firmware Updates:** Implement bootloader-based OTA (Over-The-Air) updates via Bluetooth
- [ ] **Multiplayer Tournament Mode:** Support bracket-style elimination tournaments for 4+ players
- [ ] **Custom PCB Production:** Print professional PCB design for mass production and distribution

---

## 🎮 Comparison with Existing Products

### vs. Simon (Hasbro)

| Feature | Our Game | Simon |
|---------|----------|-------|
| **Game Modes** | 3 (Solo, Duel, Challenger) | 1 (Solo only) |
| **Player Support** | Up to 3 simultaneous | 1 player only |
| **Wireless Capability** | Yes (Bluetooth HC-05) | No |
| **Display Type** | 20x4 LCD + 4 LEDs | 4 LEDs only |
| **Leaderboard** | Persistent EEPROM storage | Score resets on power-off |
| **Audio Feedback** | Multiple tones | Single tone per action |
| **Difficulty Scaling** | Dynamic (decreasing LED time) | Exponential sequence growth |
| **Power Management** | 2-minute sleep mode | No idle sleep |
| **Customization** | Fully open-source and modifiable | Proprietary firmware |
| **Estimated Cost** | ~£623 total (~£233 excluding university resources) | $20-30 retail |

**Verdict:** While Simon is simpler and more affordable, our game provides significantly more features, customization options, and competitive gameplay suited for educational and tournament settings.

---

## 📄 License

This project is released under the **MIT License**. See the [LICENSE](LICENSE) file for details.

You are free to:
- Use this code for personal and commercial projects
- Modify and distribute the code
- Use this code in proprietary applications

**Attribution:** While not required, crediting this project would be appreciated.

---

## 👤 Author & Contact

**Project Contributors:**
- Maryam Gamal Alahmady (24080584)
- Mohamed Abdelhamid (24079047)
- Youssif Saqr (24081414) - Lead Developer
- Youssef Shady (24080686)
- Samir Hany (24080549)

**Lead Repository Maintainer:** Joe S. (@joe-sq)

- **GitHub:** [joe-sq](https://github.com/joe-sq)
- **Project Repo:** [Memory-Game-on-Arduino](https://github.com/joe-sq/Memory-Game-on-Arduino)

Questions, bug reports, or feature requests? Feel free to open a GitHub Issue!

---

## 🙏 Acknowledgments

- **University of Hertfordshire** - School of Engineering and Computer Science for academic supervision and resources
- **Arduino Community** for the fantastic IDE and hardware ecosystem
- **Frank de Brabander** for the LiquidCrystal_I2C library
- **AVR-Libc Documentation** for low-level register programming guidance
- **Simon Game (Hasbro)** - Original inspiration for memory-based gaming mechanics
- **Embedded Systems Community** for best practices in microcontroller design

---

## 📚 References & Resources

- **LCD Documentation:** [LiquidCrystal_I2C Library](https://github.com/johnrickman/LiquidCrystal_I2C)
- **Bluetooth Module:** [HC-05 AT Command Set](https://www.electronicwings.com/sensors-modules/hc-05-bluetooth-module)
- **Push Buttons:** [Gravity Mini Push Button Components](https://www.gravityelectronic.com/)
- **Simon Game:** [Wikipedia Entry](https://en.wikipedia.org/wiki/Simon_(game))
- **AVR Programming:** [AVR Libc Reference Manual](https://www.nongnu.org/avr-libc/)
- **Arduino Uno Pinout:** [Official Arduino Documentation](https://store.arduino.cc/products/arduino-uno-rev3)

---

**Last Updated:** May 20, 2026  
**Status:** Stable (v1.0) - Academic Submission Complete  
**Project Duration:** Spring 2026 (8 weeks)
