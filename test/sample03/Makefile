LDFLAGS := -z,now,-z,norelro -zexecstack
CFLAGS := -fno-stack-protector -no-pie

all:
	gcc -Wl,$(LDFLAGS) main.c -o x64.out $(CFLAGS)
	gcc -Wl,$(LDFLAGS) main.c -o x86.out $(CFLAGS) -m32
