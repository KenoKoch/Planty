#ifndef GPT_H
#define GPT_H


typedef struct {
    int GPTState ;
    int SensorValue;
} SQLData;

int get_status(const char* status);
int GPT_Communication_init();
int GPT_Read_State();
int GPT_Update_Sensorwert(int Sensorwert);


#endif

