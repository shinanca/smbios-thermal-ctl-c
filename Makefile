TARGET = smbios-thermal-ctl-c
CC = gcc
CFLAGS  = -g -Wall -O3
LDFLAGS = -lsmbios_c
PREFIX = /usr
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) ${LDFLAGS} -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	$(RM) $(BINDIR)/$(TARGET)
