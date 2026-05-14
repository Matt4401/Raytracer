##
## EPITECH PROJECT, 2026
## Raytracer
## File description:
## render_all
##

#!/usr/bin/env bash
set -euo pipefail

output_dir="./screenshots"
mkdir -p "$output_dir"

# Render each top-level scene (.cfg) in ./scenes
find ./scenes -maxdepth 1 -type f -name '*.cfg' -print0 |
while IFS= read -r -d '' file; do
    base=$(basename "$file")
    out="$output_dir/scene_${base}.ppm"
    echo "Rendering $file -> $out"
    ./raytracer "$file" > "$out"
done

echo "All renders done. Outputs in $output_dir"