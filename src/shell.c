#include "shell.h"
#include "mailbox.h"
#include "cpio.h"
#include "reboot.h"
#include "utils.h"
#include <stddef.h>

typedef struct {
	const char *command;
	const char *description;
	void (*handler)(void);
} Command;

static void help();
static void hello();
static void info();
static void ls();
static void cat();
static void user_program();
static void reboot_device();

static const Command command_list[] = {
	{"help", "Print this help menu", help},
	{"hello", "Print Hello World!", hello},
	{"info", "Show mailbox hardware info", info},
	{"ls", "List files in CPIO archive", ls},
	{"cat", "Print file content", cat},
	{"user_program", "Execute user program to test exception handler", user_program},
	{"reboot", "Reboot the device", reboot_device}
};
static const int command_count = sizeof(command_list) / sizeof(Command);

void help()
{
	for (int i = 0; i < command_count; i++) {
		uart_send_f("  %s : %s\n", command_list[i].command, command_list[i].description);
	}
}

void hello()
{
	uart_send_f("Hello World!\n");
}

void info()
{
	unsigned int board_revision;
	unsigned int base_address;
	unsigned int size;
	unsigned int rate;

	if (get_board_revision(&board_revision) != 0) {
		uart_send_f("Error: Unable to retrieve board revision.\n");
		return;
	}

	if (get_arm_memory(&base_address, &size) != 0) {
		uart_send_f("Error: Unable to retrieve ARM memory information.\n");
		return;
	}

	if (get_clock_rate(&rate, UART_CLOCK_ID) != 0) {
		uart_send_f("Error: Unable to retrieve UART clock rate.\n");
		return;
	}

	uart_send_f("Hardware Information:\n");
	uart_send_f("  Board Revision: 0x%X\n", board_revision);
	uart_send_f("  ARM Memory Base: 0x%X\n", base_address);
	uart_send_f("  ARM Memory Size: 0x%X\n", size);
	uart_send_f("  UART Clock Rate: %d Hz\n", rate);
}

void ls() {
	cpio_ls();
}

void cat() {
	char buffer[FILENAME_BUFFER_SIZE];
	char* file = 0;
	unsigned int size = 0;

	uart_send_f("Filename: ");
	uart_getline(buffer, FILENAME_BUFFER_SIZE);
	buffer[strlen(buffer) - 1] = '\0';

	file = cpio_get_file(buffer, &size);
	if (file == 0) {
		uart_send_f("File not found\n");
		return;
	}

	for (unsigned int i = 0; i < size; i++) {
		uart_send_f("%c", file[i]);
	}
	uart_send_f("\n");
}

void user_program() {
	unsigned char user_bin[] = {
		0x00, 0x00, 0x80, 0xd2, 0x00, 0x04, 0x00, 0x91, 0x01, 0x00, 0x00, 0xd4,
		0x1f, 0x14, 0x00, 0xf1, 0xab, 0xff, 0xff, 0x54, 0x00, 0x00, 0x00, 0x14
	};
	unsigned int user_bin_len = 24;

	void* load_addr = (void*)0x20000;
	memcpy(load_addr, user_bin, user_bin_len);

	asm volatile("msr spsr_el1, %0" :: "r"(0x3c0));
    asm volatile("msr elr_el1, %0" :: "r"(load_addr));
    asm volatile("msr sp_el0, %0" :: "r"(0x10000));
    asm volatile("eret");
}

void reboot_device()
{
	uart_send_f("Rebooting...\n");
	reset(1000);
}

void shell()
{
	char buffer[SHELL_BUFFER_SIZE];
	while (1) {
		uart_send_f("$ ");
		uart_getline(buffer, SHELL_BUFFER_SIZE);

		if (buffer[0] == '\0') {
			return;
		}

		buffer[strlen(buffer) - 1] = '\0';
		int command_found = 0;
		for (int i = 0; i < command_count; i++) {
			if (strcmp(buffer, command_list[i].command) == 0) {
				command_list[i].handler();
				command_found = 1;
				break;
			}
		}

		if (!command_found) uart_send_f("Command '%s' not found. Type 'help' for available commands.\n", buffer);
	}
}
