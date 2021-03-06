#include "Grid.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include <algorithm>

const float SPAWN_TIME = 5.0f;

/*
*The constructor allows the creation of grids of different sizes
*It creates the Tiles that will be used during the game and determines which tile is adjacent to which. This information is used by the path finding algorithm.
*Finally it creates a start and end point to the path that will be created
*/
Grid::Grid(class Game *game, int rows, int columns, int tileSize) : Actor(game)
{
  mRows = rows;
  mColumns = columns;
  mTileSize = tileSize;
  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      Tile *temp = new Tile(GetGame());
      temp->SetPosition(Vector2(j * mTileSize + 32, i * mTileSize + 32));
      mTiles.emplace_back(temp);
    }
  }

  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      Tile *tile = mTiles[i * mColumns + j];
      int index = i * mColumns + j;

      if (j != 0)
        tile->mAdjacent.emplace_back(mTiles[index - 1]);
      if (j != mColumns - 1)
        tile->mAdjacent.emplace_back(mTiles[index + 1]);
      if (i != 0)
        tile->mAdjacent.emplace_back(mTiles[index - mColumns]);
      if (i != mRows - 1)
        tile->mAdjacent.emplace_back(mTiles[index + mColumns]);
    }
  }

  mBeginTile = GetTile(rows / 2, 0);
  mBeginTile->SetTileState(Tile::TileState::EStart);
  mEndTile = GetTile(rows / 2, columns - 1);
  mEndTile->SetTileState(Tile::TileState::EBase);

  enemyTimer = 1.0f;
  mRunning = false;
}

/*
*Determine the shortest path between the star and end points using A* search algorithm
*/
bool Grid::FindPath(Tile *start, Tile *goal)
{
  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      mTiles[i * mColumns + j]->g = 0.0f;
      mTiles[i * mColumns + j]->mInOpenSet = false;
      mTiles[i * mColumns + j]->mInClosedSet = false;
    }
  }

  std::vector<Tile *> openSet;

  // Set current node to start, and add to closed set
  Tile *current = goal;
  current->mInClosedSet = true;

  do
  {
    // Add adjacent nodes to open set
    for (Tile *neighbor : current->mAdjacent)
    {
      //Ignores blocked tiles
      if (neighbor->mBlocked)
      {
        continue;
      }

      // Only check nodes that aren't in the closed set
      if (!neighbor->mInClosedSet)
      {
        if (!neighbor->mInOpenSet)
        {
          // Not in the open set, so set parent
          neighbor->mParent = current;
          neighbor->h = (neighbor->GetPosition() - start->GetPosition()).Length();
          // g(x) is the parent's g plus cost of traversing edge
          neighbor->g = current->g + mTileSize;
          neighbor->f = neighbor->g + neighbor->h;
          openSet.emplace_back(neighbor);
          neighbor->mInOpenSet = true;
        }
        else
        {
          // Compute g(x) cost if current becomes the parent
          float newG = current->g + mTileSize;
          if (newG < neighbor->g)
          {
            // Adopt this node
            neighbor->mParent = current;
            neighbor->g = newG;
            // f(x) changes because g(x) changes
            neighbor->f = neighbor->g + neighbor->h;
          }
        }
      }
    }
    // If open set is empty, all possible paths are exhausted
    if (openSet.empty())
    {
      break;
    }

    // Find lowest cost node in open set
    auto iter = std::min_element(openSet.begin(), openSet.end(),
                                 [](Tile *a, Tile *b)
                                 {
                                   return a->f < b->f;
                                 });
    // Set to current and move from open to closed
    current = *iter;
    openSet.erase(iter);
    current->mInOpenSet = false;
    current->mInClosedSet = true;
  } while (current != start);

  // Did we find a path?
  return (current == start) ? true : false;
}

/*
*Once the path is recalculated we have to update the TileState
*/
void Grid::UpdatePathTiles(Tile *start, Tile *goal)
{
  // Reset all tiles to normal (except for start/end)
  for (int i = 0; i < mRows; i++)
  {
    for (int j = 0; j < mColumns; j++)
    {
      if (!(i == 5 && j == 0) && !(i == 5 && j == 15))
      {
        mTiles[i * mColumns + j]->SetTileState(Tile::EDefault);
      }
    }
  }
  Tile *t = start->mParent;
  while (t != goal)
  {
    t->SetTileState(Tile::EPath);
    t = t->mParent;
  }
}

/*
*Updates the enemy timer, so that one enemy spawns every 5 seconds
*/
void Grid::UpdateActor(float deltaTime)
{
  for (auto tile : mTiles)
  {
    tile->UpdateTile();
  }

  if (mRunning)
  {
    enemyTimer -= deltaTime;
    if (enemyTimer < 0)
    {
      enemyTimer += SPAWN_TIME;
      mEnemies.emplace_back(new Enemy(GetGame(), this));
    }
  }
}

void Grid::RemoveEnemy(Enemy *enemy)
{
  auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
  if (iter != mEnemies.end())
  {
    mEnemies.erase(iter);
  }
}

void Grid::RemoveProjectile(Projectile *enemy)
{
  auto iter = std::find(mProjectiles.begin(), mProjectiles.end(), enemy);
  if (iter != mProjectiles.end())
  {
    mProjectiles.erase(iter);
  }
}

void Grid::Begin()
{
  for (auto tower : mTowers)
  {
    tower->SetState(State::EActive);
  }
  mRunning = true;
}

/*
*First, it checks wheter or not there will be a valid path connecting the start and the end if a Tower is placed at the target Tile. If so, it creates a turret and place it there.
*Then, update the path and the tiles
*/
void Grid::AddTower(Vector2 position)
{
  Tile *target = GetTile(static_cast<int>(position.y) / mTileSize, static_cast<int>(position.x) / mTileSize);

  //Once the game initializes, the Tower can't be placed in the route
  if ((mRunning && target->GetTileState() == Tile::TileState::EPath) || target->mBlocked)
    return;

  target->mBlocked = true;
  if (!FindPath(GetBegin(), GetEnd()))
  {
    target->mBlocked = false;
    return;
  }

  mTowers.emplace_back(new Tower(GetGame(), this, target));
  UpdatePathTiles(GetBegin(), GetEnd());
}

void Grid::AddProjectile(Tower *tower)
{
  Projectile *proj = new Projectile(GetGame(), this, tower->ClosestEnemy());
  proj->SetPosition(tower->GetPosition());
  mProjectiles.emplace_back(proj);
}