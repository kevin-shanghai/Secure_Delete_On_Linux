OPT=-O2 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
all: xad_file xad_hd xad_sector xad_swap xad_clear xad_clear
	echo "Congratulations, completed."
xad_file:xadlib.c xadfile.c
	gcc -g xadlib.c xadfile.c -o xad_file
	@echo xad_file completed.
xad_hd:xadlib.c xadhd.c
	gcc -g xadlib.c xadhd.c -o xad_hd
	@echo xad_hd completed.
xad_sector:xadsector.c
	gcc -g xadlib.c xadsector.c -o xad_sector
	@echo xad_sector completed.
xad_swap:xadswap.c
	gcc -g xadlib.c xadswap.c -o xad_swap
	@echo xad_swap completed.
xad_clear:xadclear.c
	gcc -g $(OPT) xadclear.c -o xad_clear
	@echo xad_clear completed.
clean:
	rm -rf xad_file xad_hd xad_sector xad_swap xad_clear
