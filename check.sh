#!/bin/bash

# Проверка количества аргументов
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Получение имени файла из аргумента
input_file="$1"
output_file="${input_file}.zip"

# Проверка наличия файла
if [ ! -f "$input_file" ]; then
    echo "File does not exist: $input_file"
    exit 1
fi

# Засечка времени начала в миллисекундах
start_time=$(gdate +%s%3N)

# Сжатие файла
zip -r "$output_file" "$input_file"

# Засечка времени окончания в миллисекундах
end_time=$(gdate +%s%3N)

# Расчёт времени выполнения в миллисекундах
elapsed_time=$((end_time - start_time))

# Получение размеров файлов
original_size=$(stat -f%z "$input_file")
compressed_size=$(stat -f%z "$output_file")

# Расчёт разницы размеров
size_difference=$((original_size - compressed_size))

# Вывод результатов
echo "Time taken: $elapsed_time milliseconds"
echo "Original size: $original_size bytes"
echo "Compressed size: $compressed_size bytes"
echo "Size difference: $size_difference bytes"

# Очистка (опционально, закомментировать, если файл .zip нужен)
# rm "$output_file"
