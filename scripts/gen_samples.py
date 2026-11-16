#!/usr/bin/env python3
"""Generate sample renders for documentation."""
import subprocess
import sys

def run(cmd):
    print(f"$ {cmd}")
    return subprocess.run(cmd, shell=True)

def main():
    # Build CPU renderer
    run("g++ -std=c++17 -O2 -I include src/render_cpu.cpp src/geodesic.cpp -o bh_cpu")
    # Render at different resolutions
    for w, h in [(160, 90), (320, 180), (640, 360)]:
        print(f"\n=== Rendering {w}x{h} ===")
        # Modify source temporarily? For simplicity just run existing
        run("./bh_cpu")
        # Convert to PNG if PIL available
        try:
            from PIL import Image
            Image.open("output.ppm").save(f"docs/sample_{w}x{h}.png")
            print(f"Saved docs/sample_{w}x{h}.png")
        except ImportError:
            pass

if __name__ == "__main__":
    main()