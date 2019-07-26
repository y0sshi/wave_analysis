#!/usr/local/bin/python3
#coding:utf-8

import sys
import wave
import numpy as np
import matplotlib.pyplot as plt

argv = sys.argv
argc = len(argv)

try:
    filename = argv[1]
except:
    print("argument error...")
    print("please input")
    print("  python3", argv[0], "[FILENAME]")
    sys.exit()


## load wavefile
wp = wave.open(filename, "rb")
channels = wp.getnchannels()
samplewidth = wp.getsampwidth()
framerate = wp.getframerate()
frames = wp.getnframes()

period = 1 / framerate
wavetime = period * frames

print('channels: ', channels)
print('samplewidth: ', samplewidth, '[Bytes]')
print('framerate: ', framerate, '[Hz]')
print('total frames: ', frames)

data_bin = wp.readframes(frames)
wp.close()

if samplewidth == 2:
    data = np.frombuffer(data_bin, dtype='int16')
elif samplewidth == 4:
    data = np.frombuffer(data_bin, dtype='int32')

if channels == 2:
    data_l = data[::channels]
    data_r = data[1::channels]
elif channels == 1:
    data_l = data
    data_r = [0] * frames

## Fourier translation
N = 2400
start = 65536
end = start + N

sample_l = data_l[start:end:1]
sample_r = data_r[start:end:1]

print("executing FFT ...")
spectrum_l = np.abs(np.fft.fft(sample_l))
spectrum_r = np.abs(np.fft.fft(sample_r))
print("done!")

# plot wave

# channel 1
plt.subplot(2, 2, 1)
plt.xlabel("time [sec]")
plt.ylabel("Amplitude")
plt.plot(sample_l, label="left wave")
plt.legend()

# channel 2
plt.subplot(2, 2, 2)
plt.xlabel("time [sec]")
plt.ylabel("Amplitude")
plt.plot(sample_r, label="right wave")
plt.legend()

# plot spectrum

# channel 1
plt.subplot(2, 2, 3)
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude")
plt.plot(spectrum_l[0:1023], label="left spectrum")
plt.xscale('log')
plt.legend()

# channel 2
plt.subplot(2, 2, 4)
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude")
plt.plot(spectrum_r[0:1023], label="right spectrum")
plt.xscale('log')
plt.legend()

# display plot
plt.tight_layout()
plt.show()
