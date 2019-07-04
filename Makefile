CC := clang
CXX := clang-cl
CFLAGS := /O2 /Wall /c -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-unknown-pragmas -Wno-newline-eof -Wno-unused-macros -Wno-reserved-id-macro -Wno-deprecated-declarations -Wno-c99-extensions
CPPFLAGS := /O2 /Wall /Wextra /c /GX -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-unknown-pragmas -Wno-newline-eof -Wno-unused-macros -Wno-reserved-id-macro -Wno-deprecated-declarations -Wno-c99-extensions -Wno-zero-as-null-pointer-constant -Wno-old-style-cast
LPLP:= glfw3.lib opengl32.lib gdi32.lib msvcrtd.lib libcmt.lib Kernel32.lib user32.lib shell32.lib
LDFLAGS := -fuse-ld=lld -flto glfw3.lib opengl32.lib gdi32.lib msvcrtd.lib user32.lib shell32.lib

LD := clang-cl

RCFLAGS := -flto -O3 -Wall -c -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-unknown-pragmas -Wno-newline-eof -Wno-unused-macros -Wno-reserved-id-macro -Wno-deprecated-declarations -Wno-c99-extensions

SRCDIR := src

SRCS := $(shell forfiles -P "$(SRCDIR)" -M "*.c" -C "CMD /C ECHO @relpath" -S)
SRCS := $(patsubst ".\\%",$(SRCDIR)\\%,$(SRCS))

OUT := $(SRCS:.c=.o)
OUT := $(OUT:.cpp=.o)
OUT := $(subst src\,bin\,$(OUT))

TREE  := $(sort $(patsubst %\,%,$(dir $(OUT))))
TREE := $(filter-out ./,$(TREE))
TREE := $(TREE:\=)

OUT := $(subst \,/,$(OUT))


OUTFILE := Automatic-Fortnight-Test.exe

.PHONY: run all

run: $(OUTFILE)
	$(OUTFILE)

all: $(OUTFILE)

$(OUTFILE) : $(OUT)
	$(LD) $(OUT) $(LDFLAGS) -o $(OUTFILE)

$(TREE): %:
		-mkdir $@

fix:
	call vcvarsall.bat amd64

bin/%.o : src/%.c $(TREE)
	$(CC) -Xclang -std=gnu17 $(RCFLAGS) $< -o $@
bin/%.o : src/%.cpp $(TREE)
	$(CC) -Xclang -std=gnu++17 $(CFLAGS) $< -o $@