# DigiSpark WiFi Stealer Scripts

## sketch_sep20a.ino - DisPark
**Windows PowerShell Evade WiFi Stealer - Webhook Exfiltration**

### Description
A Digispark-based USB HID attack script that:
- Exploits Windows GUI (Win+R) to launch elevated PowerShell
- Bypasses UAC via TAB-TAB-ENTER navigation
- Extracts all WiFi profile names and their clear-text passwords using `netsh wlan`
- Exfiltrates stolen credentials to Discord webhook

### Technical Details
- **Attack Vector:** USB Human Interface Device (HID) emulation
- **Bypass Technique:** Automatic UAC elevation via GUI interaction
- **Data Exfil:** JSON payload sent to Discord webhook endpoint
- **LED Indicator:** Visual confirmation when complete

### Legality Notice
⚠️ **ONLY RUN ON SYSTEMS YOU OWN OR HAVE EXPLICIT WRITTEN AUTHORIZATION**

### Usage
1. Program Digispark with sketch_sep20a.ino
2. Physically connect to target Windows machine
3. Script auto-executes on USB detection
4. WiFi credentials sent to configured webhook
5. LED blinks when complete

### Requirements
- Digispark ATtiny85 microcontroller
- DigiKeyboard library
- Target Windows machine with saved WiFi profiles

### Security Researcher Note
This is a proof-of-concept for security testing. The payload:
- Uses standard Windows networking commands
- Contains no malware or viruses
- Is educational for understanding USB HID attacks
- Should be used for authorized penetration testing only