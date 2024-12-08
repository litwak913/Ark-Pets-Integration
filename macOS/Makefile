CFLAGS = -Wall
LDFLAGS = -shared -fPIC -lobjc -framework Foundation -framework AppKit
LIBNAME = libgdglow

.PHONY: clean
all: $(LIBNAME).dylib

%_x86.o: %.m
	$(CC) $(CFLAGS) -c -target x86_64-apple-macos10.12 $^ -o $@

%_arm.o: %.m
	$(CC) $(CFLAGS) -c -target arm64-apple-macos11 $^ -o $@

$(LIBNAME)_x86.dylib: gdglow_x86.o
	$(CC) $(LDFLAGS) -target x86_64-apple-macos10.12 $^ -o $@

$(LIBNAME)_arm.dylib: gdglow_arm.o
	$(CC) $(LDFLAGS) -target arm64-apple-macos11 $^ -o $@

$(LIBNAME).dylib: $(LIBNAME)_x86.dylib $(LIBNAME)_arm.dylib
	lipo -create -output $@ $^

install: $(LIBNAME).dylib
	cp $^ ~/Library/Application\ Support/ArkPets/

clean:
	rm -r *.dylib *.o
