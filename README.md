**Archiver LZ77**

LZ77, proposed by Abraham Lempel and Jacob Ziv in 1977, is a lossless data compression algorithm. It operates on the principle of eliminating redundancies by replacing repeated occurrences of data with references to a single copy of that data existing earlier in the uncompressed data stream.

In practice, LZ77 provides a good balance between compression ratio and computational complexity, making it suitable for scenarios where efficient storage and transmission of data are required.

**Usage**

To use the program, compile it with a C/C++ compiler, and run it from the command line with appropriate arguments. The program supports both file and directory operations, and it provides detailed error messages and program execution times.

"<encode/decode> <input_file_or_folder> <output_file_or_folder> <folder/file>"

**How It Works**

**Sliding Window:** LZ77 uses a "sliding window" technique.

**Encoding Process:**
As the algorithm processes the input data, it searches for the longest match between the patterns in the lookahead buffer and the search buffer.
When a match is found, it is encoded as a tuple: (distance, length, next symbol), where:
distance is the number of characters back in the stream to the start of the match.
length is the length of the match.
next symbol is the character immediately following the match in the lookahead buffer.

**No Match Handling:**
If no match is found, the algorithm simply encodes the next character from the lookahead buffer as a literal (i.e., the character itself).
This literal is also added to the search buffer.

**Buffer Management:**
After encoding a match or a literal, the window slides forward by one or more characters, moving the unmatched character from the lookahead buffer to the search buffer, and new characters are added to the lookahead buffer from the input.

**End of Data:**
The process continues until all input data has been processed. If there are characters left in the lookahead buffer at the end without any matches, they are encoded as literals.

**Example** kabababababz

![LZW LZ77 LZ78 Algorithm](https://github.com/yura-poj/LZ77/assets/85958170/be1f4805-03a0-4689-b018-664f3d41d17c)


**Efficiency and Usage**

**Compression Efficiency:** The efficiency of LZ77 is highly dependent on the size of the sliding window and the characteristics of the input data. Larger windows generally provide better compression but require more memory and processing power.
**Applications:** LZ77's principles are used in more complex compression algorithms like DEFLATE (used in ZIP and GZIP), where it is often combined with Huffman coding for further size reduction.

**Advantages**

**Flexibility:** It can effectively compress any type of data (text, binaries, etc.).
**Simplicity:** Despite its powerful performance, the algorithm is relatively simple to implement.

**Disadvantages**

**Memory Usage:** The need for a large search buffer can lead to high memory usage.
**Compression Speed:** Particularly with large windows or large inputs, the process can be slow as it involves searching for matches in a growing buffer.
