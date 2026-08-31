CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -lm

SRC = src/main.c src/ols.c
BIN = elasticidade

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)

run: $(BIN)
	./$(BIN) data/avocado_total_us.csv

clean:
	rm -f $(BIN) data/resultados.csv

.PHONY: all run clean