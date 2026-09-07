# Ranked Pairs (Tideman) Electoral Graph Algorithms

A rigorous C-language implementation of the Condorcet-compliant **Ranked Pairs (Tideman)** voting system. This repository focuses on algorithmic efficiency, graph theory logic, and topological loop prevention in voting networks.

## 📐 Mathematical Framework & Logic

Unlike simple plurality voting systems where the candidate with the most votes wins, the Tideman system satisfies the **Condorcet criterion**. It evaluates elections by pitching every candidate against each other in a series of pairwise head-to-head matchups. 

The algorithm constructs a **Directed Graph** where:
- **Nodes** represent the political/electoral candidates.
- **Directed Edges (Arrows)** represent a mathematical preference margin (Winner $\rightarrow$ Loser).

### The Topological Challenge: Cycle Detection
The core complexity of the Ranked Pairs method lies in locking the strongest matchups into the graph without creating a **topological cycle (kısır döngü)**. If a cycle forms (e.g., $A$ beats $B$, $B$ beats $C$, and $C$ beats $A$), no clear Condorcet winner can be declared. 

To overcome this, this implementation deploys a recursive **Depth-First Search (DFS)** algorithm that analyzes graph connectivity before locking any edge, guaranteeing an acyclic graph with a clear **Source Node** (a candidate who defeats all others and is never defeated).

---

## 💻 Algorithmic Components

The codebase consists of six heavily optimized sub-routines:

1. `vote()`: Implements **Linear Search** with string comparison to map structural voter preference ranks to candidate identification tokens.
2. `record_preferences()`: Updates a multi-dimensional adjacency matrix tracking macro-level head-to-head statistics.
3. `add_pairs()`: Tallies and translates raw data into discrete, contestable structures containing mathematical winners and losers.
4. `sort_pairs()`: Utilizes **Selection Sort** to rank absolute matchups based on the magnitude of the margin of victory in descending order ($O(n^2)$ efficiency).
5. `lock_pairs()` & `cycle()`: Evaluates connectivity using a recursive branch-and-bound **DFS traversal** to block back-edges and eliminate mathematical cycles.
6. `print_winner()`: Identifies the topological **Source Node** of the directed acyclic graph (DAG) to declare the definitive winner.

---

## 🚀 Execution & Setup

To compile and execute the project locally:

```bash
# Compile the source code using clang
make tideman

# Run the election simulation with candidate profiles
./tideman Alice Bob Charlie
```

