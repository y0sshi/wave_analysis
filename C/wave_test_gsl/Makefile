WAVE = wave
BIN = wave_test_gsl
OPT = -lm -lgsl -lgslcblas -W

FILENAME = "../../data/304 Feldschlacht III.wav"
#FILENAME = "../../data/sine_00100/sine_-06_05_00500.wav"

default: all

object: $(WAVE).c
	gcc -c $(WAVE).c -o $(WAVE).o 

all: $(BIN).c $(WAVE).o
	gcc $(BIN).c $(WAVE).o -o $(BIN) $(OPT)
	./$(BIN) $(FILENAME)

clean:
	rm -f $(WAVE).o $(BIN)
