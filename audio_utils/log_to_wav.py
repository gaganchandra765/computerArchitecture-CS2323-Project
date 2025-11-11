# log_to_wav.py
import sys
import numpy as np
from scipy.io.wavfile import write
# this is for actually creating the .wav file
if len(sys.argv) != 2:
    print("Usage: python log_to_wav.py <input.log>")
    sys.exit(1)

log_file = sys.argv[1]
wav_file = log_file.replace('.log', '.wav')

samples = []
with open(log_file, 'r') as f:
    for line in f:
        try:
            val = int(line.strip())
            if val > 0x7FFFFFFF:
                val -= 0x100000000
            samples.append(val)
        except ValueError:
            continue

audio_data = np.array(samples, dtype=np.int32)
max_val = np.max(np.abs(audio_data))
if max_val == 0:
    print("No audio data found.")
    sys.exit(0)

scaled = np.int16((audio_data / max_val) * 32767)
write(wav_file, 44100, scaled) 
print(f"Created {wav_file}")
