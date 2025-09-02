CC = gcc
CXX = g++
CFLAGS= -Wall -Werror -Wextra -std=c11
CXXFLAGS= -Wall -Werror -Wextra -std=c++20
GCOV_FLAGS= --coverage -fprofile-arcs -ftest-coverage
LCOV_FLAGS= --no-external --ignore-errors mismatch 
CHECK_LIB = -lgtest -lgmock -pthread
QT_PATH = -DCMAKE_PREFIX_PATH:PATH=/home/zakharov/Qt/6.9.1/gcc_64


.PHONY: all install uninstall clean dvi test_snake test_tetris
 
all: install

install: install_snake_cli install_tetris_cli install_desktop
	rm -f *.o

uninstall: clean
	rm -f *.out
	rm -rf build

clean:
	rm -f *.gcda *.gcno dvi/*.out dvi/*.log dvi/*.aux dvi/*.pdf *.o *.a unit_test.out *.info *.report
	rm -rf report

dvi:
	xelatex -output-directory=dvi dvi/documentation.tex

test_snake: $(wildcard tests/*.cc ) snake_back.o
	$(CXX) $(CXXFLAGS) $^ -o unit_test_snake.out $(CHECK_LIB)
	./unit_test_snake.out

test_tetris: $(wildcard tests/*.c ) tetris_back.o fsm_tetris.o
	$(CC) $(CFLAGS) $^ -o unit_test_tetris.out $(CHECK_LIB)
	./unit_test_tetris.out


.PHONY: install_snake_cli install_tetris_cli install_desktop

install_snake_cli: front_console.o snake_back.o controller_snake.o
	$(CXX) $(CXXFLAGS) $^ -o snake_cli.out -lncurses

install_tetris_cli: front_console.o tetris_back.o fsm_tetris.o controller_tetris.o
	$(CC) $(CFLAGS) $^ -o tetris_cli.out -lncurses

install_desktop: build
	cmake --build build

build: 
	cmake -S ./gui/desktop -B ./build $(QT_PATH) --fresh


front_console.o: gui/cli/front_console.c
	$(CC) $(CFLAGS) -c $^ -o $@

controller_snake.o: gui/cli/controller.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@ -DBUILD_SNAKE

controller_tetris.o: gui/cli/controller.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@ -DBUILD_TETRIS

snake_back.o: brick_game/snake/snake_back.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

tetris_back.o: brick_game/tetris/back_tetris.c
	$(CC) $(CFLAGS) -c $^ -o $@

fsm_tetris.o: brick_game/tetris/fsm_tetris.c
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: gcov_report formation-clang check-clang cppcheck valgrind fsanitaze

gcov_report: clean 
	$(CXX) -o unit_test.out $(GCOV_FLAGS) $(CXXFLAGS) tests/*.cc brick_game/snake/snake_back.cc $(CHECK_LIB)
	./unit_test.out
	lcov -t "unit_test" -o unit_test.info -c -d . $(LCOV_FLAGS)
	genhtml -o report unit_test.info
	open ./report/index.html

formation-clang:
	cp ../materials/linters/.clang-format .
	clang-format --version
	clang-format -i */*/*.cc */*.cc */*/*.c 

check-clang:
	cp ../materials/linters/.clang-format .
	clang-format --version
	clang-format -n */*/*.cc */*.cc */*/*.c 

cppcheck:
	cppcheck --version
	cppcheck --enable=all --suppress=missingIncludeSystem --checkers-report=cppcheck.report */*.cc */*/*.cc */*/*.c

valgrind: test
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./unit_test.out

fsanitaze: clean
	g++ -o unit_test.out $(CFLAGS) tests/*.cc -fsanitize=address -fsanitize=undefined $(CHECK_LIB)
	./unit_test.out

