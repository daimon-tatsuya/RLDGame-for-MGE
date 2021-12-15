#pragma once

//**********************************************************
//
//		EnemyManagerƒNƒ‰ƒX
//
//**********************************************************


#include <vector>

//‘O•ûéŒ¾
class Character;

class EnemyManager
{
private:

	std::vector<Character*>	 enemies;// ƒGƒlƒ~[‚¾‚¯‚ğŠi”[‚·‚é
	std::vector<Character*>	 removes; // íœ‚·‚éEnemy(Character)‚ğŠi”[‚·‚é‚µ‚Ä, characteres‚Ìindex‚ğw’è‚µ‚Ä’¼Úíœ‚·‚é‚Ì‚ğ‰ñ”ğ
public:

private:

	EnemyManager() {}
	~EnemyManager();

public:

	//EnemyManager‚Ì¶¬‚Æíœ‚Í‘S‚ÄCharacterManager‚©‚çs‚¤
	 friend class CharacterManager;

	// “G‚Ì‚ğ‘Síœ
	void Clear();

	/// <summary>
	/// “G‚ğ“o˜^
	/// </summary>
	/// <param name="character">“o˜^‚·‚éƒLƒƒƒ‰</param>
	void Register(Character* character);

	/// <summary>
	/// “G‚ğíœ
	/// </summary>
	/// <param name="character">íœ‚·‚éƒLƒƒƒ‰</param>
	void Remove(Character* character);

//------------------------------------------------
//
// Getter
//
//------------------------------------------------

	// “G‚ğ”æ“¾
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// “G”z—ñ‚ğæ“¾
	std::vector<Character*> GetEnemis() { return enemies; }

	// “G‚ğæ“¾
	Character* GetEnemy(int index) { return enemies.at(index); }

	// ID‚©‚ç“G‚ğæ“¾
	Character* GetEnemyFromId(int id);

};
