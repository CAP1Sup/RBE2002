#include "MazeSolver.h"
extern Maze defaultMaze(MAX_WIDTH, MAX_HEIGHT); // Define a default Maze object

MazeSolver::MazeSolver() : maze(defaultMaze), openListSize(0) {}
MazeSolver::MazeSolver(const Maze &maze) : maze(maze), openListSize(0) {}

bool MazeSolver::findPath(Node *start, Node *goal, Node path[],
                          int &pathLength) {

  openListSize = 0; // Initialize the open list
  addToOpenList(start);
  while (openListSize > 0) {
    // Serial.println("getting Current node");
    Node *current = popBestNode(*goal); // Find the best node in the open list
    if (current->x == goal->x &&
        current->y == goal->y) { // Check if the current node is the goal node
      // Serial.println(F("Goal found"));
      reconstructPath(*current, path, pathLength, start);
      return true;
    }

    Node *neighborsPtr[MAX_NEIGHBORS];

    int numNeighborsPtr = getNeighbors(
        *current, neighborsPtr); // Get the neighbors of the current node
    for (int i = 0; i < numNeighborsPtr; ++i) {
      if (!neighborsPtr[i]->isEqual(*current)) {
        neighborsPtr[i]->parent = current;
        addToOpenList(neighborsPtr[i]);
      }
      Serial.println();
    }
  }
  pathLength = 0;
  return false;
}

void MazeSolver::addToOpenList(Node *node) {
  if (openListSize < MAX_NODES) { // Check if there is space in the open list
    openList[openListSize++] = node;
  }
}

Node *MazeSolver::popBestNode(const Node &goal) {
  int bestIndex = 0;

  // Find the node with the lowest cost
  float bestCost = calculateHeuristic(goal, *openList[0]);
  for (int i = 1; i < openListSize; ++i) {
    float currentCost = calculateHeuristic(goal, *openList[i]);
    if (currentCost < bestCost) {
      bestCost = currentCost;
      bestIndex = i;
    }
  }

  Node *bestNode = openList[bestIndex]; // Store the best node

  // Shift the remaining elements left
  for (int i = bestIndex; i < openListSize - 1; ++i) {
    openList[i] = openList[i + 1];
  }
  openListSize--;

  return bestNode;
}

int MazeSolver::calculateHeuristic(const Node &a, const Node &b) {
  // Manhattan distance as a heuristic
  return fabs(a.x - b.x) + fabs(a.y - b.y);
}

int MazeSolver::getNeighbors(const Node &node, Node *neighbors[]) {
  uint8_t count = 0;
  // Check Up
  if (node.y < MAX_HEIGHT && !node.getWallUp() &&
      !(node.parent == defaultMaze.getNode(node.x, node.y + 1))) {
    neighbors[count++] = defaultMaze.getNode(node.x, node.y + 1);
  }
  // Check Down
  if (node.y > 0 && !node.getWallDown() &&
      !(node.parent == defaultMaze.getNode(node.x, node.y - 1))) {
    neighbors[count++] = defaultMaze.getNode(node.x, node.y - 1);
  }
  // Check Left
  if (node.x > 0 && !node.getWallLeft() &&
      !(node.parent == defaultMaze.getNode(node.x - 1, node.y))) {
    neighbors[count++] = defaultMaze.getNode(node.x - 1, node.y);
  }

  if (node.x < MAX_WIDTH && !node.getWallRight() &&
      !(node.parent == defaultMaze.getNode(node.x + 1, node.y))) {
    neighbors[count++] = defaultMaze.getNode(node.x + 1, node.y);
  }
  return count; // Number of neighbors added
}

void MazeSolver::reconstructPath(Node &goalNode, Node path[], int &pathLength,
                                 Node *startNode) {
  Node *currentNode = &goalNode;
  Node *temp;

  pathLength = 0;

  // Backtrack from the goal node to the start node using parents
  while (currentNode != startNode) {
    path[pathLength++] = *currentNode; // Add the node to the path
    temp = currentNode;
    currentNode = currentNode->parent; // Move to the parent node
    currentNode->child = temp;
  }

  // The path is currently from goal to start, so it needs to be reversed
  for (int i = 0; i < pathLength / 2; ++i) {
    Node temp = path[i];
    path[i] = path[pathLength - 1 - i];
    path[pathLength - 1 - i] = temp;
  }
}

void MazeSolver::printPath(Node path[], int pathLength) {
  for (int i = 0; i < pathLength; i++) {
    Serial.print(F("Path "));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.print(path[i].x);
    Serial.print(", ");
    Serial.println(path[i].y);
    Serial.flush();
  }
}