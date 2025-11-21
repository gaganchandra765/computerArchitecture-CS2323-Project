from PIL import Image
import sys
import os


TARGET_SIZE = (256, 256) 

if len(sys.argv) != 3:
    print("Usage: python log_to_image.py <input_log> <output_image>")
    sys.exit(1)

input_log = sys.argv[1]
output_img_path = sys.argv[2]

pixels = []

try:
    print(f"Reading {input_log}...")
    
    if not os.path.exists(input_log):
        print(f"Error: Input file '{input_log}' does not exist.")
        sys.exit(1)

    with open(input_log, 'r') as f:
        for line in f:
            try:
                line = line.strip()
                if not line: continue
                
                val = int(line)
                
                
                if val < 0:
                    val += 0x100000000
                
            
                a = (val >> 24) & 0xFF
                r = (val >> 16) & 0xFF
                g = (val >> 8) & 0xFF
                b = val & 0xFF
                
                pixels.append((r, g, b, a))
            except ValueError:
                continue

 
    expected_len = TARGET_SIZE[0] * TARGET_SIZE[1]
    
    if len(pixels) < expected_len:
        print(f"Warning: Log has fewer pixels ({len(pixels)}) than expected ({expected_len}). Padding with black.")
        pixels.extend([(0,0,0,255)] * (expected_len - len(pixels)))
    elif len(pixels) > expected_len:
        print(f"Warning: Log has more pixels ({len(pixels)}) than expected. Truncating.")
        pixels = pixels[:expected_len]

    img = Image.new("RGBA", TARGET_SIZE)
    img.putdata(pixels)

    
    if output_img_path.lower().endswith(('.jpg', '.jpeg')):
        print("Format is JPEG. Converting to RGB (Transparency will be lost)...")
        
        background = Image.new("RGB", TARGET_SIZE, (255, 255, 255))
        background.paste(img, mask=img.split()[3]) 
        img = background

    img.save(output_img_path)
    print(f"Success! Saved image to {output_img_path}")

except Exception as e:
    print(f"Error: {e}")