// Implements
#include "Fyredon/Other/pch.h"

#include <Fyredon/Debug/Logger.h>

/*
	TODO
	=====
	- Utilize debug thread to make logging async
	- Maybe export to client independantly
	- Add more format specifiers
	- Automated runtime/memory error reporting?
*/


// Initialize once
Fyredon::Logger Fyredon::Logger::instance;

Fyredon::Logger::Logger() {

	// Setup virtual terminal mode
	h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (h_stdout == INVALID_HANDLE_VALUE) {
		std::fprintf(stderr, "[FATAL %lu]: Cannot setup logger terminal\n", GetLastError());
	}

	if (!GetConsoleMode(h_stdout, &mode)) {
		std::fprintf(stderr, "[FATAL %lu]: Cannot get console mode\n", GetLastError());
	}

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(h_stdout, mode)) {
		std::fprintf(stderr, "[FATAL %lu]: Cannot set console mode\n", GetLastError());
	}

	SetConsoleTitleA("Fyredon Debugger"); // Console Window title

	GetConsoleScreenBufferInfo(h_stdout, &buf_info);
	size.X = buf_info.srWindow.Right - buf_info.srWindow.Left + 1;
	size.Y = buf_info.srWindow.Bottom - buf_info.srWindow.Top + 1;


	// Draw border
	std::printf(ESC "(0");							// Enter Line drawing mode
	std::printf(CSI "106;102m");					// Border/Line color

	for (int i = 1; i < size.X; i++)				// Draw the line
		std::printf("q");

	std::printf(CSI "0m");							// Reset format
	std::printf(ESC "(B");							// Exit line drawing mode

	// Draw entry message
	const char* msg = "Fyredon Logger Setup Successful";
	std::printf(CSI "2;%dH", (size.X/2)-16);		// Move halfway
	std::printf(ESC "H");							// Enter tabstop
	std::printf(CSI "93;104m");						// Format
	std::printf("%s", msg);							// Print the message
	std::printf(CSI "0m");							// Reset format
	std::printf("\n\n");


}

// Get the singleton instance of the logger class (intended to be thread-safe)
Fyredon::Logger& Fyredon::Logger::getInstance()
{
	return instance;
}


// Helper to print a variadic log message to the console
void Fyredon::Logger::message(const char* fmt, va_list args)
{
	while (*fmt != '\0') {

		if (*fmt == 'd') {
			int i = va_arg(args, int);
			std::printf(" %d ", i);
		}
		else if (*fmt == 'c') {
			int c = va_arg(args, int);
			std::printf(" %c ", c);

		}
		else if (*fmt == 'f') {
			double d = va_arg(args, double);
			std::printf(" %f ", d);
		}
		else if (*fmt == 's') {
			const char* str = va_arg(args, const char*);
			std::printf(" %s ", str);
		}
		++fmt;
	}

}


// Log error message
void Fyredon::Logger::error(const char* fmt, ...)
{
	if (chunk)
		return;
	// Set error format
	std::printf(CSI "1m");		// Bolden
	std::printf(CSI "91m");		// Bright red
	std::printf("[ERR] ");
	time_t timestamp = time(nullptr);
	struct tm time = *localtime(&timestamp);
	std::printf("[%d-%02d-%02d %02d:%02d:%02d] ", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);


	// Forward invocation to message
	va_list args;
	va_start(args, fmt);
	message(fmt, args);
	va_end(args);
	std::printf("\n");

	// Restore console format
	std::printf(CSI "22m");		// Unbolden
	std::printf(CSI "39m");		// Default color

}

// Log info message
void Fyredon::Logger::info(const char* fmt, ...)
{
	if (chunk)
		return;
	// Set info format
	std::printf(CSI "1m");		// Bolden
	std::printf(CSI "92m");		// Bright green
	std::printf("[INF] ");
	time_t timestamp = time(nullptr);
	struct tm time = *localtime(&timestamp);
	std::printf("[%d-%02d-%02d %02d:%02d:%02d] ", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);


	// Forward invocation to message
	va_list args;
	va_start(args, fmt);
	message(fmt, args);
	va_end(args);
	std::printf("\n");


	// Restore console format
	std::printf(CSI "22m");		// Unbolden
	std::printf(CSI "39m");		// Default color

}

// Log warning message
void Fyredon::Logger::warn(const char* fmt, ...)
{
	if (chunk)
		return;
	// Set warning format
	std::printf(CSI "1m");		// Bolden
	std::printf(CSI "93m");		// Bright yellow
	std::printf("[WRN] ");
	time_t timestamp = time(nullptr);
	struct tm time = *localtime(&timestamp);
	std::printf("[%d-%02d-%02d %02d:%02d:%02d] ", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);


	// Forward invocation to message
	va_list args;
	va_start(args, fmt);
	message(fmt, args);
	va_end(args);
	std::printf("\n");

	// Restore console format
	std::printf(CSI "22m");		// Unbolden
	std::printf(CSI "39m");		// Default color
}


// Enable chunk message
void Fyredon::Logger::chunk_start(const char* header)
{
	chunk = true;
	chunk_header = header;

	// Print top box shape with header title
	print_horizontal_border(true);
	print_vertical_border();
	print_title(chunk_header);
	print_vertical_border_end();
	print_vertical_border();
	print_vertical_border_end();


}

// Disable chunk message
void Fyredon::Logger::chunk_end()
{
	print_vertical_border();
	print_vertical_border_end();
	print_horizontal_border(false);
	chunk = false;
	chunk_header = "";
	std::printf("\n");

}

// Log a chunk message
void Fyredon::Logger::chunk_msg(const char* RHS, const char* fmt, ...)
{
	if (!chunk)
		return;

	print_vertical_border();

	// RHS format
	std::printf(CSI "0;91m");
	std::printf("%+5s", "");
	std::printf("%-30s", RHS);

	// Message format
	std::printf(CSI "0;96m");

	va_list args;
	va_start(args, fmt);
	message(fmt, args);
	va_end(args);

	print_vertical_border_end();

	// Restore console format
	std::printf(CSI "22m");		// Unbolden
	std::printf(CSI "39m");		// Default color

}



/* ===== TEXT FORMATTING HELPERS ===== */
void Fyredon::Logger::print_horizontal_border(bool top)
{
	std::printf(ESC "(0");						// Line drawing mode
	std::printf(CSI "104;93m");					// Make the border bright yellow on bright blue
	std::printf(top ? "l" : "m");				// print left corner 

	for (int i = 1; i < size.X - 1; i++)
		std::printf("q");						// in line drawing mode, \x71 -> \u2500 "HORIZONTAL SCAN LINE-5"

	std::printf(top ? "k" : "j");				// print right corner
	std::printf(CSI "0m");
	std::printf(ESC "(B");						// Line drawing mode end
	std::printf("\n");

}


void Fyredon::Logger::print_vertical_border()
{

	std::printf(ESC "(0");						// Enter Line drawing mode
	std::printf(CSI "104;93m");					// bright yellow on bright blue
	std::printf("x");							// in line drawing mode, \x78 -> \u2502 "Vertical Bar"
	std::printf(CSI "0m");						// restore color
	std::printf(ESC "(B");						// exit line drawing mode

}

void Fyredon::Logger::print_vertical_border_end()
{
	// Move to end of specified column and print a border there
	std::printf(CSI "%dG", size.X);
	print_vertical_border();
	std::printf("\n");

}

void Fyredon::Logger::print_vertical_border_at(int pos)
{
	std:: printf(CSI "%dG", pos);
	print_vertical_border();

}

void Fyredon::Logger::print_title(const char* title)
{
	int pos = size.X / 2 - (strlen(title) / 2);
	std::printf(CSI "%dG", pos);				// Position title in the middle
	std::printf(CSI "102;35m");
	std::printf(title);
}


