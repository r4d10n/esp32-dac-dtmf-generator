#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

void main()
{
    FILE *fptr, *hptr;
    char fname[30];

    hptr = fopen("dtmf-wav.h","w");

    for(int i=0; i < 16; i++)
    {
        sprintf(fname,"%d-u8.wav",i);
        fptr = fopen(fname,"r+b");

        fseek(fptr, 0, SEEK_END);
        long fsize = ftell(fptr);
        fseek(fptr, 0, SEEK_SET);

        uint8_t *buffer_p = malloc(fsize + 1);
        fread(buffer_p, fsize, 1, fptr);
        fclose(fptr);

        fprintf(hptr, "unsigned char PROGMEM DTMF%d[%ld] = {", i, fsize);

        for(int j=0; j < fsize; j++)
        {
            if (j % 16 == 0) fprintf(hptr,"\n\t");

            //fprintf(hptr,"0x%2.2x, 0x%2.2x,", (uint8_t)buffer_p[j], (uint8_t)(buffer_p[j] >> 8));
            fprintf(hptr,"0x%2.2x,", buffer_p[j]);
        }

        fseek(hptr, -1, SEEK_CUR);

        fprintf(hptr, "};\n\n"); 
    }

    fclose(hptr);

}