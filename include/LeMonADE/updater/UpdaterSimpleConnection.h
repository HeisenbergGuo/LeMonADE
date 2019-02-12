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

#ifndef LEMONADE_UPDATER_UPDATERSIMPLECONNECTION_H
#define LEMONADE_UPDATER_UPDATERSIMPLECONNECTION_H


#include <LeMonADE/updater/moves/MoveLocalBase.h>
#include <LeMonADE/utility/RandomNumberGenerators.h>
#include<LeMonADE/updater/AbstractUpdater.h>
#include <LeMonADE/updater/moves/MoveConnectSc.h>

/**
 * @file
 *
 * @class UpdaterSimpleConnection
 *
 * @brief Simple simulation updater for generic polyrotaxanes.
 *
 * @details It takes the type of move as template argument MoveType
 * and the number of mcs to be executed as argument for the constructor
 *
 * @tparam IngredientsType Ingredients class storing all system information( e.g. monomers, bonds, etc).
 * @tparam MoveType name of the specialized move.
 */

template<class IngredientsType,class MoveType, class ConnectionMoveType>
class UpdaterSimpleConnection:public AbstractUpdater
{

public:
  /**
   * @brief Standard Constructor initialized with ref to Ingredients and MCS per cycle
   *
   * @param ing a reference to the IngredientsType - mainly the system
   * @param steps MCS per cycle to performed by execute()
   */
  UpdaterSimpleConnection(IngredientsType& ing,uint32_t steps = 1 )
  :ingredients(ing),nsteps(steps){}
  
 
  /**
   * @brief This checks all used Feature and applies all Feature if all conditions are met.
   *
   * @details This function runs over \a steps MCS and performs the moves.
   * It setting the age of the system and prints a simple simple simulation speed
   * in the number of attempted monomer moves per s (tried and performed monomer moves/s).
   *
   * @return True if function are done.
   */
  bool execute();

  /**
   * @brief This function is called \a once in the beginning of the TaskManager.
   *
   * @details It´s a virtual function for inheritance.
   * Use this function for initializing tasks (e.g. init SDL)
   *
   **/
  virtual void initialize();

  /**
   * @brief This function is called \a once in the end of the TaskManager.
   *
   * @details It´s a virtual function for inheritance.
   * Use this function for cleaning tasks (e.g. destroying arrays, file outut)
   *
   **/
  virtual void cleanup(){};

private:
  //! Specialized move to be used for the movement of the monomers 
  MoveType move;
  //! 
  ConnectionMoveType connectionMove;
  //! Number of mcs to be executed
  uint32_t nsteps;
  //! random number generator (seed set in main program)
  RandomNumberGenerators rng;
  //! 
  IngredientsType& ingredients;
  //! number of monomers which could have a reaction bond
  uint32_t NReactiveSites;
  //! number of already reacted monomers
  uint32_t NReactedSites;
  //! get conversion of the reaction process
  double getConversion(){return (1.*NReactedSites)/(1.*NReactiveSites);};
  
};
/**Implementation of the member functions
 * @brief 
 */
template<class IngredientsType,class MoveType, class ConnectionMoveType>
bool UpdaterSimpleConnection<IngredientsType,MoveType,ConnectionMoveType>::execute()
{
  
      time_t startTimer = time(NULL); //in seconds
      std::cout<<"connection mcs "<<ingredients.getMolecules().getAge() << " passed time " << ((difftime(time(NULL), startTimer)) ) <<std::endl;
      for(int n=0;n<nsteps;n++)
      {
	  std::vector<uint32_t> TagedMonomers; 
	for(size_t m=0;m<ingredients.getMolecules().size();m++)
	{
		if( ingredients.getMolecules()[m].IsReactive() ) TagedMonomers.push_back(m);
		move.init(ingredients);
		if(move.check(ingredients)==true)
		{
			move.apply(ingredients);
		}
	}
	uint32_t nTags(TagedMonomers.size());
	
	for(uint32_t i =0; i < nTags;i++)
	{
		// choose a random monomer 
		uint32_t ID(TagedMonomers[ rng.r250_rand32() % nTags ]);
		//vicinity reaction  
		connectionMove.init(ingredients,ID);
		if ( connectionMove.check(ingredients) )
		{
		  connectionMove.apply(ingredients);
		  NReactedSites+=2;
		}
	}
	ingredients.modifyMolecules().setAge(ingredients.getMolecules().getAge()+1);
      }
      std::cout <<"Conversion at "<<ingredients.getMolecules().getAge() << " is " << getConversion()  << std::endl;
      std::cout<<"connection mcs "<<ingredients.getMolecules().getAge() << " with " << (((1.0*nsteps)*ingredients.getMolecules().size())/(difftime(time(NULL), startTimer)) ) << " [attempted connections/s]" <<std::endl;
      std::cout<<"connection mcs "<<ingredients.getMolecules().getAge() << " passed time " << ((difftime(time(NULL), startTimer)) ) << " with " << nsteps << "connection MCS "<<std::endl;
      return false;
};
template<class IngredientsType,class MoveType, class ConnectionMoveType>
void  UpdaterSimpleConnection<IngredientsType,MoveType,ConnectionMoveType>::initialize()
{
  for(uint32_t i = 0 ; i < ingredients.getMolecules().size(); i++ )
  {
    if ( ingredients.getMolecules()[i].IsReactive() )
    {
      uint32_t NLinks(ingredients.getMolecules().getNumLinks(i));
      uint32_t nIrreversibleBonds;
      for (uint32_t n = 0 ; n < NLinks ;n++)
      {
	uint32_t neighbor(ingredients.getMolecules().getNeighborIdx(i,n));
	if( ingredients.getMolecules()[i].IsReactive() )
	  NReactedSites+=2;
	nIrreversibleBonds++;
      }
      NReactiveSites+=(ingredients.getMolecules()[i].getNMaxBonds()-nIrreversibleBonds);
    }
  }
};
#endif 	/*LEMONADE_UPDATER_UPDATERSIMPLECONNECTION_H*/