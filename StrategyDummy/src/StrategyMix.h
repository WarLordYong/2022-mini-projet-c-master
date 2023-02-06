#pragma once

#include "../../Commun/IStrategyLib.h"
#include "Utilities.h"

class StrategyMix
{
public:
	StrategyMix(unsigned int id, unsigned int nbPlayer, const SMap* map);
	StrategyMix(const StrategyMix &obj) =delete;
	StrategyMix(StrategyMix &&obj) =delete;
	~StrategyMix();
	StrategyMix& operator=(const StrategyMix &obj) =delete;
	StrategyMix& operator=(StrategyMix &&obj) =delete;

    void updateTurn(const SGameState *state);

public:
	bool PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn);

protected:
	const unsigned int Id;
	const unsigned int NbPlayer;
	SMap Map;
    unsigned int *Points; // Points de chaque joueur
	unsigned int *DiceStock; // Réserve de dés de chaque joueur
	int NbVoid; // nb de tour où l'on a rien fait
};