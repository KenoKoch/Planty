#ifndef MOISTURE_H
#define MOISTURE_H



// Funktion Sensor auslesen
float ReadMoisture();
// Funktion Feuchtigkeitsanstieg erkennen
bool DetectMoistureRaise (int SensorValue);

#endif
