#ifndef WAVE_H
#define WAVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct wave_t {
  char     RIFF[5];
  uint32_t FILESIZE;
  char     WAVE[5];
  char     fmt[5];
  uint32_t fmt_Bytes;
  uint16_t formatID;
  uint16_t channel;
  uint32_t sampling_rate;
  double   sampling_period;
  uint32_t data_velocity; // Byte / sec
  uint16_t block_size; // (byte / sample) * channel
  uint16_t bps; // bit / sample
  uint16_t extension_size;
  char     chunk_id[5];
  uint32_t chunk_Bytes;
  uint32_t samplings;
  uint16_t *data;
  uint16_t *wave_l, *wave_r;
} wave_t;

typedef struct sample_t {
  double   *wave_l, *wave_r;
  uint32_t samplings;
  uint32_t sampling_rate;
  double   sampling_period;
} sample_t;

void print_wave_header(wave_t wave);
int load_wave_header(FILE *fp, wave_t *wave);
int load_wave_extension(FILE *fp, wave_t *wave);
int load_wave_data(FILE *fp, wave_t *wave);

#endif
