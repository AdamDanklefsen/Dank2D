#pragma once
#include <string>

class DankException {
public:
	DankException(const wchar_t* file, unsigned int line, const std::wstring& note = L"")
		:
		note(note),
		file(file),
		line(line) {
	}
	const std::wstring& getNote() const { return note; }
	const std::wstring& getFile() const { return file; }
	const unsigned int getLine() const { return line; }
	const std::wstring getLoc() const {
		return std::wstring(L"Line [") + std::to_wstring(line) + std::wstring(L"] in file ") + file;
	}
	virtual std::wstring GetFullMessage() const = 0;
	virtual std::wstring GetExceptionType() const = 0;
private:
	std::wstring note, file;
	unsigned int line;
};