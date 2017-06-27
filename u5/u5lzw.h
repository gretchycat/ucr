// =============================================================
// This program decompresses Ultima_6-style LZW-comrpessed files
// =============================================================

#include <stdio.h>

unsigned char read1(FILE *f);
long read4(FILE *f);
long get_filesize(FILE *input_file);
bool is_valid_lzw_file(FILE *input_file);
long get_uncompressed_size(FILE *input_file);
int get_next_codeword (long& bits_read, unsigned char *source, int codeword_size);
void output_root(unsigned char root, unsigned char *destination, long& position);
void get_string(int codeword);
void lzw_decompress(unsigned char *source, long source_length, unsigned char *destination, long destination_length);
void lzw_decompress(FILE *input_file, FILE* output_file);
//void one_argument(char *file_name);
//void two_arguments(char *source_file_name, char *destination_file_name);

