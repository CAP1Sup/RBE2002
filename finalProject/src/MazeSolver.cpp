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
      reconstructPath(&current, path, pathLength); // IMPLEMENT THIS
      return true;
    }

    Node neighbors[MAX_NEIGHBORS];
    int numNeighbors = getNeighbors(current, neighbors);
    for (int i = 0; i < numNeighbors; ++i) {
      Node &neighbor = neighbors[i];
      if (!visited[neighbor.x][neighbor.y]) {
        neighbor.cost = current.cost + edgeCost(current, neighbor);
        neighbor.heuristic = calculateHeuristic(neighbor, goal);
        neighbor.parent = new Node(current); // Consider memory management
        addToOpenList(neighbor);
      }
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
  // Implement logic to find and remove the best node from openList
  // This could be a simple linear search for the node with the lowest total
  // cost Placeholder implementation - needs proper logic
  Node bestNode = openList[0];
  return bestNode;
}

float MazeSolver::calculateHeuristic(const Node &a, const Node &b) {
  // Manhattan distance as a heuristic
  return fabs(a.x - b.x) + fabs(a.y - b.y);
}

float MazeSolver::edgeCost(const Node &a, const Node &b) {
  // Assuming a simple cost model - can be modified
  return 1.0;
}

int MazeSolver::getNeighbors(const Node &node, Node neighbors[]) {
  int count = 0;

  // Check Up
  if (node.y > 0 && !maze.isWall(node.x, node.y - 1)) {
    neighbors[count++] = Node(node.x, node.y - 1);
  }
  // Check Down
  if (node.y < maze.getHeight() - 1 && !maze.isWall(node.x, node.y + 1)) {
    neighbors[count++] = Node(node.x, node.y + 1);
  }
  // Check Left
  if (node.x > 0 && !maze.isWall(node.x - 1, node.y)) {
    neighbors[count++] = Node(node.x - 1, node.y);
  }
  // Check Right
  if (node.x < maze.getWidth() - 1 && !maze.isWall(node.x + 1, node.y)) {
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
