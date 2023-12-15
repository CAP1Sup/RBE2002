/**
 * @file Node.h
 * @author Kang Zhang
 * @brief
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <stdint.h>

#define UP_MASK 0b0001
#define RIGHT_MASK 0b0010
#define DOWN_MASK 0b0100
#define LEFT_MASK 0b1000

class Node {
public:
  uint8_t x, y; //
  Node *parent; // Parent node in the path
  Node *child;

  /**
   * @brief Construct a new Node object
   * Default constructor
   *
   */
  Node();
  /**
   * @brief Construct a new Node object
   *
   * @param x Coordinate X
   * @param y Coordinate Y
   */
  Node(uint8_t x, uint8_t y);

  /**
   * @brief Check if the node has valid coordinates
   *
   * @return true
   * @return false
   */
  bool isValid() const; // Check if the node has valid coordinates

  /**
   * @brief Check if the node is equal to another node
   *
   * @param other Node to Compare to
   * @return true
   * @return false
   */
  bool isEqual(Node &other);

  /**
   * @brief Check if the node is equal to another node
   *
   * @param other Node to Compare to (const)
   * @return true
   * @return false
   */
  bool isEqual(const Node &other);

  /**
   * @brief Set the Walls object
   *
   * @param north
   * @param east
   * @param south
   * @param west
   */
  void setWalls(bool north, bool east, bool south, bool west);

  /**
   * @brief Get the Wall Up object
   *
   * @return true
   * @return false
   */
  bool getWallUp() const;
  /**
   * @brief Get the Wall Right object
   *
   * @return true
   * @return false
   */
  bool getWallRight() const;
  /**
   * @brief Get the Wall Down object
   *
   * @return true
   * @return false
   */
  bool getWallDown() const;
  /**
   * @brief Get the Wall Left object
   *
   * @return true
   * @return false
   */
  bool getWallLeft() const;

  /**
   * @brief Set the Wall Up object
   *
   * @param wall
   */
  void setWallUp(bool wall);
  /**
   * @brief Set the Wall Right object
   *
   * @param wall
   */
  void setWallRight(bool wall);
  /**
   * @brief Set the Wall Down object
   *
   * @param wall
   */
  void setWallDown(bool wall);
  /**
   * @brief Set the Wall Left object
   *
   * @param wall
   */
  void setWallLeft(bool wall);

  /**
   * @brief Print the wall layout
   *
   */
  void printWall();

  /**
   * @brief Get the X object
   *
   * @return int
   */
  int getX() const;
  /**
   * @brief Get the Y object
   *
   * @return int
   */
  int getY() const;

  uint8_t walls = 0b0000;
};