// OutputDebugString as an ostream:
// cdbg << "Hello " << name << std::endl;
// wcdbg << L"Hello " << wname << std::endl;
// 
// cnull and wcnull are do-nothing stremas that support macro based logging on/off selection:
//
// #ifdef NDEBUG
// #	define CDBG cnull
// #	define WCDBG wcnull
// #else
// #	define CDBG cdbg
// #	define WCDBG wcdbg
// #endif


#ifndef BOOSTTESTUI_DBGSTREAM_H
#define BOOSTTESTUI_DBGSTREAM_H

#pragma once

// #pragma message("\"Imaging/dbgstream.h\" is deprecated, please use <Fei/dbgstream.h> instead.")

#include <streambuf>
#include <string>
#include "windows.h"


namespace gj {

template <class Elem, class Tr = std::char_traits<Elem>, class Alloc = std::allocator<Elem> >
class basic_debugbuf : public std::basic_streambuf<Elem, Tr>
{
protected:
	int sync()
	{
		output(m_buf.c_str());
		m_buf.clear();
		return 0;
	}

	int_type overflow(int_type c)
	{
		if (c == traits_type::eof())
			return 0;

		m_buf += traits_type::to_char_type(c);
		if (c == '\n')
			sync();
		return c;
	}

private:
	std::basic_string<Elem, Tr, Alloc> m_buf;

	static void output(const char* msg)
	{
		OutputDebugStringA(msg);
	}

	static void output(const wchar_t* msg)
	{
		OutputDebugStringW(msg);
	}
};

template <class Elem, class Tr = std::char_traits<Elem> >
class basic_dbgstream : public std::basic_ostream<Elem, Tr>
{
public:
	basic_dbgstream() : std::basic_ostream<Elem, Tr>(&buf)
	{
	}

private:
	basic_debugbuf<Elem, Tr> buf;
};

template <class Elem, class Tr = std::char_traits<Elem>, class Alloc = std::allocator<Elem> >
class basic_nullbuf : public std::basic_streambuf<Elem, Tr>
{
};

template <class Elem, class Tr = std::char_traits<Elem> >
class basic_nullstream : public std::basic_ostream<Elem, Tr>
{
public:
	basic_nullstream() : std::basic_ostream<Elem, Tr>(&buf)
	{
	}

private:
	basic_nullbuf<Elem, Tr> buf;
};

typedef basic_nullstream<char> nullstream;
typedef basic_nullstream<wchar_t> wnullstream;

typedef basic_dbgstream<char> dbgstream;
typedef basic_dbgstream<wchar_t> wdbgstream;

} // namespace gj

__declspec(selectany) gj::nullstream cnull;
__declspec(selectany) gj::wnullstream wcnull;

__declspec(selectany) gj::dbgstream cdbg;
__declspec(selectany) gj::wdbgstream wcdbg;

#endif // BOOSTTESTUI_DBGSTREAM_H