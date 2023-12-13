#include "MazeSolver.h"
static Maze defaultMaze(MAX_WIDTH, MAX_HEIGHT); // Define a default Maze object

MazeSolver::MazeSolver() : maze(defaultMaze), openListSize(0) {}
MazeSolver::MazeSolver(const Maze &maze) : openListSize(0), maze(maze) {}

bool MazeSolver::findPath(Node start, Node goal, Node path[], int &pathLength) {
  goalNode = goal; // Store the goal node
  openListSize = 0;
  addToOpenList(&start);
  Serial.println("Finding path");
  Serial.print(openListSize);
  Serial.flush();
  delay(1000);
  while (openListSize > 0) {
    Node *current = popBestNode(goalNode);
    if (current->x == goalNode.x && current->y == goalNode.y) {
      reconstructPath(current, path, pathLength);
      return true;
    }
    Node neighbors[MAX_NEIGHBORS];
    int numNeighbors = getNeighbors(*current, neighbors);
    for (int i = 0; i < numNeighbors; ++i) {
      Serial.println(i);
      Node *neighbor = &neighbors[i];
      Serial.print("Neighbor: ");
      Serial.flush();
      neighbor->parent = current;
      addToOpenList(neighbor);
    }
  }

  pathLength = 0;
  return false;
}

void MazeSolver::addToOpenList(Node *node) {
  if (openListSize < MAX_NODES) {
    openList[openListSize++] = node;
  }
}

Node *MazeSolver::popBestNode(const Node &goal) {
  int bestIndex = 0;
  float bestCost = calculateHeuristic(*openList[0], goal);

  for (int i = 1; i < openListSize; ++i) {
    float currentCost = calculateHeuristic(*openList[i], goal);
    if (currentCost < bestCost) {
      bestCost = currentCost;
      bestIndex = i;
    }
  }

  Node *bestNode = openList[bestIndex];
  for (int i = bestIndex; i < openListSize - 1; ++i) {
    openList[i] = openList[i + 1];
  }
  openListSize--;
  return bestNode;
}

float MazeSolver::calculateHeuristic(const Node &a, const Node &b) {
  // Manhattan distance as a heuristic
  return fabs(a.x - b.x) + fabs(a.y - b.y);
}

int MazeSolver::getNeighbors(const Node &node, Node neighbors[]) {
  uint8_t count = 0;
  // Check Up
  if (node.y > 0 && !maze.isWall(node.x, node.y - 1)) {
    Serial.println("Up");
    neighbors[count++] = Node(node.x, node.y - 1);
  }
  // Check Down
  if (node.y < MAX_HEIGHT - 1 && !maze.isWall(node.x, node.y + 1)) {
    Serial.println("Down");
    neighbors[count++] = Node(node.x, node.y + 1);
  }
  // Check Left
  if (node.x > 0 && !maze.isWall(node.x - 1, node.y)) {
    Serial.println("Left");
    neighbors[count++] = Node(node.x - 1, node.y);
  }

  if (node.x < MAX_WIDTH - 1 && !maze.isWall(node.x + 1, node.y)) {
    Serial.println("Right");
    neighbors[count++] = Node(node.x + 1, node.y);
  }
  return count; // Number of neighbors added
}

void MazeSolver::reconstructPath(Node *node, Node path[], int &pathLength) {

  // Backtrack from the goal node to the start node
  while (node != nullptr) {
    path[pathLength++] = *node;
    node = node->parent; // No deletion needed
  }

  // Reverse the path to get it in start-to-goal order
  for (int i = 0; i < pathLength / 2; ++i) {
    Node temp = path[i];
    path[i] = path[pathLength - 1 - i];
    path[pathLength - 1 - i] = temp;
  }
}
