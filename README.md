# K40_Display_with_temp_waterflow
K40 Laser cutter display with water flow and temperature sensor

This is a project to put some safety and automation to the K40 Laser cutter.
The Arduino checks with the flow rate sensor the water flow. If the flow is under 30 l/h Relay 2 is active.
This can activate a warning light or acoustic feedback.
Also is controls a "cooling system" for water if the water temperature is above 22°C Relay 1 is active.
You can change the value of the temperature set point with two buttons from 15°C to 25°C.
If both buttons pushed at the same time is set to the default temperature 22°C.
Relay 1 can switch a cooler or optical / acoustical feedback.

Components:
	- 4 Channel Relay Board Module with Optocoupler (5V)
	- 1.3" 128x64 OLED LCD Display Module IIC I2C Interface 3.3-5V
	- DS18B20 Temperature Sensor
	- YF-S401 Water Flow Sensor
	- Arduino Nano (clone) or compatible
	- 2x Buttons
	- 4x 10kOhn Resistor
	- 4k7 Resistor
