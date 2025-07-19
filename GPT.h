#ifndef GPT_H
#define GPT_H


typedef struct {
    int GPTState ;
    int SensorValue;
} SQLData;

int GptCommunicationInit();
int GptReadState();
int GptUpdateSensorwert(int Sensorwert);


#endif

