#include "StrategyMix.h"

#include <map>
#include <iostream>
#include <vector>
#include <cmath>

StrategyMix::StrategyMix(unsigned int id, unsigned int nbPlayer, const SMap *map) :
		Id(id),
		NbPlayer(nbPlayer),
		NbVoid(0) {

	Points = new unsigned int[nbPlayer];
	DiceStock = new unsigned int[nbPlayer];
	// faire une copie entière de la structure map localement dans l'objet Map
	std::map<unsigned int, SCell *> mapSCell;
	Map.cells = new SCell[map->nbCells];
	Map.nbCells = map->nbCells;

	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i].nbNeighbors = map->cells[i].nbNeighbors;
		Map.cells[i].neighbors = new SCell *[Map.cells[i].nbNeighbors]();
		Map.cells[i].infos.id = map->cells[i].infos.id;
		mapSCell[Map.cells[i].infos.id] = &Map.cells[i];
	}
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; j++) {
			Map.cells[i].neighbors[j] = mapSCell[map->cells[i].neighbors[j]->infos.id];
		}
	}
}

StrategyMix::~StrategyMix() {
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		delete Map.cells[i].neighbors;
	}
	delete[] Map.cells;
}

void StrategyMix::updateTurn(const SGameState *state) {
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i].infos.nbDices = state->cells[i].nbDices;
		Map.cells[i].infos.owner = state->cells[i].owner;
	}
	for (unsigned int i = 0; i < NbPlayer; i++) {
		Points[i] = state->points[i];
		DiceStock[i] = state->diceStock[i];
	}
}

bool StrategyMix::PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn) {
	updateTurn(state);

	bool play = false;
    bool calcul = false;

    double best_score = 0;
	std::pair<unsigned int, unsigned int> best_action (NULL,NULL);

	for (unsigned int i = 0; i < Map.nbCells; i++) {
		// cellule de ce joueur avec un nombre de dés > 1
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
			for (unsigned int j = 0; j < Map.cells[i].nbNeighbors; j++) {
				if (Map.cells[i].neighbors[j]->infos.owner != Id){
				    // cellule voisine de l'autre joueur avec moins de dés
					if (Map.cells[i].infos.nbDices > Map.cells[i].neighbors[j]->infos.nbDices){
						// on choisie cette cellule à attaquer
						if (!play) {
							turn->cellFrom = Map.cells[i].infos.id;
							turn->cellTo = Map.cells[i].neighbors[j]->infos.id;
							play = true;
						} 
						// on charche une grande diff de nbDices
						else if (pow((Map.cells[i].infos.nbDices - Map.cells[i].neighbors[j]->infos.nbDices),2)
								> pow((Map.cells[turn->cellFrom].infos.nbDices - Map.cells[turn->cellTo].infos.nbDices),2)) {
							turn->cellFrom = Map.cells[i].infos.id;
							turn->cellTo = Map.cells[i].neighbors[j]->infos.id;
						}
					}
				    // ou cellule voisine de l'autre joueur avec les mêmes ou moins de dés
					else if (Map.cells[i].infos.nbDices == Map.cells[i].neighbors[j]->infos.nbDices && !play){
						// ou le nombre de tours sans opération > 5
						if (NbVoid > 5){
							turn->cellFrom = Map.cells[i].infos.id;
							turn->cellTo = Map.cells[i].neighbors[j]->infos.id;
						}
						// ou le nombre de tours sans opération > 2 et < 5
						else if(NbVoid > 2){
							int nbTerritories = getNbTerritories(Id, state);
							int nbDices = getNbDices(Id, state);
							int nbTerritoriesOpponent = getNbTerritories(Map.cells[i].neighbors[j]->infos.owner, state);
							int nbAdjacentCells = 0;

							for(unsigned int k = 0; k < Map.cells[i].neighbors[j]->nbNeighbors; k++){
								if(Map.cells[i].neighbors[j]->neighbors[k]->infos.owner == Id){
									nbAdjacentCells++;
								}
							}
							// Si le nombre de territoires de l'adversaire est inférieur à 2, l'attaquant a un grand avantage
							if(nbTerritoriesOpponent < 2){
								turn->cellFrom = Map.cells[i].infos.id;
								turn->cellTo = Map.cells[i].neighbors[j]->infos.id;
							}
							// Si le nombre de territoires adjacents peuvent être augmenté de plus de 2 une fois l'attaque réussie
							// et le joueur a supérieur ou égal à 3 territoires ou un nombre de dés inférieur ou égal à
							else if(nbAdjacentCells > 2 && ((nbTerritories >= 3) || (nbDices >= 20))){
								turn->cellFrom = Map.cells[i].infos.id;
								turn->cellTo = Map.cells[i].neighbors[j]->infos.id;
							}
						}
						else{
							// Si la cellule adjacente est une cellule adverse et qu'elle a un nombre de dés inférieur à celui de la cellule attaquante, 
                            // CalculScore() sert à calculer le score d'une attaque réussie, le score d'une attaque ratée 
                            // et la probabilité virtuelle d'une attaque réussie.
                            double score_atq_sucess = CalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], true);
                            double score_atq_fail = CalculScore(Id, Map.cells[i], *Map.cells[i].neighbors[j], false);
                            double proba_sucess = ProbabilitySuccess(Map.cells[i].infos.nbDices, Map.cells[i].neighbors[j]->infos.nbDices);
                            
                            // on calcul l'esprance du score obtenu après attaque en fonction des 2 scores 
                            // obtenus et de la probabilité de réussite
                            double score = (score_atq_sucess * proba_sucess) + (score_atq_fail * (1 - proba_sucess)); // varie entre -13 et 21
                            if (score > best_score && !calcul) { 
                                // S'il s'agit de la meilleure attente trouvée jusqu'à présent, 
                                // nous la conserverons et prendrons cette mesure ce tour à moins que nous ne trouvions une meilleure
                                best_score = score;
                                best_action.first = Map.cells[i].infos.id;
                                best_action.second = Map.cells[i].neighbors[j]->infos.id;
                                if(j == Map.cells[i].nbNeighbors-1){
                                    calcul = true;
                                }
                            }
                            if (best_score > 0 && calcul) {
                                turn->cellFrom = best_action.first;
                                turn->cellTo = best_action.second;
                            }
						}
					}
				}
			}
		}
	}

	if (!play) {
		NbVoid++;
		return (0);
	}
	return (1);
}
