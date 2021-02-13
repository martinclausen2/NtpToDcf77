# NtpToDcf77
Receive time and date via WLAN / NTP and transmit wired as DCF77

The code is based on various sources freely available.

# Design Objectives
- Transmit precise time and date information to clock even if no DCF signal is receiveable (e.g. because the clock itself or other devices might interfer with the reception or the signal is simply to weak)
- Use local fritz box as NTP server (to use other NTP servers please adjust code)
- WPS button method for connection, no hard coded credentials
- Low cost
  - Cheap and few components
  - Linear instaead of switching voltage regulator
- Power saving
  - Time is only send for 2 houres very 20 hours, during the later the WLAN is switched off

# Active Components
- ESP-01 WLAN module
- LF33 voltage regulator
  - Operating input voltage up to 16V 
  - Suffcient output current for ESP-01

# Tools
- Schematic KiCAD (no layout yet)
- Adurino IDE

# Applications
Update you older [word clock](https://www.mikrocontroller.net/articles/Word_Clock_Variante_1) or other clock with DCF77 input
