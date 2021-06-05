#!/usr/bin/env python3
"""
..
  Copyright 2021

.. moduleauthor:: Jan Piotr Buchmann <jan.buchmann@hhu.de>
"""

import io
import os
import sys
import graphviz

class Node:

  def __init__(self, name, label, start, end, maxval, left, right, feature):
    self.name = name
    self.label = label
    self.start = start
    self.end = end
    self.max = maxval
    self.left = None if left == '0' else left
    self.right = None if right == '0' else right
    self.feature = feature

  def node_values(self):
    return f"{self.label}  {self.name}  {self.feature}\\n{self.start}-{self.end}: {self.max}\\nl:{self.left}   r:{self.right}"


def main():
  nodes = {}
  dot = graphviz.Digraph()
  nodecount = 0

  for i in sys.stdin:
    if i[0] == "#":
      continue
    cols = i.strip().split('\t')
    if cols[0] not in nodes:
      nodes[cols[0]] = Node(cols[0], nodecount, cols[1], cols[2], cols[3], cols[4], cols[5], cols[6])
      nodecount += 1
    else:
      sys.exit(f"Node: {cols[0]} not unique. Error.")

  for i in nodes:
    dot.node(i, label=nodes[i].node_values())
    if nodes[i].left:
      dot.edge(i, nodes[i].left)
    if nodes[i].right:
      dot.edge(i, nodes[i].right)
  print(dot.source)
  return 0

if __name__ == '__main__':
  main()
