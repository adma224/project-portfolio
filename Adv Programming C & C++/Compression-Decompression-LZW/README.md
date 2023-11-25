Readme for LZW File Compression-Decompression Package

This package constains the implementation for a program that compresses and decompresses files onto binary files. The user may choose any type of file, and compressit, and then decompress any
file that has been previously compressed by the same program.
It won't work with files compressed by other programs.

To use follow these instructions:

  The Makefile creates the "lwzdecoder" executable, but the user may also use the "encode" or "decode" commands to either compress or decompress files

  Use options:

  	-c   Performs file compression
  	-d   Performs file decompression
  	Choosing either of these two options is necessary, and the user can't choose both. By using commands "encode" or "decode" is not needed to use these options.
  	-i <infile>   Sets the input file, default is stdin
  	-o <outfile>   Sets the output file, default is stdout
  	-v   Verbatim option, displays information about the compression.

 Issues:
 	So far the compression algorithm works fine. A few minor adjustments to the io.c file were made that perform the compression in a different way. You may get wrong output for decompressing using other people's LZW decoder programs. I checked the contents of the memory file line by line and it showed correct compression, it is just formatted in a slightly different way. Currently working on a fix.
 	Also, the decompression is not working correctly, which is due to a problem in the main.c file that I have enough time to correct. The problem relies within the main decompression loop.
 	The other files work perfectly. 
 	The lines of code that produce memory leaks in the main.c file are commented out to avoid said memory leaks.
 	Finally, I couldn't adapt the program using little endian and big endian for use in 32bit or 62bit computers
