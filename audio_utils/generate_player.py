# generate_player.py
import wave
import sys
import struct
import os
# this is for generating the actual player
# have to check for edge cases for safety.

def get_lui_addi_pair(value):
    if value == 0:
        return (0, 0)
    upper = (value >> 12) & 0xFFFFF
    lower = value & 0xFFF
    if lower & 0x800:
        upper += 1
    return (upper, lower)
if not os.path.exists('audio_data.txt'):
    print("Error: audio_data.txt not found.")
    print("Please run wav_to_txt.py first.")
    sys.exit(1)
n_samples = 0
with open('audio_data.txt', 'r') as f:
    n_samples = len(f.readlines())
if n_samples == 0:
    print("Error: audio_data.txt is empty.")
    sys.exit(1)
    
    # different testing modes - we can hear the differnce in audio directly
for mode in [0, 1, 2]: 
    if mode == 0:
        filename ="player_clean.s"
    elif mode == 1:
        filename ="player_glitch.s"
    else:
        filename= "player_corrected.s"

## writing into the .s testing file for our speciifc audio file and performing injectFlip and checkError for testing.

    (count_lui,count_addi) =get_lui_addi_pair(n_samples)
    with open(filename, 'w') as f:
        f.write("main:\n")
        f.write(f"  lui x6, {count_lui}\n")
        f.write(f"  addi x6, x6, {count_addi}\n")
        f.write("  lui x7, 0x10000\n")
        f.write("  lui x8, 0x30000\n")
        f.write("loop:\n")
        f.write("  lwpd x10, 0(x8)\n") 
        if mode == 1: 
            f.write("  injectFlip x10, x10, x0\n")
        elif mode == 2: 
            f.write("  injectFlip x10, x10, x0\n")
            f.write("  checkError x10, x10, x0\n")
        f.write("  sw x10, 0(x7)\n")
        f.write("  addi x6, x6, -1\n")
        f.write("  bne x6, x0, loop\n")
        f.write("  ecall\n") 
    print(f"Generated {filename} with a correct counter of {n_samples}")
