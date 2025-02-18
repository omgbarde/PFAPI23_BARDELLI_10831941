# Highway Stations Shortest Path Calculator

Welcome to the **Highway Stations Shortest Path Calculator** repository! 

🚗💨 This C program computes the shortest paths between highway service stations, following the specifications provided in the reference file.

## 🚀 Project Overview
This program efficiently calculates the shortest paths between service stations on a highway using different algorithms depending on the travel direction (left or right). It ensures optimal performance while adhering to the constraints required by the verification server.

[See reference file](https://github.com/omgbarde/PFAPI23_BARDELLI_10831941/blob/cd6ad11e6d63bb85ea36e1b544601eafc9531f49/Testo%20Prova%20Finale%202023.pdf)

## 🛠️ Data Structure Choice
To store the service stations efficiently, I needed a data structure that supports **fast insertions, deletions, and lookups**. Therefore, I opted for a **Red-Black Tree**, which balances these operations efficiently.

## 🔢 Path Planning Algorithms
Different strategies were used based on the direction of travel:
- **Rightward Paths (→)**: A **greedy approach** is used. Starting from the destination and moving backward, the algorithm always selects the **farthest reachable station**. If the starting station is reached, the path is guaranteed to be optimal.
- **Leftward Paths (←)**: A **BFS-style algorithm** is implemented. The search starts from the final station and ensures that the first valid path found is both the **shortest** and the **most optimal** in terms of distance from the start of the highway.

## ⚡ Performance
- Red-Black Tree Operations: Provide efficient insertions, deletions, and lookups in O(log n) time.

- Rightward Path Calculation: Runs in approximately O(n) due to the greedy approach scanning reachable stations.

- Leftward Path Calculation: The BFS traversal ensures an O(n) complexity for finding the shortest path.

<div align="center">
  
  | **memoria max a runtime** 	| **tempo di esecuzione** 	|
  |:-------------------------:	|:-----------------------:	|
  |          40,2 mB          	|          ~ 5,7s         	|

*performance based on non-public tests made by a verifier server*
  
</div>

## 🔧 Possible Optimizations
An additional optimization in the **vehicle addition/removal mechanism** could improve performance further, as this operation represents the main bottleneck. Implementing a **max-heap** or a **tree structure with a pointer to the maximum element** would have significantly enhanced efficiency.

---
Feel free to explore, contribute, or provide feedback! 🚀
