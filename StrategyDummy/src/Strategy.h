#pragma once

#include <iostream>
#include <vector>

#include "../../Commun/IStrategyLib.h"
//#include "StrategyDiffDice.h"
//#include "StrategyBasic.h"
#include "StrategyMix.h"


class Strategy : public IStrategy
{
public:
	Strategy() : Current(nullptr) {}
	virtual ~Strategy() { delete Current; }
	void InitGame(unsigned int id, unsigned int nbPlayer, const SMap* map);
	bool PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn);
	void EndGame(unsigned int idWinner);

protected:
	//StrategyDiffDice* Current;
	//StrategyBasic* Current;
	StrategyMix* Current;
};
