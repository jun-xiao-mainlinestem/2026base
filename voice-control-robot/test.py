import whisper
import pyaudio
import wave

# Audio settings
FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 1024
RECORD_SECONDS = 5
WAVE_OUTPUT_FILENAME = "output.wav"

def record_audio():
    audio = pyaudio.PyAudio()

    print("🎙️ Recording... Speak now!")

    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)

    frames = []

    for _ in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)

    print("✅ Recording finished.")

    stream.stop_stream()
    stream.close()
    audio.terminate()

    # Save to .wav file
    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(audio.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()

def transcribe_audio():
    print("🔍 Transcribing...")
    model = whisper.load_model("tiny")  # or "base", "small"
    result = model.transcribe(WAVE_OUTPUT_FILENAME)
    print("📝 Transcription:")
    print(result["text"])

if __name__ == "__main__":
    record_audio()
    transcribe_audio()
