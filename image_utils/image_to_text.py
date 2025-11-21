from PIL import Image
import sys


TARGET_SIZE = (256, 256)

if len(sys.argv) != 3:
    print("Usage: python image_to_txt.py <input_image> <output_txt>")
    sys.exit(1)

input_path = sys.argv[1]
output_path = sys.argv[2]

try:
    img = Image.open(input_path)
    img = img.resize(TARGET_SIZE)
    img = img.convert("RGBA") 

    pixels = list(img.getdata())
    
    print(f"Converting image to {len(pixels)} 32-bit integers...")

    with open(output_path, 'w') as f:
        for r, g, b, a in pixels:
            packed = (a << 24) | (r << 16) | (g << 8) | b
            if packed > 0x7FFFFFFF:
                packed -= 0x100000000
            
            f.write(f"{packed}\n")

    print(f"Successfully wrote {output_path}")

except Exception as e:
    print(f"Error: {e}")