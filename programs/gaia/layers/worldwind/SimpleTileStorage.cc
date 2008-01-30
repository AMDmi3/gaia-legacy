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

#include "SimpleTileStorage.h"

namespace gaia {

SimpleTileStorage::SimpleTileStorage() {
	if (pthread_mutex_init(&m_QueueMutex, 0) != 0) {
		throw ErrnoException("pthread_mutex_init() failed", errno);
	}

	if (pthread_cond_init(&m_QueueEmptyCond, 0) != 0) {
		pthread_mutex_destroy(&m_QueueMutex);
		throw ErrnoException("pthread_cond_init() failed", errno);
	}

	if (pthread_create(&m_Thread, 0, ThreadEntryPoint, (void*)this) != 0) {
		pthread_mutex_destroy(&m_QueueMutex);
		pthread_cond_destroy(&m_QueueEmptyCond);
		throw ErrnoException("pthread_create() failed", errno);
	}

	m_pNextLoadStorage = 0;
	m_pSaveStorage = 0;
}

SimpleTileStorage::~SimpleTileStorage() {
	int *result;

	/* wait for loader thread to finish */
	pthread_cancel(m_Thread);
	pthread_join(m_Thread, (void**)&result);

	/* cleanup synchronisation objects */
	pthread_mutex_destroy(&m_QueueMutex);
	pthread_cond_destroy(&m_QueueEmptyCond);
}

void* SimpleTileStorage::ThreadEntryPoint(void* pthis) {
	SimpleTileStorage* ts = (SimpleTileStorage*)pthis;

	/* start loader */
	ts->ThreadRun();

	return (void*)1;
}

void SimpleTileStorage::Enqueue(TilePtr tile) {
	/* insert requested item in queue */
	pthread_mutex_lock(&m_QueueMutex);

	/* signal worker thread if queue was empty */
	if (m_Queue.size() == 0)
		pthread_cond_signal(&m_QueueEmptyCond);

	m_Queue.push(tile);

	pthread_mutex_unlock(&m_QueueMutex);
}

void SimpleTileStorage::ThreadRun() {
	/* spin in this loop forever */
	while(1) {
		pthread_mutex_lock(&m_QueueMutex);
		while (m_Queue.size() == 0)
			pthread_cond_wait(&m_QueueEmptyCond, &m_QueueMutex);

		TilePtr current = m_Queue.front();
		m_Queue.pop();

		pthread_mutex_unlock(&m_QueueMutex);

		if (!current->IsOld()) {
			try {
				try {
					/* do actual processing - defined in derived class */
					Process(current);
				} catch (std::exception &e) {
					/* storage error worth to be reported to user, like:
					 * - unable to download from web due to `cannot connect' or `authentication failed'
					 * - unable to save
					 */
					warning("SimpleTileStorage: error (%s) (%d %d %d)\n", e.what(), current->GetX(), current->GetY(), current->GetLevel());

					/* XXX: this may need to be added to avoid loops
					 * (but there will be no loops in basic storage layout with 1 local and 1 remote storages)
					if (current->IsSaveable)
						throw;
					*/
				}

				/* pass it down the chain */
				if (current->IsLoaded() && current->IsSaveable() && m_pSaveStorage)
					m_pSaveStorage->Enqueue(current);
				else if (!current->IsLoaded() && m_pNextLoadStorage)
					m_pNextLoadStorage->Enqueue(current);
			} catch (std::exception &e) {
				/* fatal error in enqueue?! */
				warning("SimpleTileStorage: fatal error (%s) (%d %d %d)\n", e.what(), current->GetX(), current->GetY(), current->GetLevel());
			}
		}
	}
}

void SimpleTileStorage::SetNextLoadStorage(TileStorage *ts) {
	m_pNextLoadStorage = ts;
}

void SimpleTileStorage::SetSaveStorage(TileStorage *ts) {
	m_pSaveStorage = ts;
}

void SimpleTileStorage::Detach() {
	m_pNextLoadStorage = 0;
	m_pSaveStorage = 0;
}

} /* namespace gaia */
