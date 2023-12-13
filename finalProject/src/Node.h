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

class Node {
public:
  uint8_t x, y; //
  Node *parent; // Parent node in the path

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
};