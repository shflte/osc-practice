#include "shell.h"
#include "uart.h"
#include "mailbox.h"
#include "reboot.h"
#include "utils.h"
#include "config.h"
#include <stddef.h>


typedef struct {
	const char *command;
	const char *description;
	void (*handler)(void);
} Command;

static void help();
static void hello();
static void info();
static void reboot_device();

static const Command command_list[] = {
	{"help", "Print this help menu", help},
	{"hello", "Print Hello World!", hello},
	{"info", "Show mailbox hardware info", info},
	{"reboot", "Reboot the device", reboot_device}
};

void read_command(char *buffer)
{
	size_t index = 0;
	char input;

	while (1) {
		input = uart_recv();
		input = input == '\r' ? '\n' : input;
		uart_send_f("%c", input);
		if (input == '\n') {
			break;
		}
		if (index < SHELL_BUFFER_SIZE - 1) {
			buffer[index++] = input;
		}
	}
	buffer[index] = '\0';
}

void help()
{
	uart_send_f("Available commands:\r\n");
	for (size_t i = 0; i < COMMAND_COUNT; i++) {
		uart_send_f("  %s : %s\r\n", command_list[i].command, command_list[i].description);
	}
}

void hello()
{
	uart_send_f("Hello World!\r\n");
}

void info()
{
	unsigned int board_revision;
	unsigned int base_address;
	unsigned int size;
	unsigned int rate;

	if (get_board_revision(&board_revision) != 0) {
		uart_send_f("Error: Unable to retrieve board revision.\r\n");
		return;
	}

	if (get_arm_memory(&base_address, &size) != 0) {
		uart_send_f("Error: Unable to retrieve ARM memory information.\r\n");
		return;
	}

	if (get_clock_rate(&rate, UART_CLOCK_ID) != 0) {
		uart_send_f("Error: Unable to retrieve UART clock rate.\r\n");
		return;
	}

	uart_send_f("Hardware Information:\r\n");
	uart_send_f("  Board Revision: 0x%X\r\n", board_revision);
	uart_send_f("  ARM Memory Base: 0x%X\r\n", base_address);
	uart_send_f("  ARM Memory Size: 0x%X\r\n", size);
	uart_send_f("  UART Clock Rate: %d Hz\r\n", rate);
}

void reboot_device()
{
	uart_send_f("Rebooting...\r\n");
	reset(1000);
}

void parse_command(char *buffer)
{
	trim_newline(buffer);
	uart_send_f("\r");

	if (buffer[0] == '\0') {
		return;
	}

	for (size_t i = 0; i < COMMAND_COUNT; i++) {
		if (strcmp(buffer, command_list[i].command) == 0) {
			command_list[i].handler();
			return;
		}
	}

	uart_send_f("Command '%s' not found. Type 'help' for available commands.\r\n", buffer);
}

void shell()
{
	char buffer[SHELL_BUFFER_SIZE];
	while (1) {
		uart_send_f("$ ");
		read_command(buffer);
		parse_command(buffer);
	}
}
