#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_fft_real.h>
#include "wave.h"

#define SAMPLES 131072

void wave_plot(FILE *gp, sample_t sample);
void spectrum_plot(FILE *gp, sample_t sample);

int main(int argc, char *argv[]) {
  /* argument */
  if (argc > 2) {
    printf("argument error: too many argument\n");
    printf("  %s [filename]\n", argv[0]);
    return -1;
  } else if (argc < 2) {
    printf("argument error: too few argument\n");
    printf("  %s [filename]\n", argv[0]);
    return -1;
  }

  int i;
  FILE *fp, *gp;
  wave_t wave;

  if ((fp = fopen(argv[1], "r")) == NULL) {
    printf("cannot open %s\n", argv[1]);
    return -1;
  }

  load_wave_header(fp, &wave);
  print_wave_header(wave);
  wave.data = (uint16_t *)malloc(sizeof(uint16_t) * wave.samplings);
  load_wave_data(fp, &wave);
  fclose(fp);

  printf("number of samplings: %d\n", wave.samplings);

  /* split channel data */
  int cnt_l, cnt_r;
  wave.wave_l = (uint16_t *)malloc(sizeof(uint16_t) * wave.samplings / 2);
  wave.wave_r = (uint16_t *)malloc(sizeof(uint16_t) * wave.samplings / 2);

  cnt_l = 0;
  cnt_r = 0;
  for (i=0; i<(int)wave.samplings; i++) {
    if (i % 2 == 0) {
      wave.wave_l[cnt_l] = wave.data[i];
      cnt_l++;
    } else {
      wave.wave_r[cnt_r] = wave.data[i];
      cnt_r++;
    }
  }

  /* N point sampling */
  int N = 2048;
  int start = 65536;
  int temp_l, temp_r;
  sample_t sample;
  sample.wave_l = (double*)malloc(sizeof(double)*N);
  sample.wave_r = (double*)malloc(sizeof(double)*N);

  sample.samplings = N;
  sample.sampling_period = wave.sampling_period;
  sample.sampling_rate = wave.sampling_rate;
  for (i=0; i<N; i++) {
    sample.wave_l[i] = (signed short)wave.wave_l[i + start];
    sample.wave_r[i] = (signed short)wave.wave_r[i + start];
  }

  /* plot wave */
  if ((gp = popen("gnuplot -persist", "w")) == NULL) {
    printf("error: cannot pipe open\n");
    return -1;
  }
  wave_plot(gp, sample);
  pclose(gp);

  /* N point FFT */
  gsl_fft_real_radix2_transform(sample.wave_l, 1, N);
  gsl_fft_real_radix2_transform(sample.wave_r, 1, N);

  /* plot spectrum */
  if ((gp = popen("gnuplot -persist", "w")) == NULL) {
    printf("error: cannot pipe open\n");
    return -1;
  }
  spectrum_plot(gp, sample);
  pclose(gp);

  /* free */
  free(wave.data);
  free(wave.wave_l);
  free(wave.wave_r);
  free(sample.wave_l);
  free(sample.wave_r);

  return 0;
}

void wave_plot(FILE *gp, sample_t sample) {
  int i;

  fprintf(gp, "set terminal x11\n");
  fprintf(gp, "set xlabel \"Time [sec]\"\n");
  fprintf(gp, "set ylabel \"Amplitude\"\n");
  fprintf(gp, "set multiplot\n");

  /* left wave */
  fprintf(gp, "set lmargin screen 0.1\n");
  fprintf(gp, "set rmargin screen 0.9\n");
  fprintf(gp, "set tmargin screen 0.9\n");
  fprintf(gp, "set bmargin screen 0.6\n");
  fprintf(gp, "plot \"-\" with lines linewidth 1.5 title \"wave left\"\n");
  for (i=0; i<(int)sample.samplings; i++) {
    fprintf(gp, "%lf %.0lf\n", i * sample.sampling_period, sample.wave_l[i]);
  }
  fprintf(gp, "e\n");

  /* right wave */
  fprintf(gp, "set lmargin screen 0.1\n");
  fprintf(gp, "set rmargin screen 0.9\n");
  fprintf(gp, "set tmargin screen 0.4\n");
  fprintf(gp, "set bmargin screen 0.1\n");
  fprintf(gp, "plot \"-\" with lines linewidth 1.5 title \"wave right\"\n");
  for (i=0; i<(int)sample.samplings; i++) {
    fprintf(gp, "%lf %.0lf\n", i * sample.sampling_period, sample.wave_r[i]);
  }
  fprintf(gp, "e\n");

  fprintf(gp, "unset multiplot");
  fflush(gp);
}

void spectrum_plot(FILE *gp, sample_t sample) {
  int i;

  fprintf(gp, "set terminal x11\n");
  fprintf(gp, "set xlabel \"Frequency [Hz]\"\n");
  fprintf(gp, "set ylabel \"Amplitude\"\n");
  fprintf(gp, "set logscale x\n");
  fprintf(gp, "set multiplot\n");

  /* left wave */
  fprintf(gp, "set lmargin screen 0.1\n");
  fprintf(gp, "set rmargin screen 0.9\n");
  fprintf(gp, "set tmargin screen 0.9\n");
  fprintf(gp, "set bmargin screen 0.6\n");
  fprintf(gp, "plot \"-\" with lines linewidth 1.5 title \"spectrum left\"\n");
  for (i=0; i<(int)sample.samplings / 2; i++) {
    fprintf(gp, "%d %.0lf\n", i * sample.sampling_rate / sample.samplings, fabs(sample.wave_l[i]));
  }
  fprintf(gp, "e\n");

  /* right wave */
  fprintf(gp, "set lmargin screen 0.1\n");
  fprintf(gp, "set rmargin screen 0.9\n");
  fprintf(gp, "set tmargin screen 0.4\n");
  fprintf(gp, "set bmargin screen 0.1\n");
  fprintf(gp, "plot \"-\" with lines linewidth 1.5 title \"spectrum right\"\n");
  for (i=0; i<(int)sample.samplings / 2; i++) {
    fprintf(gp, "%d %.0lf\n", i * sample.sampling_rate / sample.samplings, fabs(sample.wave_r[i]));
  }
  fprintf(gp, "e\n");

  fprintf(gp, "unset multiplot");
  fflush(gp);
}
