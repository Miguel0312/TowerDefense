#include "Grid.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include <algorithm>

Grid::Grid(class Game *game, int rows, int columns) : Actor(game)
{
  mRows = rows;
  mColumns = columns;
  mTileSize = 64;
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

  mBeginTile = GetTile(5, 0);
  mBeginTile->SetTileState(Tile::TileState::EStart);
  mEndTile = GetTile(5, 15);
  mEndTile->SetTileState(Tile::TileState::EBase);

  enemyTimer = 1.0f;
  mRunning = false;
}

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

void Grid::GenerateOutput(SDL_Renderer *renderer)
{
  for (auto tile : mTiles)
    tile->GenerateOutput(renderer);
}

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
      enemyTimer = 5.0f;
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

void Grid::AddTower(Vector2 position)
{
  Tile *target = GetTile(static_cast<int>(position.y) / mTileSize, static_cast<int>(position.x) / mTileSize);

  if (target->mBlocked)
    return;

  target->mBlocked = true;
  if (!FindPath(GetBegin(), GetEnd()))
  {
    target->mBlocked = false;
    return;
  }

  mTowers.emplace_back(new Tower(GetGame(), this, target));
  UpdatePathTiles(GetBegin(), GetEnd());
  UpdateActor(0);
}

void Grid::AddProjectile(Tower *tower)
{
  Projectile *proj = new Projectile(GetGame(), this, tower->ClosestEnemy());
  proj->SetPosition(tower->GetPosition());
  mProjectiles.emplace_back(proj);
}