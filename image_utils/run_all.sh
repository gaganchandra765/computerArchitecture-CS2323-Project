#!/usr/bin/env bash

set -e  

python ../image_utils/image_to_text.py ../image_utils/image.jpg image_data.txt
python ../image_utils/generate_player_image.py

echo "running player_image_clean.s"
./vm --run player_image_clean.s
mv -f image_out.log clean.log

echo "running player_image_glitch.s"
./vm --run player_image_glitch.s
mv -f image_out.log glitch.log

echo "running player_image_corrected.s"
./vm --run player_image_corrected.s
mv -f image_out.log corrected.log

echo "converting logs to PNG"
python ../image_utils/log_to_image.py clean.log clean.png
python ../image_utils/log_to_image.py glitch.log glitch.png
python ../image_utils/log_to_image.py corrected.log corrected.png

echo "Success! Generated: clean.png, corrected.png glitch.png"
