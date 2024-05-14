#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

input_dir="$1"

if [ ! -d "$input_dir" ]; then
    echo "Directory does not exist: $input_dir"
    exit 1
fi

for input_file in "$input_dir"/*; do
    if [ -d "$input_file" ]; then
        continue
    fi

    output_file="${input_file}.zip"

    start_time=$(gdate +%s%3N)

    zip -r "$output_file" "$input_file" &> /dev/null

    end_time=$(gdate +%s%3N)

    elapsed_time=$((end_time - start_time))

    original_size=$(stat -f%z "$input_file")
    compressed_size=$(stat -f%z "$output_file")

    size_difference=$((original_size - compressed_size))

    echo "File: $input_file"
    echo "Time taken: $elapsed_time milliseconds"
    echo "Original size: $original_size bytes"
    echo "Compressed size: $compressed_size bytes"
    echo "Size difference: $size_difference bytes"
    echo "-------------------------"
    rm "$output_file"
done
