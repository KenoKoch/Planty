#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

// Alte Funktion zum File lesen
int get_status(const char* status) {
    FILE* f = fopen("/home/keno/Desktop/data/state.txt", "r");
    if (!f) return 0;
    char line[64];
    int value = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, status, strlen(status)) == 0) {
            value = atoi(line + strlen(status) + 1);
            break;
        }
    }
    fclose(f);
    return value;
}

// Error DB Funktion
int ErrorDB(int ReturnValue, sqlite3 *DB,  char *Error){
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

// DB und Tabellen erstellen
int GPT_Communication() {
    // Variablen initialisieren
    sqlite3 *Datenbank;
    int ReturnSQLLiteDB;
    char *DatenbankError = NULL;
    // SQL Befehle
    const char *sql_Tabelle_Init = "CREATE TABLE IF NOT EXISTS C (value INTEGER);" "CREATE TABLE IF NOT EXISTS GPT (flag BOOLEAN);";
    const char *sql_Zeile_Init_c = "INSERT INTO C (value) SELECT 0 WHERE NOT EXISTS (SELECT 1 FROM C);";
    const char *sql_Zeile_Init_gpt = "INSERT INTO GPT (flag) SELECT 0 WHERE NOT EXISTS (SELECT 1 FROM GPT);";
    const char *sql_Read_GPT = "SELECT flag FROM GPT LIMIT 1;";

    // DB öffnen 
    ReturnSQLLiteDB = sqlite3_open("mydatabase.db", &Datenbank);
    if (ErrorDB(ReturnSQLLiteDB, Datenbank, NULL) == 1) {
        return 1;
    }

    // Init Tabellen und Zeilen
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Tabelle_Init, 0, 0, &DatenbankError);
    if (ErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Zeile_Init_c, 0, 0, &DatenbankError);
    if (ErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Zeile_Init_gpt , 0, 0, &DatenbankError);
    if (ErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }

    // GPT Lesen
    ReturnSQLLiteDB = sqlite3_exec(Datenbank, sql_Read_GPT , 0, 0, &DatenbankError);
    if (ErrorDB(ReturnSQLLiteDB, Datenbank, DatenbankError) == 1) {
        return 1;
    }

    


    sqlite3_close(Datenbank);
    return 0;
}

