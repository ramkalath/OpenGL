PROJECT_NAME = 35_shadows

all:
	@cd ../build && \
	cmake .. && \
	make && \
	cd ../src
	@echo "\033[92m ------------- Compilation Successful ------------------- \033[0m"

run:
	@../bin/$(PROJECT_NAME)

debug:
	cd ../build && \
	cmake -G "CodeBlocks - Unix Makefiles" .. && \
	codeblocks ${PROJECT_NAME}.cbp &! > /dev/null

clean:
	rm -rf ../build/*
	rm ../bin/*