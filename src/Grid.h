#pragma once
#include "Actor.h"
#include "Tile.h"

/*
*The Grid is formed by Tiles. It allows the addition of Turrets and spawns the Enemies
*It uses a A* search algorithm to update the path when needed
*/
class Grid : public Actor
{
public:
  Grid(class Game *game, int rows, int columns);

  void UpdateActor(float deltaTime) override;

  bool FindPath(Tile *startTile, Tile *endTile);
  void UpdatePathTiles(Tile *start, Tile *goal);

  Tile *GetTile(int row, int column) { return mTiles[row * mColumns + column]; }
  Tile *GetBegin() const { return mBeginTile; }
  Tile *GetEnd() const { return mEndTile; }

  void AddTower(Vector2 position);
  void AddProjectile(class Tower *Tower);
  void Begin() { mRunning = true; }
  bool IsRunning() { return mRunning; }

  void RemoveProjectile(class Projectile *projectile);
  void RemoveEnemy(class Enemy *enemy);

  std::vector<class Enemy *> GetEnemies() { return mEnemies; }

private:
  std::vector<Tile *> mTiles;
  int mRows;
  int mColumns;
  int mTileSize;
  bool mRunning;

  float enemyTimer;
  std::vector<class Enemy *> mEnemies;

  std::vector<class Tower *> mTowers;
  std::vector<class Projectile *> mProjectiles;

  Tile *mBeginTile;
  Tile *mEndTile;
};