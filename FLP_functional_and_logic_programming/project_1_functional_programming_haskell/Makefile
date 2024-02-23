OUT := flp21-fun
ARGS := -i

PACK := flp-fun-xrosin00


.PHONY: build
build: $(OUT)

$(OUT): ./src/*.hs
	ghc --make -Wall -o $@ $^
