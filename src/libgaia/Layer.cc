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

#include "Layer.h"

namespace gaia {

/********************************************************************\
 * LayerMeta                                                        *
\********************************************************************/
LayerMeta *LayerMeta::first = 0;

LayerMeta::LayerMeta(char *nname, Layer *(*nspawn)(), int ngroup, int ninitiallyactive) {
	name = nname;
	spawn = nspawn;
	group = ngroup;
	initiallyactive = ninitiallyactive;

	/* insert newly registered layer right after last
	 * layer with smaller group number, i.e.
	 * for new layer with group = 1, it will be placed:
	 * 0 0 0 [here] 1 1 1 2 2 2
	 * 0 0 1 0 [here] 1 1 2 2 2
	 * 1 1 1 0 0 0 [here] 2 2 2
	 */
	LayerMeta *current, *after = 0;
	for (current = first; current; current = current->next)
		if (current->group < ngroup)
			after = current;

	if (after == 0) {
		next = first;
		first = this;
	} else {
		next = after->next;
		after->next = this;
	}
}

/********************************************************************\
 * Layer
\********************************************************************/
Layer::Layer() {
}

Layer::~Layer() {
}

} /* namespace gaia */
