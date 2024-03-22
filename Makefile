.PHONY: all DR DC DX

all: DR DC DX move_DR_DX

DR:
	$(MAKE) -C DR

DC:
	$(MAKE) -C DC

DX:
	$(MAKE) -C DX

move_DR_DX:
	mv DX/bin/* DR/bin/
