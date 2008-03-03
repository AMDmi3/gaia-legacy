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

#include "wwfetch_internal.h"

/**
 * Return size of last downloaded file
 */
size_t wwfetch_get_data_size(wwfetch *handle) {
	return handle->currentdata ? handle->currentsize : 0;
}

/**
 * Return pointed to buffer containing last downloaded file
 */
void *wwfetch_get_data_ptr(wwfetch *handle) {
	return handle->currentdata;
}
