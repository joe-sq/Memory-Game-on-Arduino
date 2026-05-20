https://github.com/user-attachments/assets/6ae138a8-4cae-4cb9-8363-480d744d5a68

# Multiplayer Memory Game Console

An interactive, microcontroller-based embedded system designed to challenge and test players' memory and reaction speed. Built on the Arduino Uno platform using AVR programming, this console generates dynamic LED light and tone sequences that players must remember and accurately reproduce. 

---

## 🛠️ Tech Stack & Components

### Software & Architecture
* **Language:** AVR C / C++ (Arduino IDE)
* **Architecture:** State Machine Architecture (for fast response, modularity, and smooth menu transitions)
* **Key Techniques:** Software debouncing (eliminates mechanical button noise), Pointer Arithmetic (optimized memory and execution efficiency for the ATmega328P processor)

### Hardware Components
* **Microcontroller:** Arduino Uno
* **Display:** LCD ($20\times4$) with I2C module (`LiquidCrystal_I2C`)
* **I/O:** 4 Addressable LEDs & 8 Physical Push Buttons (4 per player)
* **Audio Feedback:** Piezo Buzzer (distinct frequencies for success chimes and failure tones)
* **Storage:** Non-volatile Built-in EEPROM
* **Wireless:** HC-05 Bluetooth Module (enables 3-player expansion)
* **Power:** 3.7V Lithium-ion Battery with a dual battery holder

---

## 🎮 Gameplay Modes

1. **Solo Sprint Mode:** A single-player mode where the system generates increasingly complex LED sequences. To scale difficulty dynamically, the duration each LED remains lit decreases as your score increases. High scores are permanently saved to the EEPROM leaderboard.
2. **Synchronous Duel Mode:** Two or three players compete simultaneously against the same generated sequence. The system evaluates inputs and awards points based on both correctness and speed.
3. **Challenger Mode ("Maker vs. Breaker"):** Player 1 inputs a custom secret sequence. Player 2 must replicate it. If Player 2 fails, Player 1 must re-verify their original sequence to claim the point, ensuring fair play.

---

## 💾 EEPROM Memory Management
To elevate the device beyond a basic prototype, the system utilizes the ATmega328P's non-volatile EEPROM memory. Leaderboard rankings and individual high scores are permanently stored directly to memory allocations, preserving game data even after the console is powered completely off or hard-reset.

---

​⚡ Challenges Faced & Key Learnings

​Hardware Debugging: Encountered a faulty initial LCD module that yielded no background brightness or characters despite pristine code configurations. Testing components individually isolated the hardware error, highlighting the necessity of modular debugging in mechatronics.
​Resource Optimization: Utilized fast pointer arithmetic instead of heavy array indexing to handle memory allocation strings efficiently on the constrained ATmega328P processor.

---

## 🚀 How to Run and Flash the Project

### Prerequisites
* Arduino IDE installed.
* Necessary Library: `LiquidCrystal_I2C` and standard `EEPROM` / `SoftwareSerial` libraries.

### Steps
1. Clone this repository:
 ```bash
   git clone https://github.com/joe-sq/Memory-Game-on-Arduino.git
```

2. Open the main controller workspace file in the Arduino IDE.
3. Wire the hardware components according to the pin configurations defined in hardware.h (e.g., LEDs on Port D pins 4–7, Player 1         buttons on Port B pins 0–3).
4. Connect your Arduino Uno via USB, select the correct COM port, and click Upload.
