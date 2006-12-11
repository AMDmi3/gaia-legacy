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

#ifndef __GAIA__SMARTPTR_H__
#define __GAIA__SMARTPTR_H__

namespace gaia {

/**
 * Smart pointer with reference counting.
 *
 * Based on Marshall Cline's C++ FAQ Lite:
 * http://www.parashift.com/c++-faq-lite/
 */
template <class T>
class SmartPtr {
public:
	SmartPtr(): m_Ptr(0) {
	}

	SmartPtr(T* p): m_Ptr(p) {
		if (m_Ptr)
			m_Ptr->IncRef();
	}

	SmartPtr(const SmartPtr<T> &p): m_Ptr(p.m_Ptr) {
		if (m_Ptr) 
			m_Ptr->IncRef();
	}

	virtual ~SmartPtr() {
		if (m_Ptr && m_Ptr->DecRef() == 0)
			delete m_Ptr;
	}

	SmartPtr<T>& operator= (const SmartPtr<T> &p) {
		/*
		 * DO NOT CHANGE THE ORDER OF THESE STATEMENTS!
		 * (This order properly handles self-assignment and other issues)
		 */

		T* const old = m_Ptr;
		m_Ptr = p.m_Ptr;
		if (m_Ptr)
			m_Ptr->IncRef();

		if (old && old->DecRef() == 0)
			delete old;

		return *this;
	}

	bool operator== (const T *p) {
		return m_Ptr == p;
	}

	bool operator== (SmartPtr<T> &p) {
		return m_Ptr == p.m_Ptr;
	}

	bool operator!= (const T *p) {
		return !(m_Ptr == p);
	}

	bool operator!= (SmartPtr<T> &p) {
		return !(m_Ptr == p.m_Ptr);
	}

	T* operator-> () {
		return m_Ptr;
	}

	T& operator* () {
		return *m_Ptr;
	}

	/* bypass, use with caution */
	T* GetPtr() {
		return m_Ptr;
	}

private:
	T	*m_Ptr;
};

} /* namespace gaia */

#endif
