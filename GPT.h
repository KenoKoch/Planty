#ifndef GPT_H
#define GPT_H


typedef struct {
    int GPTState ;
    int SensorValue;
} SQLData;

int get_status(const char* status);
int GptCommunicationInit();
int GptReadState();
int GptUpdateSensorwert(int Sensorwert);


#endif

