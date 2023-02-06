#include "interface_lib.h"
#include "Strategy.h"
#include <iostream>

API_EXPORT void InitStrategy(SInfo* info)
{
	std::cout << "InitStrategyMix" << std::endl;

	info->SetName("StrategyMix");

	info->AddMember("Chenrui ZHU");
	info->AddMember("Xiangyong LI");
	info->AddMember("Mamisoa RANDRIANARIM");
	info->AddMember("Xianxiang ZHANG");
}

API_EXPORT IStrategy* StrategyFactory()
{
	return(new Strategy());
}

API_EXPORT void StrategyDelete(IStrategy* obj)
{
	delete(obj);
}

// ----------------------------------------------------------------------------

void Strategy::InitGame(unsigned int id, unsigned int nbPlayer, const SMap* map)
{
	if (Current) delete Current;
	//Current = new StrategyDiffDice(id, nbPlayer, map);
	//Current = new StrategyBasic(id, nbPlayer, map);
	Current = new StrategyMix(id, nbPlayer, map);
}

bool Strategy::PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn)
{
	if (Current) return(Current->PlayTurn(gameTurn, state, turn));
	return(false);
}

void Strategy::EndGame(unsigned int /*idWinner*/)
{
	if (Current)
	{
		delete Current;
		Current = nullptr;
	}
}
