#include <stdio.h>
#include "Animation.h"
#include "Frames.h"
#include <unistd.h>
#include <stdlib.h>



int main() {
 
    // Animation
    while(1){

        int MoistureCase;
        Pictures Ladebalken;

        if (MoistureCase >= 28) {
            MoistureCase = 13;
        }
        else if (MoistureCase < 13)
        {
            MoistureCase = 13;
        }
                      

        // Ladebalkenbilder als Variable
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
        }

        animate_dual(&Happy_Pictures,&Ladebalken,  MoistureCase ,200);
        MoistureCase ++;
    }
    return 0;

}