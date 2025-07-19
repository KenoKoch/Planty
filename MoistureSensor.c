#include <stdio.h>
#include <gpiod.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#define I2C_BUS "/dev/i2c-1"
#define ADS1115_ADDR 0x48

float ReadMoisture(){
    // Variablen deklarieren
    int i2c_fd;
    unsigned char config[3];
    unsigned char data[2];
    int16_t raw_value;
    float voltage;

    // I2C-Bus öffnen
    if ((i2c_fd = open(I2C_BUS, O_RDWR)) < 0) 
    {
        perror("I2C-Bus kann nicht geöffnet werden");
        return 1;
    }

     // ADS1115 als Slave-Device auswöhlen
    if (ioctl(i2c_fd, I2C_SLAVE, ADS1115_ADDR) < 0) 
    {
        perror("Fehler bei Slave-Adresszuweisung");
        close(i2c_fd);
        return 1;
    }
 
    // Konfigurationsregister setzen 
    config[0] = 0x01;
    config[1] = 0xC3; 
    config[2] = 0x83; 

    if (write(i2c_fd, config, 3) != 3) 
    {
        perror("Fehler beim Schreiben der Konfiguration");
        return 1;
    }

    usleep(2000); // Warte auf Konversion

    // Zeiger auf Konversionsregister setzen
    unsigned char reg[1] = {0x00};
    if (write(i2c_fd, reg, 1) != 1) 
    {
        perror("Fehler beim Setzen des Zeigers");
        return 1;
    }

    // Messwert lesen
    if (read(i2c_fd, data, 2) != 2) 
    {
        perror("Fehler beim Lesen der Daten");
        return 1;
    }

    raw_value = (data[0] << 8) | data[1];
    voltage = (raw_value * 4.096) / 32767.0;

    close(i2c_fd);
    return voltage;
}

bool DetectMoistureRaise (int SensorValue) {
    // Variablen init
    static int OldSensorValue = 0;
    static time_t Oldseconds = 0;
    static int initialized = 0;
    int SensorChangeValue = 3;
    time_t TimeChangeValue = 2;
    
    //aktuellen Zeit auslesen und Feuchtigkeitsänderung berechnen
    time_t Currentseconds = time(NULL); 
    int DifferenceSensor = OldSensorValue - SensorValue ;
    time_t DifferenceTime = Currentseconds - Oldseconds ;

    if (!initialized) {
        OldSensorValue = SensorValue;
        Oldseconds = Currentseconds;
        initialized = 1;
        return false;
    }

    if (DifferenceTime >= 5) {
        Oldseconds = Currentseconds;
        
        if (DifferenceSensor >= SensorChangeValue ){
            OldSensorValue = SensorValue;
            return true;
        } else {
            OldSensorValue = SensorValue;
            return false;
        }

    } 
    return false;

}