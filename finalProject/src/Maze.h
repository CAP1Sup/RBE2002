/**
 * @file Maze.h
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

const int MAX_WIDTH = 6; // Define maximum dimensions for the maze
const int MAX_HEIGHT = 3;

class Maze {
public:
  /**
   * @brief Construct a new Maze object
   * Default constructor
   *
   */
  Maze(){};
  /**
   * @brief Construct a new Maze object
   *
   * @param width  Max Width
   * @param height Max Height
   */
  Maze(int width, int height);

  /**
   * @brief Check if there is a wall at the given coordinates
   *
   * @param x Coordinate x
   * @param y Coordinate y
   * @return true
   * @return false
   */
  bool isWall(int x, int y) const;

  /**
   * @brief Set the Wall object
   *
   * @param x Coordinate X
   * @param y Coodinate Y
   * @param hasWall
   */
  void setWall(int x, int y, bool hasWall);

  /**
   * @brief Get the Width object
   *
   * @return int
   */
  int getWidth() const;

  /**
   * @brief Get the Height object
   *
   * @return int
   */
  int getHeight() const;

private:
  bool grid[MAX_WIDTH][MAX_HEIGHT];
  int width;
  int height;
};