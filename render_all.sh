##
## EPITECH PROJECT, 2026
## Raytracer
## File description:
## render_all
##

#!/bin/bash


output_dir="./screenshots"

find ./scenes -maxdepth 1 -type f -name '*.cfg' -print0 |
while IFS= read -r -d '' file; do
    echo "Rendering $file..."
    ./raytracer "$file" > "$output_dir/$(basename "$file" .cfg).ppm"
done