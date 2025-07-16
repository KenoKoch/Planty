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
from dotenv import load_dotenv


# Initialisieren

load_dotenv()
client = OpenAI(api_key=os.getenv("OPENAPIKEY"))
messages = [{"role": "system", "content": "Du bist eine lustige Pflanze, die gerne Witze macht und Aufmerksamkeit braucht"}]
AKTIVIERUNGSWORT = "pflanze"


# Funktionen
def UpdateStateDB(speaking: int):
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

def ReadSensorwertDB():
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

def RecordAudioVad(SamplingRate=16000, Aggressiveness=3, PaddingDuration=300, MaxRecordingDuration=10):
    VAD = webrtcvad.Vad(Aggressiveness)
    FrameDuration = 30  # 10, 20 or 30 ms
    FrameSize = int(SamplingRate * FrameDuration / 1000)  # Samples per frame
    PaddingFrames = int(PaddingDuration / FrameDuration)

    AudioStream = sd.InputStream(samplerate=SamplingRate, channels=1, dtype='int16')
    AudioStream.start()

    Frames = collections.deque(maxlen=PaddingFrames)
    VoiceDetected = False
    VoicedFrames = []

    StartTime = time.time()

    while True:
        data, _ = AudioStream.read(FrameSize)
        if len(data) == 0:
            break
        pcm_data = data.tobytes()
        is_speech = VAD.is_speech(pcm_data, sample_rate=SamplingRate)

        if not VoiceDetected:
            Frames.append((pcm_data, is_speech))
            num_voiced = len([f for f, speech in Frames if speech])
            if num_voiced > 0.9 * Frames.maxlen:
                VoiceDetected = True
                # Alle gepufferten Frames speichern
                for f, s in Frames:
                    VoicedFrames.append(f)
                Frames.clear()
        else:
            VoicedFrames.append(pcm_data)
            Frames.append((pcm_data, is_speech))
            num_unvoiced = len([f for f, speech in Frames if not speech])
            if num_unvoiced > Frames.maxlen:
                break

        # Optional: maximale Aufnahmezeit verhindern
        if time.time() - StartTime > MaxRecordingDuration:
            break

    AudioStream.stop()
    AudioStream.close()

    print("Aufnahme beendet.")

    # Alle Frames zusammenfügen
    audio_data = b''.join(VoicedFrames)
    audio_np = np.frombuffer(audio_data, dtype='int16')

    return audio_np


def ConvertAudioToText(audio, fs=16000):
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
        audio = RecordAudioVad(SamplingRate=16000, Aggressiveness=3, PaddingDuration=300, MaxRecordingDuration=10)
        text = ConvertAudioToText(audio)
        if not text:
            #print("Nichts erkannt. Noch einmal versuchen...")
            continue
        print("Erkannt:", text)
        if AKTIVIERUNGSWORT in text.lower():
            #print("Aktivierungswort erkannt! Chat startet...")
            break

# Warte auf das Aktivierungswort
UpdateStateDB(speaking = 3)
warte_auf_schlagwort()
os.system(f'espeak -v de "Ich höre"')
#print("Starte Chat-Schleife!")

# Haupt-Chat-Schleife
while True:
    audio = RecordAudioVad(SamplingRate=16000, Aggressiveness=3, PaddingDuration=300, MaxRecordingDuration=10)
    text = ConvertAudioToText(audio)
    UpdateStateDB(speaking = 3)
    if not text:
        #print("Nicht verstanden, bitte nochmal versuchen.")
        continue
    #print("Du:", text)
    if text.lower() in ["exit", "quit", "beenden"]:
        #print("Beende den Chat. Bis zum nächsten Mal!")
        break

    # Sensorwert an GPT übergeben
    sensorwert = ReadSensorwertDB()
    if sensorwert is not None:
        sensor = f"Der aktuelle Feuchtigkeitswert der Pflanze ist {sensorwert}. 8 = sehr feucht, 22 = sehr trocken"
        messages.append({"role": "system", "content": sensor})

    messages.append({"role": "user", "content": text})
    try:
        completion = client.chat.completions.create(
            model="o3-mini",  
            messages=messages
        )
        UpdateStateDB(speaking = 2)
        reply = completion.choices[0].message.content
    except Exception as e:
        print(f"Fehler bei der Anfrage an OpenAI: {e}")
        continue

    # print("Pflanze:", reply)
    os.system(f'espeak -v de "{reply}"')
    messages.append({"role": "assistant", "content": reply})
 
