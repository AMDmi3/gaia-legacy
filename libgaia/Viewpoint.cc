/* 
 * Copyright (C) 2006-2008 Dmitry Marakasov
 *
 * This file is part of Gaia.
 *
 * Gaia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gaia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gaia.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Viewpoint.h"

namespace gaia {

Viewpoint::Viewpoint() {
	m_Translation = 0.0;
	m_TranslationTime = DEFAULT_TRANSLATION_TIME;
}

Viewpoint::Viewpoint(Eye &eye) {
	m_CurrentEye = eye;
	m_SourceEye = eye;
	m_TargetEye = eye;

	m_Translation = 1.0;
	m_TranslationTime = DEFAULT_TRANSLATION_TIME;
}

Viewpoint::~Viewpoint() {
}

void Viewpoint::SetCurrentCoordinates(Eye &eye) {
	m_CurrentEye = eye;
	m_SourceEye = eye;
	m_TargetEye = eye;

	m_Translation = 1.0;
}

void Viewpoint::SetTargetCoordinates(Eye &eye) {
	m_SourceEye = m_CurrentEye;
	m_TargetEye = eye;

	m_Translation = 0.0;
}

void Viewpoint::Animate(double delta) {
	if (m_Translation >= 1.0)
		return;

	m_Translation += delta/m_TranslationTime;

	if (m_Translation >= 1.0) {
		m_Translation = 1.0;
		m_CurrentEye = m_TargetEye;
		return;
	}

	double trans = TranslationFunction(m_Translation);
	m_CurrentEye = m_SourceEye * (1.0 - trans) + m_TargetEye * trans;
}

double Viewpoint::TranslationFunction(double percent) {
	/*
	 * 1-(1-x)^n is most suitable here
	 */
	return percent * (2.0 - percent);
}

Eye Viewpoint::GetEye() {
	return m_CurrentEye;
}

} /* namespace gaia */
