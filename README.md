# Planty

#### Video Demo:  https://youtu.be/ZDwLqy_1ssQ

#### Description:

Plant Moisture Monitor with ChatGPT Integration  
This program runs on a Raspberry Pi with a connected display, continuously measures the moisture level of a plant and visualizes the plant's status through various animations. It also integrates ChatGPT allowing interactive conversations with your plant.

Features
- Continuously reads the plant's moisture level using a sensor.
- Displays different animations on the screen reflecting the plant's current moisture state. Happy, okay, or sad faces depending on moisture level.
- A progress bar visualizing the current moisture.
- Special animations during watering or when ChatGPT is active.
- A Python script (GPT.py) is launched in parallel, enabling communication with ChatGPT. You can interact and "talk" with your plant.

#### Requirements
Hardware  
Raspberry Pi (any model with GPIO support)  
Moisture sensor and ADS1115 analog digital converter  
Display for showing animations  

Software  
C compiler (gcc) to compile the main program  
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
bash  
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

#### Description of Files:
##### Main.c
This file is the main entry point of the application. It starts the GPT Python script and controls animations. The file coordinates the overall program flow by calling functions defined in other source files.

##### Animation.c
This file contains the function responsible for animating frames defined in Frames.c (stored as string arrays) using the ncurses library. It displays two animations beside each other by iterating over each row of the current frame, in both animations, and printing the rows to the terminal using the mvprintw function. The function calculates the maximum numbers of frames in both animations and loops the shorter animation as long as the longer one is finshed. Also a decimal variable can be displayed alongside the second animation.

##### Frames.c
In this file the frames used for the animation function in animation.c are defined. The frames are organized in a custom Pictures struct that contains an array of string arrays (array of char arrays). Each string array represents one frame of ASCII art, which together form the animation sequence. Additionally the struct stores information such as the number of frames, the number of rows and the number of columns per frame, allowing the animation functions to properly iterate over it.

##### MoistureSensor.c
This file contains the logic to read the current sensor value using an ADS1115 analog digital converter. It communicates with the ADS1115 with the Linux I2C bus interface (/dev/i2c-1). The code opens the communication interface, sets the correct configurations to the converter and reads the raw measurement value. The raw value is then converted to a voltage representing the sensor reading. Additionally the file includes a function to detect a decrease in the sensor value which corresponds to a rise in moisture.

##### GPT.c
In this file the communication between the GPT.py script and the main C program is implemented using an SQLite database. It provides a function (GPT_Communication_init) to create the database on the first call and initialize it with two tables named GPT and C. Each table contains a single entry. The C table stores an integer named Sensorwert and the GPT table holds a boolean named speaking. The file has two more functions called GPT_Read_State and GPT_Update_Sensorwert. The first one reads the value of speaking from the table GPT and the second one updates the current sensor value to the Sensorwert column of the C table.

##### GPT.py
This script implements an integration of the OpenAI o3-mini Model using their API (openai libary). It captures audio input using voice activity detection (webrtcvad library) to detect speech segments and converts them to text with Google’s speech recognition (speech_recognition library). The function WaitForKeyword checks if the predefined activation keyword (pflanze) is in the converted text and starts the chat. During the main loop it reads the current sensor value from the database (ReadSensorwertDB) and sends it along with detected text to the OpenAI model. Also the UpdateStateDB function sets the bool speaking in the SQLite database to true or false depending on whether a response is outputted or not. The responses from the OpenAI API are spoken aloud via text-to-speech using espeak. 

#### Notes
Moisture sensor values are typically in the range of 0.8 to 2.2 (depending on the sensor) you can adapt it in MoistureSensor.c or in main.c switch Statement  
ChatGPT integration requires an internet connection and a valid API key (the API key must be definded in an .env file).

