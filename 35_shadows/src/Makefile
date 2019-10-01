PROJECT_NAME = 35_shadows

all:
	echo "#define USE_GLEW" > use_glew.h
	@cd ../build && cmake .. && make
	rm use_glew.h && touch use_glew.h
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
