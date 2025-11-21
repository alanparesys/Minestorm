#ifndef BONUS_H
#define BONUS_H
#include "struct.h"
#include "sphere2D.h"

// Nombre maximum de bonus
#define MAX_BONUS 1

// Déclaration externe du tableau de bonus
extern Bonus bonus[MAX_BONUS];

// Timer pour le spawn des bonus
extern float bonusSpawnTimer;
extern bool bonusActive;
extern bool rapidFireActive;
extern float rapidFireCountDown;
extern bool shieldActive;
extern float shieldTimer;
extern float shipBulletCooldown;
//extern float timeSinceLastShot;
/*
extern Enemy bigBasicEnemies[];
extern int maxBigBasicEnemies;
extern bool bigBasicEnemiesSpawn[];
*/
// Initialisation du système de bonus
void InitBonus(void);

// Spawn d'un nouveau bonus
void SpawnBonus(void);

// Update du bonus (timer, collision, etc.)
void UpdateBonus(GameAssets* assets);
void GiveBonus(GameAssets* assets, int i);

// Dessin du bonus
void DrawBonus(GameAssets* assets);

// Fonctions pour le bouclier
void DrawShield(void);
void UpdateShield(void);

// Tuer des ennemis aléatoires
void KillRandomEnemies(GameAssets* assets);

// Obtenir la hitbox du bonus
Sphere2D GetBonusHitbox(int i);
Sphere2D BuildBonusHitboxGeneric(Bonus* bonus, int i);

// Vérifier la collision avec le joueur
void CheckBonusPlayerCollision(GameAssets* assets);

#endif