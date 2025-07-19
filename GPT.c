#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>


// Error Handling
static int HandleErrorDB(int ReturnValue, sqlite3 *DB,  char *Error){
    if (ReturnValue != SQLITE_OK) {
        if (Error) {
            fprintf(stderr, "Error von Datenbank: %s\n", Error);
            sqlite3_free(Error);
        } else {
            fprintf(stderr, "Error von Datenbank: %s\n", sqlite3_errmsg(DB));
        }
        sqlite3_close(DB);
        return 1;
    } 
    return 0;
}

// Callback Funktion zum Daten speichern aus DB
typedef struct {
    int GPTState ;
    int SensorValue;
} SQLData;

SQLData Results = {0};

static int callback(void *data, int NumberColumns, char **ColumnsResults, char **ColumnsNames) {
    if (strcmp(ColumnsNames[0], "speaking") == 0) {
        if (NumberColumns > 0 && ColumnsResults[0]) {
            Results.GPTState = atoi(ColumnsResults[0]);
            return 0;  
        }
    }
    else
        if (NumberColumns > 0 && ColumnsResults[0]) {
            Results.SensorValue = atoi(ColumnsResults[0]);
            return 0;   
        }
    return 1;
}


int GptCommunicationInit() {
    // Variablen initialisieren
    sqlite3 *Datenbank;
    int ReturnSQLLiteDB;
    char *DatenbankError = NULL;
    // SQL Befehle
    const char *sql_Tabelle_Init = "CREATE TABLE IF NOT EXISTS C (Sensorwert INTEGER);" "CREATE TABLE IF NOT EXISTS GPT (speaking BOOLEAN);";
    const char *sql_Zeile_Init_c = "INSERT INTO C (Sensorwert) SELECT 0 WHERE NOT EXISTS (SELECT 1 FROM C);";
    const char *sql_Zeile_Init_gpt = "INSERT INTO GPT (speaking) SELECT 0 WHERE NOT EXISTS (SELECT 1 FROM GPT);";
    
    ReturnSQLLiteDB = sqlite3_open("Database.db", &Datenbank);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, NULL) == 1) {
        return 1;
    }

    // Init Tabellen und Zeilen
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Tabelle_Init, 0, 0, &DatenbankError);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Zeile_Init_c, 0, 0, &DatenbankError);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Zeile_Init_gpt , 0, 0, &DatenbankError);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }

    sqlite3_close(Datenbank);
    return 0;
}

int GptReadState() {
    // Variablen initialisieren
    sqlite3 *Datenbank;
    int ReturnSQLLiteDB;
    char *DatenbankError = NULL;
    // SQL Befehle
    const char *sql_Read_GPT = "SELECT speaking FROM GPT LIMIT 1;";
    

    // DB öffnen 
    ReturnSQLLiteDB = sqlite3_open("Database.db", &Datenbank);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, NULL) == 1) {
        return 1;
    }

    // GPT Lesen
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Read_GPT , callback, 0, &DatenbankError);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }

    sqlite3_close(Datenbank);
    return Results.GPTState;
}

int GptUpdateSensorwert(int Sensorwert) {
    // Variablen initialisieren
    sqlite3 *Datenbank;
    int ReturnSQLLiteDB;
    char *DatenbankError = NULL;
    
    // SQL Befehle generieren
    char sql_Update_Sensorwert[35];
    snprintf(sql_Update_Sensorwert, sizeof(sql_Update_Sensorwert),"UPDATE C SET Sensorwert = %d;", Sensorwert);
    
    // DB öffnen 
    ReturnSQLLiteDB = sqlite3_open("Database.db", &Datenbank);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, NULL) == 1) {
        return 1;
    }

    // GPT Lesen
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Update_Sensorwert , callback, 0, &DatenbankError);
    if (HandleErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }

    sqlite3_close(Datenbank);
    return 0;
}

