# Save as wav_to_txt.py
# wav file to .txt file for easier file processing . and for testing using a .s file
import wave, struct, sys
samples = []
with wave.open('test.wav', 'rb') as wav:
    n_frames = wav.getnframes()
    frames = wav.readframes(n_frames)
    if wav.getsampwidth() == 2: 
        raw = struct.unpack(f'<{n_frames}h', frames)
        samples = [s << 16 for s in raw] 
    elif wav.getsampwidth() == 4: 
        samples = struct.unpack(f'<{n_frames}i', frames)
with open('audio_data.txt', 'w') as f:
    for s in samples:
        f.write(f"{s}\n")
print(f"Created audio_data.txt with {len(samples)} samples.")
