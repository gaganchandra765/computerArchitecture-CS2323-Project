
# create_wav.py
import numpy as np
from scipy.io.wavfile import write
# seting some sampling rate 
sample_rate = 44100     
duration_sec = 5.0      
frequency_hz = 440       
amplitude = 0.5          
# this is the example audio file for testing 
t = np.linspace(0.,duration_sec,int(sample_rate *duration_sec),endpoint=False)
wave_data =amplitude*np.sin(2 *np.pi*frequency_hz *t)


audio_normalized =wave_data*32767
audio_16bit =audio_normalized.astype(np.int16)
write("test.wav", sample_rate, audio_16bit)
print("Successfully generated 'test.wav' (5 seconds, 440 Hz).")
