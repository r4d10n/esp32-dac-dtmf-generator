// Need to fix Unsigned 8bit generation
//
// Use for now: for file in *.wav; do sox $file -b 8 -e unsigned-integer $(basename "$file" .wav)-u8.wav ; done
// use: wav2h for converting all wav's to single h file

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

int dtmf[16][2] = {
  {941, 1336}, // 0
  {697, 1209}, // 1
  {697, 1336}, // 2
  {697, 1477}, // 3
  {770, 1209}, // 4
  {770, 1336}, // 5
  {770, 1477}, // 6
  {852, 1209}, // 7
  {852, 1336}, // 8
  {852, 1477}, // 9
  {697, 1633}, // A
  {770, 1633}, // B
  {852, 1633}, // C
  {941, 1633}, // D
  {941, 1209}, // *
  {941, 1477}  // #
};

char dtmf_key[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', '*', '#'};

/*
The header of a wav file Based on:
https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
*/
typedef struct wavfile_header_s {
    char    ChunkID[4];     /*  4   */
    int32_t ChunkSize;      /*  4   */
    char    Format[4];      /*  4   */

    char    Subchunk1ID[4]; /*  4   */
    int32_t Subchunk1Size;  /*  4   */
    int16_t AudioFormat;    /*  2   */
    int16_t NumChannels;    /*  2   */
    int32_t SampleRate;     /*  4   */
    int32_t ByteRate;       /*  4   */
    int16_t BlockAlign;     /*  2   */
    int16_t BitsPerSample;  /*  2   */

    char    Subchunk2ID[4];
    int32_t Subchunk2Size;
} wavfile_header_t;

typedef struct PCM16_stereo_s {
    int16_t left;
    int16_t right;
} PCM16_stereo_t;

#define SUBCHUNK1SIZE   (16)
#define AUDIO_FORMAT    (1) /*For PCM*/

#define NUM_CHANNELS    (1)

#define SAMPLE_RATE     (16000)

#define BITS_PER_SAMPLE (16)

#define BYTE_RATE       (SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8)
#define BLOCK_ALIGN     (NUM_CHANNELS * BITS_PER_SAMPLE / 8)

#define TONE_DURATION   (0.5) // 0.25s Duration

#define FRAME_COUNT     (int)(TONE_DURATION * SAMPLE_RATE)

#define HEADER_SIZE     (sizeof(wavfile_header_t))
#define FILE_SIZE       (HEADER_SIZE + FRAME_COUNT)

wavfile_header_t generate_wave_header() {
    wavfile_header_t wav_header;
    int32_t subchunk2_size;
    int32_t chunk_size;

    size_t write_count;

    subchunk2_size  = FRAME_COUNT * NUM_CHANNELS * BITS_PER_SAMPLE / 8;
    chunk_size      = 4 + (8 + SUBCHUNK1SIZE) + (8 + subchunk2_size);

    wav_header.ChunkID[0] = 'R';
    wav_header.ChunkID[1] = 'I';
    wav_header.ChunkID[2] = 'F';
    wav_header.ChunkID[3] = 'F';

    wav_header.ChunkSize = chunk_size;

    wav_header.Format[0] = 'W';
    wav_header.Format[1] = 'A';
    wav_header.Format[2] = 'V';
    wav_header.Format[3] = 'E';

    wav_header.Subchunk1ID[0] = 'f';
    wav_header.Subchunk1ID[1] = 'm';
    wav_header.Subchunk1ID[2] = 't';
    wav_header.Subchunk1ID[3] = ' ';

    wav_header.Subchunk1Size = SUBCHUNK1SIZE;
    wav_header.AudioFormat = AUDIO_FORMAT;
    wav_header.NumChannels = NUM_CHANNELS;
    wav_header.SampleRate = SAMPLE_RATE;
    wav_header.ByteRate = BYTE_RATE;
    wav_header.BlockAlign = BLOCK_ALIGN;
    wav_header.BitsPerSample = BITS_PER_SAMPLE;

    wav_header.Subchunk2ID[0] = 'd';
    wav_header.Subchunk2ID[1] = 'a';
    wav_header.Subchunk2ID[2] = 't';
    wav_header.Subchunk2ID[3] = 'a';
    wav_header.Subchunk2Size = subchunk2_size;

    return wav_header;
}

void generate_DTMF_stereo(int dtmf_index, PCM16_stereo_t  *buffer_p) {
    double amplitude = 0.5 * SHRT_MAX;

    float w1 = 2.0 * 3.141592 * (float)dtmf[dtmf_index][0] / (float)SAMPLE_RATE;
    float w2 = 2.0 * 3.141592 * (float)dtmf[dtmf_index][1] / (float)SAMPLE_RATE;

    for(int32_t k = 0;  k < FILE_SIZE; k++)  {
        if (k < HEADER_SIZE / sizeof(PCM16_stereo_t)) {
            buffer_p[k].left = 0;
            buffer_p[k].right = 0;
        }
        else {
            buffer_p[k].left    = (int16_t)((amplitude * sin((float)k * w1)) + (amplitude * sin((float)k * w2)));
            buffer_p[k].right   = buffer_p[k].left;
        }
    }
}

void generate_DTMF_mono(int dtmf_index, int16_t  *buffer_p) {
    double amplitude = 0.5 * SHRT_MAX;

    float w1 = 2.0 * 3.141592 * (float)dtmf[dtmf_index][0] / (float)SAMPLE_RATE;
    float w2 = 2.0 * 3.141592 * (float)dtmf[dtmf_index][1] / (float)SAMPLE_RATE;

    for(int32_t k = 0;  k < FILE_SIZE; k++)  {
        if (k < HEADER_SIZE / sizeof(int16_t))
            buffer_p[k] = 0;
        else if (k < FILE_SIZE / 2)
            buffer_p[k] = (int16_t)((amplitude * sin((float)k * w1)) + (amplitude * sin((float)k * w2)));
            //buffer_p[k] = (uint8_t)(amplitude * (sin((float)k * w1)) + sin((float)k * w2));
        else
            buffer_p[k] = 0;
    }


}

int main(int argc, char *argv[]) {
    int ret;
    FILE *fptr, *hptr;
    char fname[30];

    #if NUM_CHANNELS == 1
    int16_t buffer_p[FILE_SIZE];
    #else
    PCM16_stereo_t  buffer_p[FILE_SIZE];
    #endif

    wavfile_header_t wave_header = generate_wave_header();

    hptr = fopen("dtmf-wav.h","w");

    for(int i=0; i < 16; i++) {
        #if NUM_CHANNELS == 1
        generate_DTMF_mono(i, buffer_p);
        #else
        generate_DTMF_stereo(i, buffer_p);
        #endif

        memcpy((void *)&buffer_p, (void *)&wave_header, sizeof(wave_header));

        sprintf(fname, "%d.wav", i);
        fptr = fopen(fname, "wb");

        #if NUM_CHANNELS == 1
        fwrite((void *)&buffer_p, sizeof(int16_t), FILE_SIZE, fptr);
        #else
        fwrite((void *)&buffer_p, sizeof(PCM16_stereo_t), FILE_SIZE, fptr);
        #endif

        fclose(fptr);

        fprintf(hptr, "unsigned char PROGMEM DTMF%d[%ld] = {", i, (FILE_SIZE)*2);

        for(int j=0; j < FILE_SIZE; j++)
        {
            if (j % 4 == 0) fprintf(hptr,"\n");

            #if NUM_CHANNELS == 1
            fprintf(hptr,"0x%2.2x, 0x%2.2x,", (uint8_t)buffer_p[j], (uint8_t)(buffer_p[j] >> 8));
            //fprintf(hptr,"0x%2.2x,", buffer_p[j]);
            #else
            fprintf(hptr,"0x%2.2x, 0x%2.2x, ", (uint8_t)buffer_p[j].left, (uint8_t)(buffer_p[j].left >> 8), (uint8_t)buffer_p[j].right, (uint8_t)(buffer_p[j].right >> 8));
            #endif
        }

        fseek(hptr, -1, SEEK_CUR);

        fprintf(hptr, "};\n\n");

    }

    fclose(hptr);
}
