/*--------------------------------------------------------------------------------
    ooo      L   attice-based  |
  o\.|./o    e   xtensible     | LeMonADE: An Open Source Implementation of the
 o\.\|/./o   Mon te-Carlo      |           Bond-Fluctuation-Model for Polymers
oo---0---oo  A   lgorithm and  |
 o/./|\.\o   D   evelopment    | Copyright (C) 2013-2015 by 
  o/.|.\o    E   nvironment    | LeMonADE Principal Developers (see AUTHORS)
    ooo                        | 
----------------------------------------------------------------------------------

This file is part of LeMonADE.

LeMonADE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LeMonADE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LeMonADE.  If not, see <http://www.gnu.org/licenses/>.

--------------------------------------------------------------------------------*/

#ifndef LEMONADE_UPDATER_UPDATERSYNCHRONIZE_H
#define LEMONADE_UPDATER_UPDATERSYNCHRONIZE_H

#include <LeMonADE/updater/AbstractUpdater.h>

/**
 * @file
 *
 * @class UpdaterSynchronize
 *
 * @brief This Updater forces the synchronization of the Ingredients and the simulation.
 *
 * @deprecated
 *
 * @todo Can this be removed? If not change the inheritance to AbstractUpdater.
 **/
template< class T > class UpdaterSynchronize: public IngredientsUpdater < T >
{
public:
	UpdaterSynchronize( T& val):IngredientsUpdater< T >(val){};
	bool execute();

	  /**
	   * @brief This function is called \a once in the beginning of the TaskManager.
	   *
	   * @details It´s a virtual function for inheritance.
	   * Use this function for initializing tasks (e.g. init SDL)
	   *
	   **/
	  virtual void initialize(){};

	  /**
	   * @brief This function is called \a once in the end of the TaskManager.
	   *
	   * @details It´s a virtual function for inheritance.
	   * Use this function for cleaning tasks (e.g. destroying arrays, file outut)
	   *
	   **/
	  virtual void cleanup(){};
};

template<class T >
bool UpdaterSynchronize<T>::execute()
{
	this->getIngredients().synchronize();
	return true;
}

#endif /* LEMONADE_UPDATER_UPDATERSYNCHRONIZE_H */
