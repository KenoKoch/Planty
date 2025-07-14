#!/home/keno/Desktop/GPT/venv python3

from openai import OpenAI
import sounddevice as sd
import numpy as np
import speech_recognition as sr
import webrtcvad
import os
import sqlite3
import collections
import time


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

def Read_Sensorwert_DB():
    Database = None
    sensorwert = None
    try:
        Database = sqlite3.connect("Database.db")
        cursor = Database.cursor()
        cursor.execute("SELECT Sensorwert FROM C LIMIT 1;")
        result = cursor.fetchone()
        if result:
            sensorwert = result[0]
    except sqlite3.Error as e:
        print(f"Fehler beim Lesen des Sensorwerts: {e}")
    finally:
        if Database:
            Database.close()
    return sensorwert

def record_audio(duration=3, fs=16000):
    #print(f"Aufnahme ({duration} Sekunden)...")
    recording = sd.rec(int(duration * fs), samplerate=fs, channels=1, dtype='int16')
    sd.wait()
    return np.squeeze(recording)

def record_audio_vad(fs=16000, aggressiveness=3, padding_duration_ms=300, max_recording_duration=10):
    vad = webrtcvad.Vad(aggressiveness)
    frame_duration_ms = 30  # 10, 20 or 30 ms
    frame_size = int(fs * frame_duration_ms / 1000)  # Samples per frame
    num_padding_frames = int(padding_duration_ms / frame_duration_ms)

    stream = sd.InputStream(samplerate=fs, channels=1, dtype='int16')
    stream.start()

    frames = collections.deque(maxlen=num_padding_frames)
    triggered = False
    voiced_frames = []

    start_time = time.time()

    while True:
        data, _ = stream.read(frame_size)
        if len(data) == 0:
            break
        pcm_data = data.tobytes()
        is_speech = vad.is_speech(pcm_data, sample_rate=fs)

        if not triggered:
            frames.append((pcm_data, is_speech))
            num_voiced = len([f for f, speech in frames if speech])
            if num_voiced > 0.9 * frames.maxlen:
                triggered = True
                # Alle gepufferten Frames speichern
                for f, s in frames:
                    voiced_frames.append(f)
                frames.clear()
        else:
            voiced_frames.append(pcm_data)
            frames.append((pcm_data, is_speech))
            num_unvoiced = len([f for f, speech in frames if not speech])
            if num_unvoiced > frames.maxlen:
                break

        # Optional: maximale Aufnahmezeit verhindern
        if time.time() - start_time > max_recording_duration:
            break

    stream.stop()
    stream.close()

    print("Aufnahme beendet.")

    # Alle Frames zusammenfügen
    audio_data = b''.join(voiced_frames)
    audio_np = np.frombuffer(audio_data, dtype='int16')

    return audio_np


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

    # Sensorwert an GPT übergeben
    sensorwert = Read_Sensorwert_DB()
    if sensorwert is not None:
        sensor = f"Der aktuelle Feuchtigkeitswert der Pflanze ist {sensorwert}. 8 = sehr feucht, 22 = sehr trocken"
        messages.append({"role": "system", "content": sensor})

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

    # print("Pflanze:", reply)
    os.system(f'espeak -v de "{reply}"')
    messages.append({"role": "assistant", "content": reply})
 
