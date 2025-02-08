# Display exported symbols:
#  nm -D pkcs11-logger.so | grep ' T '

SRC_DIR=../../src

CC= gcc
ARCH_FLAGS= -m32
CFLAGS= $(ARCH_FLAGS) -Wall -Wextra -Werror -O2 -I$(SRC_DIR)
LIBNAME=pkcs11-logger-x86.so

all: dl.o init.o lock.o log.o pkcs11-logger.o translate.o utils.o
	$(CC) $(ARCH_FLAGS) -shared -o $(LIBNAME) \
	-Wl,-soname,$(LIBNAME) \
	-Wl,--version-script,pkcs11-logger.version \
	dl.o init.o lock.o log.o pkcs11-logger.o translate.o utils.o \
	-lc -ldl -lpthread
	strip --strip-all $(LIBNAME)

dl.o: $(SRC_DIR)/dl.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/dl.c

init.o: $(SRC_DIR)/init.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/init.c

lock.o: $(SRC_DIR)/lock.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/lock.c

log.o: $(SRC_DIR)/log.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/log.c

pkcs11-logger.o: $(SRC_DIR)/pkcs11-logger.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/pkcs11-logger.c

translate.o: $(SRC_DIR)/translate.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/translate.c

utils.o: $(SRC_DIR)/utils.c $(SRC_DIR)/*.h
	$(CC) $(CFLAGS) -fPIC -c $(SRC_DIR)/utils.c

clean:
	-rm -f *.o

distclean: clean
	-rm -f *.so
