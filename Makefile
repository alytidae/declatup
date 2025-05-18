CC = clang
CFLAGS = -std=gnu17 -Wall -Wextra

SRC = main.c args.c tomlc17.c package_xbps.c
SRC_PATH = $(addprefix src/,$(SRC))
OUT = declatup

all:
	$(CC) $(CFLAGS) $(SRC_PATH) -o $(OUT)

clean:
	rm -f $(OUT)
