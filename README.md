# Planty

#### Video Demo:  <URL HERE>

#### Description:

Plant Moisture Monitor with ChatGPT Integration
This program runs on a Raspberry Pi with a connected display, continuously measures the moisture level of a plant, and visualizes the plant's status through various animations. It also integrates ChatGPT, allowing interactive conversations with your plant.

Features
- Moisture Measurement
- Continuously reads the plant's moisture level using a sensor.
- Displays different animations on the screen reflecting the plant's current moisture state:
- Happy, okay, or sad faces depending on moisture level.
- A progress bar visualizing the current moisture.
- The plant blinks every few cycles.
- Special animations during watering or when ChatGPT is active.
- A Python script (GPT.py) is launched in parallel, enabling communication with ChatGPT. You can interact and "talk" with your plant.

#### Requirements
Hardware  
Raspberry Pi (any model with GPIO support)  
Moisture sensor and ADS1115 analog digital converter  
Display (e.g., LCD or OLED) for showing animations  

Software  
C compiler (e.g., gcc) to compile the main program  
Python 3 with virtual environment (venv) for the GPT script  
Internet connection for ChatGPT requests

Installation & Setup  
1. Compile the Program
Compile the C program using your preferred compiler, for example:
bash
gcc Main.c -o Planty Frames.c Animation.c MoistureSensor.c GPT.c -lncurses

2. Prepare the Python Environment
In your project directory:
bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirementsPy.txt

4. Install xfce4-terminal  
sudo apt-get install xfce4-terminal

5. Setup autostart routine  
Create a .desktop File in your confiq directory on your Raspberry pi (Raspberry OS) that starts the compiled Program when the System is booting   
For Example:
 ```
[Desktop Entry]
Type=Application
Name=PlantyAutostart
Exec=xfce4-terminal --fullscreen --hide-menubar --hide-toolbar --hide-borders --command="bash -c 'sleep 5; cd /Your/Directory/Planty && ./Planty; read'"
Terminal=false
Name[en_GB]=PlantyAutostart
 ```
The main program automatically starts the Python script in the background.

#### Project Structure

/Planty
 ```
├── Main.c                # Main C program
├── Animation.c/.h        # Animation Function
├── Frames.c/.h           # Frames for Animation
├── MoistureSensor.c/.h   # Sensor reading logic and moisture raise detection
├── GPT.c/.h              # ChatGPT communication interface
├── GPT.py                # Python script for ChatGPT integration
├── venv/                 # Python virtual environment
├── requirementsPy.txt    # Python dependencies
├── requirementsC.txt     # C dependencies
└── README.md             # This file
 ```

#### Notes
Moisture sensor values are typically in the range of 0.8 to 2.2 (depending on the sensor) you can adapt it in MoistureSensor.c or in main.c switch Statement  
ChatGPT integration requires an internet connection and a valid API key (the API key must be set in GPT.py).

