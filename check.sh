#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

input_file="$1"
output_file="${input_file}.zip"

if [ ! -f "$input_file" ]; then
    echo "File does not exist: $input_file"
    exit 1
fi

start_time=$(gdate +%s%3N)

zip -r "$output_file" "$input_file"

end_time=$(gdate +%s%3N)
elapsed_time=$((end_time - start_time))

original_size=$(stat -f%z "$input_file")
compressed_size=$(stat -f%z "$output_file")

size_difference=$((original_size - compressed_size))

echo "Time taken: $elapsed_time milliseconds"
echo "Original size: $original_size bytes"
echo "Compressed size: $compressed_size bytes"
echo "Size difference: $size_difference bytes"

rm "$output_file"
