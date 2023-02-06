#include "Utilities.h"

int getNbTerritories(unsigned int IDPlayer, const SGameState *state) {
	int nbTerr = 0;
	for (unsigned int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbTerr++;
	}
	return nbTerr;
}

int getNbDices(unsigned int IDPlayer, const SGameState * state)
{
	int nbDices = 0;
	for (unsigned int i = 0; i < state->nbCells; i++)
	{
		if (state->cells[i].owner == IDPlayer)
			nbDices += state->cells[i].nbDices;
	}
	return nbDices;
}

// Fonction pour calculer un score d'attaque, basé sur le nombre de voisins de la cellule attaquante, 
// son nombre de dés, et le nombre de voisins ennemis qu'elle a et leur nombre de dés
double CalculScore(unsigned int id, SCell& cellule, SCell& enemy, bool success) {
	if (success == true) { // calcul du score si l'attaque réussie
		double score = 0;

		// on calcule le score de la cellule que l'on vient de prendre
		double nb_neighbors_enemies = 0;
		double nb_dices_neighbors = 0;
		for (unsigned int i = 0; i < enemy.nbNeighbors; i++) {
			if (enemy.neighbors[i]->infos.owner != id) {
				nb_neighbors_enemies += 1;
				nb_dices_neighbors += enemy.neighbors[i]->infos.nbDices;
			}
		}
		if (nb_neighbors_enemies != 0) { // si la cellule que l'on vient de prendre a des voisins ennemis
			score += (cellule.infos.nbDices - 1) - (nb_dices_neighbors / nb_neighbors_enemies); // varie entre -7 et 6
		}
		else { // si elle n'a pas de voisins ennemies, son score est son nombre de dés
			score += cellule.infos.nbDices - 1; // varie entre 1 et 7
		}

		// pour la cellule avec laquelle on attaque
		// score de la cellule avant attaque
		nb_neighbors_enemies = 0;
		nb_dices_neighbors = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_neighbors_enemies += 1;
				nb_dices_neighbors += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_before = (nb_dices_neighbors / nb_neighbors_enemies) - (cellule.infos.nbDices); // varie entre -7 et 6

		// score de la cellule après attaque
		nb_neighbors_enemies = 0;
		nb_dices_neighbors = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id && cellule.neighbors[i]->infos.id != enemy.infos.id) {
				nb_neighbors_enemies += 1;
				nb_dices_neighbors += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_after = 1; // après avoir attaquer il nous reste forcément 1 dès sur la cellule
		if (nb_neighbors_enemies != 0) {
			score_after = (nb_dices_neighbors / nb_neighbors_enemies) - 1; // varie entre 0 et 7
		}

		// score total de l'attaque : addition du score de la cellule à attaquer 
		// + la différence de score de la cellule qui attaque, entre avant et après avoir attqué
		score += (score_after - score_before); // varie entre -13 et 21

		return score;
	}
	else { // calcul du score si l'attaque échoue
		double score = 0;

		// pour la cellule avec laquelle on attaque
		// score avant attaque
		double nb_neighbors_enemies = 0;
		double nb_dices_neighbors = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_neighbors_enemies += 1;
				nb_dices_neighbors += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_before = (nb_dices_neighbors / nb_neighbors_enemies) - (cellule.infos.nbDices); // varie entre -7 et 6

		// score après attaque
		nb_neighbors_enemies = 0;
		nb_dices_neighbors = 0;
		for (unsigned int i = 0; i < cellule.nbNeighbors; i++) {
			if (cellule.neighbors[i]->infos.owner != id) {
				nb_neighbors_enemies += 1;
				nb_dices_neighbors += cellule.neighbors[i]->infos.nbDices;
			}
		}
		double score_after = 1; // après avoir attaqué il nous reste forcément 1 dès sur la cellule
		if (nb_neighbors_enemies != 0) {
			score_after = (nb_dices_neighbors / nb_neighbors_enemies) - 1; // varie entre 0 et 7
		}
		
		// score total de l'attaque 
		score += (score_after - score_before); // varie entre -6 et 14

		return score;
	}
}

// La probabilité virtuelle qu'une cellule attaque avec succès, en fonction du nombre de dés 
// dans la cellule qui attaque et du nombre de dés dans la cellule qui est attaqué.
double ProbabilitySuccess(unsigned int nbDes, unsigned int nbDesVoisin) {
	return (nbDes - nbDesVoisin + 1) / nbDes; // varie entre 0.125 et 1
}