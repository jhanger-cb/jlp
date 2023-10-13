CC		= c++
CFLAGS	= -O3 -g -Wall
TARGET	= jlp
OUT_DIR	= ./bin/
MKDIR_P	= mkdir -p

all: directories $(TARGET)
directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

$(TARGET): $(TARGET).cpp
			$(CC) $(CFLAGS) -o bin/$(TARGET) $(TARGET).cpp -l boost_program_options

clean:
	$(RM) -r ${OUT_DIR}
