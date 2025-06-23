#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "Animation.h"
#include "Frames.h"
#include "MoistureSensor.h"
#include "GPT.h"



int main() {

    // Variablen initialisieren
    int CounterTillBlink = 0;
    Pictures Ladebalken = Ladebalken1_Pictures;
    bool GPT_State = false;
    
    // GPT Python Skript starten
    int Successfull = system("GPT.py &");
    if (Successfull == -1) {
        perror("Fehler beim Starten von GPT.py");
    } else {
        printf("GPT.py gestartet.\n");
    }

    while(1){

        // Sensorwert auslesen und in DB schreiben
        float Moisture = ReadMoisture();
        int MoistureCase = (int)(Moisture * 10);
        GPT_Update_Sensorwert(MoistureCase);
       
        // GPT Status abfragen
        if (GPT_Communication_init() == 0) {
            GPT_State = (GPT_Read_State() == 2);
        }

        // Merker Case von Pi File kopieren
        switch(MoistureCase){
            case 13:
                Ladebalken = Ladebalken15_Pictures;
                break;
            case 14:
                Ladebalken = Ladebalken15_Pictures;
                break;
            case 15:
                Ladebalken = Ladebalken14_Pictures;
                break;
            case 16:
                Ladebalken = Ladebalken13_Pictures;
                break;
            case 17:
                Ladebalken = Ladebalken12_Pictures;
                break;
            case 18:
                Ladebalken = Ladebalken11_Pictures;
                break;
            case 19:
                Ladebalken = Ladebalken10_Pictures;
                break;
            case 20: 
                Ladebalken = Ladebalken9_Pictures;
                break;
            case 21: 
                Ladebalken = Ladebalken8_Pictures;
                break;
            case 22:
                Ladebalken = Ladebalken7_Pictures;
                break;
            case 23:
                Ladebalken = Ladebalken6_Pictures;
                break;
            case 24:
                Ladebalken = Ladebalken5_Pictures;
                break;
            case 25:
                Ladebalken = Ladebalken4_Pictures;
                break;
            case 26:
                Ladebalken = Ladebalken3_Pictures;
                break;
            case 27:
                Ladebalken = Ladebalken2_Pictures;
                break;
            case 28:
                Ladebalken = Ladebalken1_Pictures;
                break;
            default:
                Ladebalken = Ladebalken1_Pictures;
                break;
        }

        if (GPT_State) {
            animate_dual(&Speak_Pictures,&Ladebalken,  MoistureCase ,200);
        }
        else {
            if (Moisture > 2.1) 
            {
                animate_dual(&Sad_Pictures,&Ladebalken,  MoistureCase ,200);
            }
            else if  (Moisture <= 2.1  && Moisture >= 1.8) 
            {
                animate_dual(&Okay_Pictures,&Ladebalken,  MoistureCase ,200);
            }
            else if  (Moisture < 1.8) 
            {
                animate_dual(&Happy_Pictures,&Ladebalken,  MoistureCase ,200);
            } 
        }

        // Alle drei animationen blinzeln
        CounterTillBlink += 1;
        if (CounterTillBlink >= 3) {
            animate_dual(&Blink_Pictures,&Ladebalken,  MoistureCase, 200);
            CounterTillBlink = 0;
        }

        usleep(100000);
    }
    return 0;

}