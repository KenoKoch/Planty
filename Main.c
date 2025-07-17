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
    bool GptState = false;
    bool WateringState = false;

    // GPT Python Skript starten
    int Successfull = system("./venv/bin/python ./GPT.py &");
    if (Successfull == -1) {
        perror("Fehler beim Starten von GPT.py");
    } 

    while(1){

        // Sensorwert auslesen und in DB schreiben
        float Moisture = ReadMoisture();
        int MoistureCase = (int)(Moisture * 10);
        int MoistureValue = 0;
        GptUpdateSensorwert(MoistureCase);

        // Umrechnen von MoistureCase in 0 bis 15
        MoistureValue = 23 - MoistureCase ;
       
        // GPT Status abfragen
        if (GptCommunicationInit() == 0) {
            GptState = (GptReadState() == 2);
        }

        // Abfragen ob gerade gegossen wird
        WateringState = DetectMoistureRaise(MoistureCase);

        // Merker Case von Pi File kopieren
        switch(MoistureCase){
            case 8:
                Ladebalken = Ladebalken15_Pictures;
                break;
            case 9:
                Ladebalken = Ladebalken14_Pictures;
                break;
            case 10:
                Ladebalken = Ladebalken13_Pictures;
                break;
            case 11:
                Ladebalken = Ladebalken12_Pictures;
                break;
            case 12:
                Ladebalken = Ladebalken11_Pictures;
                break;
            case 13:
                Ladebalken = Ladebalken10_Pictures;
                break;
            case 14:
                Ladebalken = Ladebalken9_Pictures;
                break;
            case 15:
                Ladebalken = Ladebalken8_Pictures;
                break;
            case 16:
                Ladebalken = Ladebalken7_Pictures;
                break;
            case 17:
                Ladebalken = Ladebalken6_Pictures;
                break;
            case 18:
                Ladebalken = Ladebalken5_Pictures;
                break;
            case 19:
                Ladebalken = Ladebalken4_Pictures;
                break;
            case 20: 
                Ladebalken = Ladebalken3_Pictures;
                break;
            case 21: 
                Ladebalken = Ladebalken2_Pictures;
                break;
            case 22:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 23:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 24:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 25:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 26:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 27:
                Ladebalken = Ladebalken1_Pictures;
                break;
            case 28:
                Ladebalken = Ladebalken1_Pictures;
                break;
            default:
                Ladebalken = Ladebalken1_Pictures;
                break;
        }

        if (WateringState) {
            AnimateDual(&Rain_Pictures,&Ladebalken, MoistureValue ,300);
        }
        else if (GptState)
        {
            AnimateDual(&Speak_Pictures,&Ladebalken,  MoistureValue ,200);
        } else {
            if (Moisture > 2.1) 
            {
                AnimateDual(&Sad_Pictures,&Ladebalken,  MoistureValue ,200);
            }
            else if  (Moisture <= 2.1  && Moisture >= 1.8) 
            {
                AnimateDual(&Okay_Pictures,&Ladebalken,  MoistureValue ,200);
            }
            else if  (Moisture < 1.8) 
            {
                AnimateDual(&Happy_Pictures,&Ladebalken,  MoistureValue ,200);
            } 
        }

        // Alle drei animationen blinzeln
        CounterTillBlink += 1;
        if (CounterTillBlink >= 3) {
            AnimateDual(&Blink_Pictures,&Ladebalken,  MoistureValue, 200);
            CounterTillBlink = 0;
        }

        //usleep(100000);
    }
    return 0;

}