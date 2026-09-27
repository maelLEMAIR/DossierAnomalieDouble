#ifndef GAME_MANAGER_H_DEFINED
#define GAME_MANAGER_H_DEFINED

#include <string>

enum TagMainScene
{
	PLAYER_TAG
};

class GameManager
{
public:
	GameManager();

	void Run();

	static GameManager& GetInstance() { return *s_pInstance; }
	static std::string GetOtherPlayerId() { return s_pInstance->idOtherPlayer; }
	static void SetOtherPlayerId(std::string _id) { s_pInstance->idOtherPlayer = _id; }

private:
	inline static GameManager* s_pInstance = nullptr;

	std::string idOtherPlayer;
};

#endif