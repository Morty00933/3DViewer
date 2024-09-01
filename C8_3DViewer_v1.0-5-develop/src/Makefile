COMPILE=gcc -Wall -Werror -Wextra -std=c11 -g

SOURCES=$(SRCPATH)/*.c
OBJECTS=*.o
LIBRARY=backend.a

UNITEST=tests.c
TESTSRC=$(UNITEST) $(LIBRARY)
GCOVSRC=$(UNITEST) $(SOURCES)
EXETEST=test

GCOVFLG=-fprofile-arcs -ftest-coverage $(CHCKFLG)
GCOVINF=$(EXETEST).info
GCOVREP=report

SRCPATH=back
GUIPATH=front
ENDPATH=build
ARCHSRC=$(SRCPATH) $(GUIPATH) $(UNITEST) Makefile *.sh *.txt *.tex objects docker

PRONAME=3d_viewer_v1
EXECUTE=$(PRONAME).app
ARCNAME=$(PRONAME)#_v1.0
ARCHDST=$(ARCNAME)/src
ARCHIVE=$(ARCNAME).tgz

SRCCLNG=$(GUIPATH)/mainwindow.cpp $(GUIPATH)/mainwindow.h $(GUIPATH)/main.cpp $(GUIPATH)/glwidget.cpp $(GUIPATH)/glwidget.h
VGFLAGS=--tool=memcheck --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all

OS=$(shell uname -s)
CHCKLBS=-lcheck #-fsanitize=address
LINUX=-lm -lpthread -lrt -lsubunit
ifeq ($(OS), Linux)
	CHCKFLG=$(CHCKLBS) $(LINUX)
else
	CHCKFLG=$(CHCKLBS)
endif

all: clean install run

install: uninstall
	mkdir $(ENDPATH)
	cd $(GUIPATH) && qmake && make && make clean && rm Makefile
ifeq ($(OS), Linux)
	mv $(GUIPATH)/$(PRONAME) $(ENDPATH)
else
	mv $(GUIPATH)/$(EXECUTE) $(ENDPATH)
endif

uninstall:
ifeq ($(OS), Linux)
	rm -rf $(ENDPATH) $(GUIPATH)/$(PRONAME)
else
	rm -rf $(ENDPATH) $(GUIPATH)/$(EXECUTE)
endif

run:
ifeq ($(OS), Linux)
	./$(ENDPATH)/$(PRONAME)
else
	open $(ENDPATH)/$(EXECUTE)
endif

clean: uninstall undist
	rm -rf $(EXETEST) *.o *.a *.out *.dSYM *.gcda *.gcno *.gcov *.info $(GCOVREP) $(GUIPATH)/.qmake.stash $(GUIPATH)/*.o

dvi:
	open Read.txt
	texi2dvi ReadMe.tex
	texi2pdf ReadMe.tex

dist: undist
	mkdir -p $(ARCHDST)
	cp -r $(ARCHSRC) $(ARCHDST)
	tar -cvzf $(ARCHIVE) $(ARCNAME)/
	rm -rf $(ARCNAME)

undist:
	rm -rf $(ARCNAME) $(ARCHIVE)

$(LIBRARY):
	$(COMPILE) -c $(SOURCES)
	ar rcs $(LIBRARY) $(OBJECTS)
	rm -rf $(OBJECTS)

test: clean $(LIBRARY)
	$(COMPILE) $(TESTSRC) $(CHCKFLG) -o $(EXETEST)
	./$(EXETEST)

gcov_report: clean $(LIBRARY)
	$(COMPILE) $(GCOVSRC) $(GCOVFLG) -o $(EXETEST)
	./$(EXETEST)
	lcov -t "$(EXETEST)" -o $(GCOVINF) --no-external -c -d  .
	genhtml $(GCOVINF) -o $(GCOVREP)
	open $(GCOVREP)/index.html

clang:
	clang-format -style=Google -n $(SRCPATH)/*.* $(SRCCLNG)

memtest: test
ifeq ($(OS), Linux)
	valgrind -q $(VGFLAGS) ./$(EXETEST)
else
	CK_FORK=no leaks --atExit -- ./$(EXETEST)
endif
