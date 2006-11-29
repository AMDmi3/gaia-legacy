/*
 * gaia - opensource 3D interface to the planet
 * Copyright (C) 2006 gaia project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __GAIA__EXCEPTIONS_H__
#define __GAIA__EXCEPTIONS_H__

#include <exception>
#include <errno.h>

#define EXCEPTION_MESSAGE_BUFFER 1024

/*
 * Some clever books say that exception should be as lightweight
 * as possible, and I try stick to this rule for now.
 */

/**
 * Generic exception class.
 * Contains error message and optional error code.
 */ 
class Exception: public std::exception {
public:
	Exception(const char *str) throw() : m_Message(str) { }
	virtual ~Exception() throw() { }

	virtual const char* what() const throw() { return m_Message; }

private:
	const char	*m_Message;
	int		m_Errno;
};

/**
 * Exception with error code.
 * Provides additional error code, useful for syscall/curl/bdb errors.
 */ 
class ErrnoException: public std::exception {
public:
	ErrnoException(const char *str, int err) throw() : m_Message(str), m_Errno(err) { }
	virtual ~ErrnoException() throw() { }

	virtual const char* what() const throw() { return m_Message; };
	virtual int err() const throw() { return m_Errno; };

private:
	const char	*m_Message;
	int		m_Errno;
};

/*
 * Prototype exception class; maybe useful
 *
class SysException: public std::exception {
public:
	SysException(const char *str, int errno) : m_Messsage(str), m_Errno(errno), m_TempBuf(0) throw() { }
	virtual ~Exception() throw() { delete[] m_TempBuf; }

	virtual const char* what() const throw() {
		if (!m_TempBuf) {
			m_TempBuf = new char [EXCEPTION_MESSAGE_BUFFER];
			snprintf(m_TempBuf, EXCEPTION_MESSAGE_BUFFER, "%s: %s", 
		}

		return m_Message;
	};

private:
	const char	*m_Message;
	int		m_Errno;

	char		*m_TempBuf;
};*/

#endif
