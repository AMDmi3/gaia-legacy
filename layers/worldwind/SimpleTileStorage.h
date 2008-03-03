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

#ifndef __GAIA__SIMPLETILESTORAGE_H__
#define __GAIA__SIMPLETILESTORAGE_H__

#include <pthread.h>
#include <queue>

#include "Exceptions.h"
#include "TileStorage.h"
#include "Messages.h"

namespace gaia {

/**
 * Base class for tile storages with queue and one working thread;
 *
 * @deprecated should be restructured
 */
class SimpleTileStorage: public TileStorage {
public:
	/**
	 * Constructor.
	 */
	SimpleTileStorage();

	/**
	 * Destructor.
	 */
	virtual ~SimpleTileStorage();

	/**
	 * Insert tile into processing queue.
	 */
	void Enqueue(TilePtr tile);

	/**
	 * Set next storage for tile loading.
	 *
	 * If we cannot load given tile, it will be passed to
	 * NextLoadStorage.
	 */
	void SetNextLoadStorage(TileStorage *ts);

	/**
	 * Set storage for tile saving.
	 *
	 * After successfull loading, tile will be passed to
	 * SaveStorage to be saved.
	 */
	void SetSaveStorage(TileStorage *ts);

	/**
	 * Stop passing tiles to other storages.
	 *
	 * Need to be called before destruction of storage.
	 */
	void Detach();

protected:
	/**
	 * Do actual processing - defined in derived class.
	 */
	virtual void Process(TilePtr tile) = 0;

private:
	/**
	 * Entry point for worker thread.
	 */
	static void* ThreadEntryPoint(void* pthis);

	/**
	 * Thread function.
	 *
	 * This function loops endlessly taking tiles from queue one
	 * by one and processig them with Process()
	 */
	void ThreadRun();

/* variables */
private:
	std::queue<TilePtr>	m_Queue;	///< Queue of tiles waiting to be saved/loaded with this storage

	pthread_mutex_t m_QueueMutex;		///< Mutex to protect queue
	pthread_cond_t	m_QueueEmptyCond;	///< Condition to wake up thread after sleep
	pthread_t	m_Thread;		///< Thread in which all actual loading/saving goes asynchronously

protected:
	TileStorage	*m_pNextLoadStorage;	///< Storage to pass tile to for loading, if we couldn't load it
	TileStorage	*m_pSaveStorage;	///< Storage to pass tile to for saving
};

} /* namespace gaia */

#endif
