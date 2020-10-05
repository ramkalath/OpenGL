PROJECT_NAME = animation
.SILENT: all project

all: 
	# echo "#define USE_GLEW" > use_glew.h
	cd ../build; \
	make
	# rm use_glew.h && touch use_glew.h
	@echo -e "\033[92m ------------- Compilation Successful ------------------- \033[0m"
project:
	if [ ! -d "../build" ]; then mkdir ../build; fi
	if [ ! -d "../bin" ]; then mkdir ../bin; fi
	cd ../build; \
	cmake ..; \
	make
run:
	../bin/$(PROJECT_NAME)
	@echo -e "\033[92m ------------- Execution Successful ------------------- \033[0m"
debug:
	cd ../build && \
	cmake -G "CodeLite - Unix Makefiles" .. && \
	codelite ${PROJECT_NAME}.workspace &! > /dev/null
clean:
	rm -rf ../build/*
	rm ../bin/*
