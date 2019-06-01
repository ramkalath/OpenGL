# (TODO: enter project name here)
PROJECT_NAME = assimp_0

all:
	cd ../build && make
	@echo "\033[92m ------------- Compilation Successful ------------------- \033[0m" 

run:
	@../bin/$(PROJECT_NAME)
	@echo "\033[92m ------------- Execution Successful ------------------- \033[0m"

debug:
	cd ../build && \
	cmake -G "CodeBlocks - Unix Makefiles" .. && \
	codeblocks ${PROJECT_NAME}.cbp &! > /dev/null

clean:
	rm -rf ../build/*
	rm ../bin/*
