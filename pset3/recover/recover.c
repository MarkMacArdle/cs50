#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

long int get_file_size(FILE *passed_file_pointer);

int main(int argc, char *argv[])
{
    //ensure correct usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");

    //check if input file opened okay
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // now that pointer at end it's position in the stream is the file size in bytes
    long int size = get_file_size(inptr);

    // set stream to start of file before starting loop
    fseek(inptr, 0, SEEK_SET);

    //variables to be used in loop
    bool writing_file = false;
    long int FAT_block_size = 512;
    int FAT_block[FAT_block_size];
    FILE *outptr;

    for (long int i = 0, found_count = 0; i < size; i += FAT_block_size)
    {
        //move stream i bytes from the start
        fseek(inptr, i, SEEK_SET);
        int first_3_bytes[3];
        fread(first_3_bytes, sizeof(char), 3, inptr);

        //check if first 3 and then 4th bytes match jpg standard of starting bits of a file
        if (*first_3_bytes == 0xffd8ff)
        {
            int fourth_byte = 0;
            fread(&fourth_byte, sizeof(char), 1, inptr);

            if (fourth_byte == 0xe0 ||
                fourth_byte == 0xe1 ||
                fourth_byte == 0xe2 ||
                fourth_byte == 0xe3 ||
                fourth_byte == 0xe4 ||
                fourth_byte == 0xe5 ||
                fourth_byte == 0xe6 ||
                fourth_byte == 0xe7 ||
                fourth_byte == 0xe8 ||
                fourth_byte == 0xe9 ||
                fourth_byte == 0xea ||
                fourth_byte == 0xeb ||
                fourth_byte == 0xec ||
                fourth_byte == 0xed ||
                fourth_byte == 0xee ||
                fourth_byte == 0xef)
            {

                char pic_file_name[8]; //there will be 7 characters in the file name + end string character ie xxx.jpg\0
                sprintf(pic_file_name, "%03li.jpg", found_count);

                //increment found_count after naming so first file is called 000.jpg
                found_count++;

                //if a file was already being written then stop it
                if (writing_file)
                {
                    fclose(outptr);
                }

                //create new file
                writing_file = true;
                outptr = fopen(pic_file_name, "w");
            }
        }

        //if in a file write then write this FAT block
        if (writing_file)
        {
            //reset stream to start of this block. It will have moved by checking the first few bytes
            fseek(inptr, i, SEEK_SET);

            fread(FAT_block, FAT_block_size, 1, inptr);
            fwrite(FAT_block, FAT_block_size, 1, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
}


long int get_file_size(FILE *passed_file_pointer)
{

    fseek(passed_file_pointer, 0, SEEK_SET); // seek to beginning of file
    fseek(passed_file_pointer, 0, SEEK_END); // seek to end of file

    // now that the pointer is at its end position in the stream it's also the file size in bytes
    long int size = ftell(passed_file_pointer);

    fseek(passed_file_pointer, 0, SEEK_SET); // seek back to beginning of file

    //printf("in func: %li\n", size);

    return size;
}
