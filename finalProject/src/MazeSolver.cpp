#include "MazeSolver.h"
static Maze defaultMaze(MAX_WIDTH, MAX_HEIGHT); // Define a default Maze object

MazeSolver::MazeSolver() : maze(defaultMaze), openListSize(0) {}
MazeSolver::MazeSolver(const Maze &maze) : maze(maze), openListSize(0) {}

bool MazeSolver::findPath(Node start, Node goal, Node path[], int &pathLength) {
  goalNode = goal; // Store the goal node
  openListSize = 0;
  addToOpenList(&start);
  delay(3000);
  while (openListSize > 0) {
    Node *current = popBestNode(goalNode);
    if (current->x == goalNode.x && current->y == goalNode.y) {
      reconstructPath(*current, path, pathLength);
      return true;
    }
    Node neighbors[MAX_NEIGHBORS];
    int numNeighbors = getNeighbors(*current, neighbors);

    Serial.flush();
    for (int i = 0; i < numNeighbors; ++i) {
      Node *neighbor = new Node(neighbors[i]); // Dynamically allocate new Node
      if (!neighbor->isEqual(*current)) {
        neighbor->parent = current;
        addToOpenList(neighbor);
      } else {
        delete neighbor; // Avoid memory leak if neighbor is the same as current
      }
    }
    delay(100);
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
  float bestCost = calculateHeuristic(goal, *openList[0]);

  for (int i = 1; i < openListSize; ++i) {
    float currentCost = calculateHeuristic(goal, *openList[i]);
    if (currentCost < bestCost) {
      bestCost = currentCost;
      bestIndex = i;
    }
  }

  Node *bestNode = openList[bestIndex];

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

int MazeSolver::getNeighbors(const Node &node, Node neighbors[]) {
  uint8_t count = 0;
  // Check Up
  if (node.y > 0 && !maze.isWall(node.x, node.y - 1) &&

      !node.parent->isEqual(Node(node.x, node.y - 1))) {
    // Serial.println("Up");
    neighbors[count++] = Node(node.x, node.y - 1);
  }
  // Check Down
  if (node.y < MAX_HEIGHT - 1 && !maze.isWall(node.x, node.y + 1) &&
      !node.parent->isEqual(Node(node.x, node.y + 1))) {
    // Serial.println("Down");
    neighbors[count++] = Node(node.x, node.y + 1);
  }
  // Check Left
  if (node.x > 0 && !maze.isWall(node.x - 1, node.y) &&
      !node.parent->isEqual(Node(node.x - 1, node.y))) {
    // Serial.println("Left");
    neighbors[count++] = Node(node.x - 1, node.y);
  }

  if (node.x < MAX_WIDTH - 1 && !maze.isWall(node.x + 1, node.y) &&
      !node.parent->isEqual(Node(node.x + 1, node.y))) {
    // Serial.println("Right");
    neighbors[count++] = Node(node.x + 1, node.y);
  }
  return count; // Number of neighbors added
}

void MazeSolver::reconstructPath(Node &goalNode, Node path[], int &pathLength) {
  Node *currentNode = &goalNode;
  pathLength = 0;

  // Backtrack from the goal node to the start node
  while (currentNode != nullptr) {
    path[pathLength++] = *currentNode; // Add the node to the path
    currentNode = currentNode->parent; // Move to the parent node
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
    Serial.print("Path ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(path[i].x);
    Serial.print(", ");
    Serial.println(path[i].y);
    Serial.flush();
  }
}