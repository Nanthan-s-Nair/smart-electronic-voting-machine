 smart-electronic-voting-machine
 
"Introducing the SecureVote System: A Smart Electronic Voting Machine with Fingerprint Verification. Elevate your voting experience ensuring each vote is unique and secure.

Table of Contents:
Connections

1.1 Switches (S1-S5)
1.2 Fingerprint Sensor
1.3 I2C LCD Display

Working

2.1 Initialization
2.2 Setup Function
2.3 Fingerprint Enrollment (enrollFingerprint Function)
2.4 Main Loop (loop Function)
2.5 Fingerprint Verification (checkFingerprint Function)
2.6 Winner Determination

Overview:
The provided code implements a secure electronic voting system with fingerprint verification. Candidates are represented by physical buttons (S1-S4), and the final voting is confirmed with a separate button (S5). A fingerprint sensor ensures that each voter can cast only one vote. The system displays real-time voting information on an I2C LCD and determines the winner based on the maximum vote count. Fingerprint enrollment is handled during the setup phase. This document provides a detailed description of the code, covering connections, initialization, setup, and the overall workflow.

1. Connections:

1.1 Switches (S1-S5):
Connect to digital pins (8, 7, 6, 5, 4).
Represent candidates A, B, C, D, and the final voting button, respectively.

1.2 Fingerprint Sensor:
Connect to digital pins 2 (RX) and 3 (TX) for software serial communication.
Captures and processes fingerprint data for verification.

1.3 I2C LCD Display:
Connect to the appropriate I2C pins.
Specific connections depend on the I2C adapter used (commonly A4 for SDA, A5 for SCL).

2. Working:
   
2.1 Initialization:
Include essential libraries and define pins and variables.
Prepares the system for proper functionality.

2.2 Setup Function:
Configure pins, initialize communication, and display a startup message.
Verify the presence of the fingerprint sensor.
Call enrollFingerprint for initial fingerprint enrollment.

2.3 Fingerprint Enrollment (enrollFingerprint Function):
Users input a unique ID for their fingerprint.
Two fingerprint images are captured for model creation.
The model is converted, and both model and ID are stored.

2.4 Main Loop (loop Function):
Continuously updates the LCD with current vote counts.
Waits for button presses, increments associated vote counts, and initiates fingerprint verification.
Resets fingerprint verification after each vote.

2.5 Fingerprint Verification (checkFingerprint Function):
Users place their finger on the sensor.
Checks if the fingerprint has been used for voting.
Prompts for a new fingerprint ID if needed.
Matches the fingerprint and allows the user to vote.

2.6 Winner Determination:
Updates the LCD with current vote counts after each vote.
Determines the winner based on the maximum vote count when the final vote button (S5) is pressed.
Displays the result on the LCD and resets vote counts.
