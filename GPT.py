#!/home/keno/Desktop/GPT/venv python3

from openai import OpenAI
import sounddevice as sd
import numpy as np
import speech_recognition as sr
import os
import sqlite3


# Initialisieren
client = OpenAI(api_key="")
messages = [{"role": "system", "content": "Du bist eine lustige Pflanze, die gerne Witze macht und Aufmerksamkeit braucht"}]
AKTIVIERUNGSWORT = "pflanze"


# Funktionen
def Update_State_DB(speaking: int):
    Database = None
    try:
        Database = sqlite3.connect("Database.db") 
        cursor = Database.cursor()
        # speaking schreiben
        cursor.execute("UPDATE GPT SET speaking = ?;", (speaking,))
        Database.commit()
        #print("GPT Status in DB aktualisiert.")
    except sqlite3.Error as e:
        print(f"Fehler beim Schreiben in die Datenbank: {e}")
    finally:
        if Database:
            Database.close()

def record_audio(duration=3, fs=16000):
    #print(f"Aufnahme ({duration} Sekunden)...")
    recording = sd.rec(int(duration * fs), samplerate=fs, channels=1, dtype='int16')
    sd.wait()
    return np.squeeze(recording)

def recognize_speech(audio, fs=16000):
    recognizer = sr.Recognizer()
    audio_data = sr.AudioData(audio.tobytes(), fs, 2)
    try:
        return recognizer.recognize_google(audio_data, language="de-DE")
    except sr.UnknownValueError:
        return ""
    except sr.RequestError as e:
        print(f"Spracherkennung fehlgeschlagen: {e}")
        return ""

# Auslöser für Chat
def warte_auf_schlagwort():
    #print(f"Sage das Aktivierungswort („{AKTIVIERUNGSWORT}“), um den Chat zu starten.")
    while True:
        audio = record_audio(duration=3)
        text = recognize_speech(audio)
        if not text:
            #print("Nichts erkannt. Noch einmal versuchen...")
            continue
        print("Erkannt:", text)
        if AKTIVIERUNGSWORT in text.lower():
            #print("Aktivierungswort erkannt! Chat startet...")
            break

# Warte auf das Aktivierungswort
Update_State_DB(speaking = 3)
warte_auf_schlagwort()
os.system(f'espeak -v de "Ich höre"')
#print("Starte Chat-Schleife!")

# Haupt-Chat-Schleife
while True:
    audio = record_audio(duration=5)
    text = recognize_speech(audio)
    Update_State_DB(speaking = 3)
    if not text:
        #print("Nicht verstanden, bitte nochmal versuchen.")
        continue
    #print("Du:", text)
    if text.lower() in ["exit", "quit", "beenden"]:
        #print("Beende den Chat. Bis zum nächsten Mal!")
        break

    messages.append({"role": "user", "content": text})
    try:
        completion = client.chat.completions.create(
            model="o3-mini",  
            messages=messages
        )
        Update_State_DB(speaking = 2)
        reply = completion.choices[0].message.content
    except Exception as e:
        print(f"Fehler bei der Anfrage an OpenAI: {e}")
        continue

    print("Pflanze:", reply)
    os.system(f'espeak -v de "{reply}"')
    messages.append({"role": "assistant", "content": reply})
 
