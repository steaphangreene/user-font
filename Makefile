CC=     gcc -O3 -s `../user/U2-CFlags`
USER=	$(shell ../user/U2-Dir)
OBJS=   $(USER)/*.o
DEPS=   $(USER)/*.cpp
DEPH=   $(USER)/*.h
ALL=    $(DEPS) $(DEPH) $(OBJS) Makefile
LIBS=	`../user/U2-Libs`

mkf:	mkf.o $(ALL)
	make -C $(USER)
	$(CC) -o mkf mkf.o $(LIBS)

mkf.o:	mkf.cpp $(DEPH)
	$(CC) -c mkf.cpp
