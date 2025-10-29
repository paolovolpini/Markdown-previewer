CPP = g++
CPPFLAGS = -Wall

TARGET = md-preview

SRC_DIR = src

SRC = $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

$(TARGET) : $(SRC)
	$(CPP) $(CPPFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)
