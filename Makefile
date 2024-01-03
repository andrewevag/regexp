.PHONY = default clean help
CXX = g++
INCLUDE = -I$(PWD)/$(INC)
LIBINCLUDE= -L$(PWD)/
CXXFLAGS = -g -Wall $(INCLUDE)
BINS = main
SRC = src
INC = inc
OBJ = obj
SOURCE = $(wildcard $(SRC)/*.cpp)
SOURCE += $(patsubst $(SRC)/%.l, $(SRC)/%.cpp, $(wildcard $(SRC)/*.l))
SOURCE += $(patsubst $(SRC)/%.y, $(SRC)/%.cpp, $(wildcard $(SRC)/*.y))
OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCE))
LIBS = -lregexp
INDUCED = $(patsubst $(SRC)/%.l, $(SRC)/%.cpp, $(wildcard $(SRC)/*.l))
INDUCED += $(patsubst $(SRC)/%.y, $(SRC)/%.cpp, $(wildcard $(SRC)/*.y))
INDUCED += $(patsubst $(SRC)/%.y, $(INC)/%.hpp, $(wildcard $(SRC)/*.y))
INDUCED += $(patsubst $(SRC)/%.y, $(SRC)/%.output, $(wildcard $(SRC)/*.y))
LIBRARY = libregexp.a

default: $(OBJ) $(BINS)  

$(OBJ):
	mkdir $(OBJ)
$(LIBRARY): $(OBJECTS)
	ar -rcs $@ $(OBJECTS)

main: main.o $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBINCLUDE) -o $@ $< $(LIBS)

main.o: main.cpp  $(INDUCED)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $< 

$(SRC)/regexplexer.cpp: $(SRC)/regexplexer.l
	flex -s -o $@ $< 

$(INC)/regexpparser.hpp $(SRC)/regexpparser.cpp: $(SRC)/regexpparser.y
	bison -dv -o $(SRC)/regexpparser.cpp $<
	mv $(SRC)/regexpparser.hpp $(INC)/regexpparser.hpp

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $^ 

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $^ 


clean: 
	$(RM) -f $(OBJ)/* $(BINS) *.o tests/bin/* $(INDUCED)
	$(RM) -df $(OBJ)
	$(RM) -f $(TESTBINS)
	$(RM) -df $(TESTBINFOLDER)
	$(RM) -f $(LIBRARY)
help: 
	@echo "\e[1;34mmake:\e[0m\n\t builds $(BINS) by compiling all the $(PWD)/$(SRC) and $(PWD) folder and linking it to bins"
	@echo "\e[1;34mmake clean:\e[0m\n\t cleans $(OBJ) folder and bins ($(BINS)) and object files in the cwd"
	@echo "\e[1;34mmake help:\e[0m\n\t displays help message"
	@echo "\e[1;34mmake create_test TESTNAME=<testname>:\e[0m\n\t creates a new test suite with the given testname"
	@echo "\e[1;34mmake test:\e[0m\n\t compiles all *.cpp files in tests folder and runs them"

debug:
	@echo "\nVARIABLES:"
	@echo "SOURCE = $(SOURCE)"
	@echo "OBJECTS = $(OBJECTS)"
	@echo "INCLUDE = $(INCLUDE)"
	@echo "LIBS = $(LIBS)"
	@echo "INDUCED = $(INDUCED)"


TEST = tests/
TESTS = $(wildcard $(TEST)*.cpp)
TESTBINS = $(patsubst $(TEST)%.cpp, $(TEST)bin/%, $(TESTS))
TESTLIBS = -lcheck -lm -lpthread -lrt -lsubunit
TESTBINFOLDER = $(TEST)bin

$(TEST)bin/%: $(TEST)%.cpp $(OBJECTS)
	$(CXX) $(INCLUDE) $^ $(LIB) -o $@  $(TESTLIBS)
## -- make test ->	compiles everything in src files and links them to 
## -- 				all tests and then runs them
## -- 
## -- To create new test use the script : check_new_suite <suite_name>
test: $(OBJ) $(TESTBINFOLDER) $(BINS) $(TESTBINS) $(OBJECTS)
	@for test in $(TESTBINS) ; do ./$$test ; done

$(TESTBINFOLDER):
	mkdir $(TESTBINFOLDER)
## -- make create_test TESTNAME="<testname>" -> creates a new check test suite
## --											and puts it in the test folder

TESTNAME = default
TESTFORMAT = \n\#include <check.h>\n\#include <cstdlib>\n\#include <cstdio>\n\nSTART_TEST($(TESTNAME)_initial_test){\n\tck_assert_int_eq(0, 0);\n} END_TEST\n\n\nSuite *$(TESTNAME)_suite(void){\n\tSuite* s;\n\tTCase *tc_core;\n\ts = suite_create(\"$(TESTNAME)\");\n\ttc_core = tcase_create(\"Core\");\n\n\ttcase_add_test(tc_core, $(TESTNAME)_initial_test);\n\n\tsuite_add_tcase(s, tc_core);\n\treturn s;\n}\nint main(void){\n\tint no_failed = 0;\n\tSuite* s;\n\tSRunner* runner;\n\ts = $(TESTNAME)_suite();\n\trunner = srunner_create(s);\n\n\tsrunner_run_all(runner, CK_NORMAL);\n\tno_failed = srunner_ntests_failed(runner);\n\tsrunner_free(runner);\n\tif(no_failed == 0){\n\t\tprintf(\"✔️  Success!\\\n\");\n\t}else{\n\t\tprintf(\"❌ %d tests failed!\\\n\", no_failed);\n\t}\t\n\treturn (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;\n}\n\n
create_test: 
	@echo "$(TESTFORMAT)" > "tests/$(TESTNAME)test.cpp"
