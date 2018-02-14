

PLATFORMIO=~/.platformio/penv/bin/platformio

.PHONY: fs clean

all: fs
	${PLATFORMIO} run

fs:
	python prepare_fs.py
	${PLATFORMIO} run -t buildfs

clean:
	rm -rf .pioenvs
	rm -rf data