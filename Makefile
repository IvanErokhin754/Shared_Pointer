CXX = clang++

CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -O2

SANITIZE_FLAGS = -fsanitize=address -g

ANALYZE_FLAGS = --analyze \
	-Xanalyzer -analyzer-checker=cplusplus.NewDeleteLeaks \
	-Xanalyzer -analyzer-output=text


SRC = src/main.cpp
APP = main


TEST_SRC = tests/test_SharedPtr_array.cpp \
		   tests/test_SharedPtr_single.cpp

TEST_BIN = tests_run

GTEST_FLAGS = $(shell pkg-config --cflags --libs gtest_main)


all: $(APP)


$(APP): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(APP)


$(TEST_BIN): $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(GTEST_FLAGS) -o $(TEST_BIN)


# ---------- TESTS ----------

test: $(TEST_BIN)
	./$(TEST_BIN)


# ---------- ADDRESS SANITIZER ----------

asan: CXXFLAGS += $(SANITIZE_FLAGS)
asan: clean $(APP)
	./$(APP)


asan-test: CXXFLAGS += $(SANITIZE_FLAGS)
asan-test: clean $(TEST_BIN)
	./$(TEST_BIN)


# ---------- STATIC ANALYZER ----------

analyze:
	$(CXX) -std=c++20 -Wall -Wextra -Iinclude $(ANALYZE_FLAGS) $(SRC)


analyze-test:
	$(CXX) -std=c++20 -Wall -Wextra -Iinclude $(ANALYZE_FLAGS) $(GTEST_FLAGS) $(TEST_SRC)


# ---------- LEAKS ----------

leaks:
	$(CXX) -std=c++20 -Wall -Wextra -Iinclude -O0 -g $(SRC) -o $(APP)
	MallocStackLogging=1 leaks --atExit -- ./$(APP)


leaks-test:
	$(CXX) -std=c++20 -Wall -Wextra -Iinclude -O0 -g \
		$(TEST_SRC) $(GTEST_FLAGS) -o $(TEST_BIN)
	MallocStackLogging=1 leaks --atExit -- ./$(TEST_BIN)


# ---------- CLEAN ----------

clean:
	rm -f $(APP) $(TEST_BIN)


.PHONY: all test asan asan-test analyze analyze-test leaks leaks-test clean