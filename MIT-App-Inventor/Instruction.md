# MIT App Inventor - Bluetooth Setup Guide (HC-05)

This guide explains how to properly configure the Bluetooth connection parameters within the MIT App Inventor project to establish a stable wireless connection for Player 3.

---

## 🔍 Important Configuration: MAC Address Update

Before building or installing the application, you **must** update the target hardware address within the app blocks to match your specific HC-05 transceiver.

### How to Find Your HC-05 MAC Address
1. Power on the Arduino console.
2. Open your smartphone's standard Bluetooth settings menu.
3. Scan for devices and locate your module (usually named `HC-05`).
4. Click on the device details to find its unique 12-character physical hardware address formatted like this: `00:21:13:02:66:B8`.

### Updating the Address in MIT App Inventor
1. Open your project in the [MIT App Inventor Designer](http://ai2.appinventor.mit.edu/).
2. Switch to the **Blocks** editor view in the top-right corner.
3. Locate the initialization block or the connection click event block (`when Connect_Button.Click`).
4. Replace the default placeholder string text block with your specific HC-05 MAC address.

---

## ⚠️ Critical Protocol Note: BLE vs. Bluetooth Classic

The **HC-05 module is a Bluetooth Classic (BR/EDR) device**, *not* a Bluetooth Low Energy (BLE) module. 

### MIT App Inventor Component Requirements:
* **Do Not Use the BLE Extension:** Do not download or use the `edu.mit.appinventor.ble` extension for this specific hardware setup. 
* **Use the Native BluetoothClient:** You must use the built-in **`BluetoothClient`** component located under the *Palette -> Connectivity* tab. The native component is specifically designed to manage standard serial communication (SPP) profiles used by the HC-05.

---

## 🍏 iOS Devices & Dynamic MAC Addressing

If you plan to port or test this application layout on Apple iOS devices (using the MIT AI2 Companion for iOS), you must take note of iOS security protocols regarding peripheral discovery:

* **MAC Address Masking:** To protect user privacy and prevent cross-device tracking, iOS completely masks and hides the true hardware MAC address of standard Bluetooth Classic peripherals.
* **Dynamic UUID Assignment:** Instead of exposing a fixed MAC address (like `00:21:13:...`), iOS generates a unique, randomized software identifier known as a **UUID** (Universally Unique Identifier) for the connected device.
* **Per-Device Variance:** This generated UUID is unique **to that specific iOS device**. If two different iPhones scan the exact same memory game console, each iPhone will display a completely different identifier string for the same HC-05 module.

### How to handle this in development:
If testing on iOS, do not rely on hardcoded MAC address strings in your blocks. Instead, configure your app user interface to use a **`ListPicker`** component. This allows the player to dynamically scan, see the human-readable name (`HC-05`), and manually select the module from the active list at runtime, bypassing hardcoded address limitations entirely.
