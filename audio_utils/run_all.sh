#!/usr/bin/env bash

set -e  

python ../audio_utils/wav_to_text.py
python ../audio_utils/generate_player.py

echo "running player_clean.s"
./vm --run player_clean.s
mv -f audio_out.log clean.log

echo "running player_glitch.s"
./vm --run player_glitch.s
mv -f audio_out.log glitch.log

echo "running player_corrected.s"
./vm --run player_corrected.s
mv -f audio_out.log corrected.log

echo "converting logs to WAV"
python ../audio_utils/log_to_wav.py clean.log
python ../audio_utils/log_to_wav.py glitch.log
python ../audio_utils/log_to_wav.py corrected.log

echo "Success! Generated: clean.wav,glitch.wav,corrected.wav"
