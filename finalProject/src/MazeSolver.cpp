#include "MazeSolver.h"
static Maze defaultMaze(MAX_WIDTH, MAX_HEIGHT); // Define a default Maze object

MazeSolver::MazeSolver() : maze(defaultMaze), openListSize(0) {
  // Constructor implementation
  for (int i = 0; i < MAX_WIDTH; ++i) {
    for (int j = 0; j < MAX_HEIGHT; ++j) {
      visited[i][j] = false;
    }
  }
}
MazeSolver::MazeSolver(const Maze &maze) : openListSize(0), maze(maze) {
  for (int i = 0; i < MAX_WIDTH; ++i) {
    for (int j = 0; j < MAX_HEIGHT; ++j) {
      visited[i][j] = false;
    }
  }
}

bool MazeSolver::findPath(Node start, Node goal, Node path[], int &pathLength) {
  openListSize = 0;
  addToOpenList(start);

  while (openListSize > 0) {
    Node current = popBestNode();
    visited[current.x][current.y] = true;

    if (current.x == goal.x && current.y == goal.y) {
      reconstructPath(&current, path, pathLength);
      return true;
    }

    Node neighbors[MAX_NEIGHBORS];
    int numNeighbors = getNeighbors(current, neighbors);
    Serial.print("Num neighbors: ");
    Serial.println(numNeighbors);
    Serial.flush();
    delay(10);
    for (int i = 0; i < numNeighbors; ++i) {
      Node &neighbor = neighbors[i];
      // if (!visited[neighbor.x][neighbor.y]) {
      //   neighbor.cost = current.cost + 1;
      //   neighbor.heuristic = calculateHeuristic(neighbor, goal);
      //   neighbor.parent = new Node(current); // Consider memory management
      //   addToOpenList(neighbor);
      // }
      delay(10);
    }
  }

  pathLength = 0;
  return false; // No path found
}

void MazeSolver::addToOpenList(const Node &node) {
  if (openListSize < MAX_NODES) {
    openList[openListSize++] = node;
  }
}

Node MazeSolver::popBestNode() {
  int bestIndex = 0;
  float bestCost = openList[0].cost + openList[0].heuristic;

  // Find the node with the lowest total cost
  for (int i = 1; i < openListSize; i++) {
    float currentCost = openList[i].cost + openList[i].heuristic;
    if (currentCost < bestCost) {
      bestCost = currentCost;
      bestIndex = i;
    }
  }

  // Save the best node
  Node bestNode = openList[bestIndex];

  // Remove the best node from the open list
  // Shift all elements after the best node one position to the left
  for (int i = bestIndex; i < openListSize - 1; i++) {
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
  if (node.y < maze.getHeight() - 1 && !maze.isWall(node.x, node.y + 1)) {
    Serial.println("Down");
    neighbors[count++] = Node(node.x, node.y + 1);
  }
  // Check Left
  if (node.x > 0 && !maze.isWall(node.x - 1, node.y)) {
    Serial.println("Left");
    neighbors[count++] = Node(node.x - 1, node.y);
  }
  // Check Right
  // Check Right
  Serial.print("Check Right: ");
  Serial.print("x+1 = ");
  Serial.print(node.x + 1);
  Serial.print(", y = ");
  Serial.println(node.y);
  Serial.println(node.x < maze.getWidth() - 1);
  if (node.x < maze.getWidth() - 1 && !maze.isWall(node.x + 1, node.y)) {
    Serial.println("Right");
    neighbors[count++] = Node(node.x + 1, node.y);
  }
  return count; // Number of neighbors added
}

void MazeSolver::reconstructPath(Node *node, Node path[], int &pathLength) {

  // Backtrack from the goal node to the start node
  while (node != nullptr) {
    path[pathLength++] = *node; // Add the node to the path
    Node *previousNode = node;
    node = node->parent;

    // Clean up the dynamically allocated memory
    if (previousNode->parent != nullptr) {
      delete previousNode;
    }
  }

  // Reverse the path to get it in start-to-goal order
  for (int i = 0; i < pathLength / 2; ++i) {
    Node temp = path[i];
    path[i] = path[pathLength - 1 - i];
    path[pathLength - 1 - i] = temp;
  }
}
