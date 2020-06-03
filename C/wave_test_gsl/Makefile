WAVE = wave
BIN = wave_test_gsl
OPT = -lm -lgsl -lgslcblas -W

#FILENAME = "../../data/sine_00100/sine_-06_05_00500.wav"
FILENAME = "../../data/newtype.wav"

default: run

run: $(BIN)
	./$(BIN) $(FILENAME)

$(WAVE).o: $(WAVE).c
	gcc -c $(WAVE).c -o $(WAVE).o 

$(BIN): $(BIN).c $(WAVE).o
	gcc $(BIN).c $(WAVE).o -o $(BIN) $(OPT)

clean:
	rm -f $(WAVE).o $(BIN)
