#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wave.h"

void print_wave_header(wave_t wave) {
  printf("RIFF header      : %s\n", wave.RIFF);
  printf("file syze        : %d [Bytes]\n", wave.FILESIZE);
  printf("WAVE header      : %s\n", wave.WAVE);
  printf("fmt header       : %s\n", wave.fmt);
  printf("fmt Bytes        : %d [Bytes]\n", wave.fmt_Bytes);
  printf("formatID         : %04x\n", wave.formatID);
  printf("channel          : %d\n", wave.channel);
  printf("sampling rate    : %d [kHz]\n", wave.channel);
  printf("data velocity    : %d\n", wave.data_velocity);
  printf("block size       : %d [Bytes]\n", wave.block_size);
  printf("bit per sampling : %d [bit/samplings]\n", wave.bps);
  printf("extension size   : %d [Bytes]\n", wave.extension_size);
  printf("data             : %s\n", wave.chunk_id);
  printf("data bytes       : %d [Bytes]\n", wave.chunk_Bytes);
  printf("samplings        : %d [samples]\n", wave.samplings);
}

int load_wave_header(FILE *fp, wave_t *wave) {
  /* initialization */
  wave->RIFF[4] = '\0';
  wave->WAVE[4] = '\0';
  wave->fmt[4] = '\0';
  wave->chunk_id[4] = '\0';

  /* RIFF chunk */
  fread(wave->RIFF, 1, 4, fp);
  fread(&wave->FILESIZE, 4, 1, fp);

  /* WAVE chunk */
  fread(wave->WAVE, 1, 4, fp);

  /* format chunk */
  fread(wave->fmt, 1, 4, fp);
  fread(&wave->fmt_Bytes, 4, 1, fp);
  fread(&wave->formatID, 2, 1, fp);
  fread(&wave->channel, 2, 1, fp);
  fread(&wave->sampling_rate, 4, 1, fp);
  fread(&wave->data_velocity, 4, 1, fp);
  fread(&wave->block_size, 2, 1, fp);
  fread(&wave->bps, 2, 1, fp);
  
  /* any chunk */
  char finish = 0;
  char *chunk;
  do {
    fread(wave->chunk_id, 1, 4, fp);
    fread(&wave->chunk_Bytes, 4, 1, fp);
    printf("%s\n", wave->chunk_id);
    if (!strcmp(wave->chunk_id, "data")) {
      wave->samplings = wave->chunk_Bytes / (wave->bps / 8);
      finish = 1;
    } else {
      chunk = (char*)malloc(sizeof(char)*(wave->chunk_Bytes));
      fread(chunk, 1, wave->chunk_Bytes, fp);
      free(chunk);
    }
  } while(!finish);

  wave->sampling_period = (double) 1 / wave->sampling_rate;

  return 0;
}

int load_wave_data(FILE *fp, wave_t *wave) {

  /* wave data */
  fread(wave->data, (wave->bps / 8), wave->samplings * 2, fp);

  return 0;
}
