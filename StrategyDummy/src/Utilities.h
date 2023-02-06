#include "../../Commun/IStrategyLib.h"

int getNbTerritories(unsigned int IDPlayer, const SGameState *state);

int getNbDices(unsigned int IDPlayer, const SGameState *state);

double CalculScore(unsigned int id, SCell& cellule, SCell& enemy, bool success);

double ProbabilitySuccess(unsigned int nbDes, unsigned int nbDesVoisin);