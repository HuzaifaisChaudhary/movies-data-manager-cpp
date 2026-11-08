# 🎬 Movies Data Manager – Data Structures in C++ (No STL)

A console-based movie database and recommendation engine built in **C++ without STL containers or algorithms**. Every data structure (an AVL tree, a hash table, linked lists, a graph, and the queue and stack behind its traversals) is implemented by hand.

It loads the **IMDb 5000 Movie Dataset** (5,043 movies) and lets you search movies by title, actor, genre, year or rating. It also recommends similar movies using **BFS/DFS graph traversal** and finds the **shortest connection path** between two movies.

Built as the term project for **Data Structures (Fall 2025)** at FAST-NUCES Islamabad.

---

## ✨ Features

| # | Feature | Data structure used |
|---|---|---|
| 1 | Load and parse 5,043 movies from CSV | Custom CSV parser that handles quoted fields |
| 2 | Search a movie by title (case-insensitive) | **AVL tree**: O(log n) |
| 3 | Search an actor and list their movies | **Hash table** (djb2 hash, chaining) + **linked list** of movies |
| 4 | Display all movies sorted by title | AVL **in-order traversal** (recursive) |
| 5 | Display all actors with their filmographies | Hash table walk |
| 6 | Recommend similar movies (breadth-first) | **Graph + BFS** (queue) |
| 7 | Recommend similar movies (depth-first) | **Graph + DFS** (stack) |
| 8 | Shortest connection path between two movies | **Graph + BFS** with parent tracking |
| 9–11 | Search by genre, year or rating range | Recursive AVL traversal with filters |

## 🧠 How It Works

```
                     ┌──────────────┐
  movie_metadata.csv │  CSV Parser  │
  ─────────────────► │  + cleaning  │
                     └──────┬───────┘
                            │ one MovieNode per movie (heap-allocated)
            ┌───────────────┼────────────────┐
            ▼               ▼                ▼
      ┌──────────┐   ┌─────────────┐   ┌───────────┐
      │ AVL Tree │   │ Actor Hash  │   │   Graph   │
      │ (title)  │   │   Table     │   │ (similar  │
      └──────────┘   └─────────────┘   │  movies)  │
                                       └───────────┘
       all three point to the SAME MovieNode, so no data is duplicated
```

- **Shared-pointer design:** each movie is stored once as a `MovieNode`. The AVL tree, the hash table and the graph each wrap it in their own lightweight node (`MovieNodeAVL`, `MovieNodeHash`, `GraphNode`) that only holds a pointer to it.
- **AVL tree:** a self-balancing BST keyed on title, using LL/RR/LR/RL rotations. Searches stay O(log n) even though the dataset is large.
- **Hash table:** 1,000 buckets with **djb2** hashing and separate chaining. Each `ActorNode` keeps a linked list of the movies that actor appears in.
- **Graph:** an adjacency list where each movie is a vertex. An edge connects two movies that **share an actor** or have the **same genre combination**. Edges are built on demand for the movies you query, which keeps loading fast.
- **BFS / DFS:** BFS uses an array-based queue and returns the closest matches first. DFS uses a stack and follows a chain of related movies.
- **Shortest path:** BFS with parent pointers, then a backtrack from the target movie to rebuild the path.

## 🖥️ Sample Output

```
======= Movies Data Manager =======
1. Load Movies from CSV
2. Search Movie by Title
3. Search Actor by Name
...
Enter choice: 1
Loading movies...
Loaded 5043 movies successfully!

Enter choice: 6
Enter movie title for recommendations: Avatar

=== Recommended Movies (BFS) ===
1. Man of Steel (2013) - Rating: 7.2
2. The Amazing Spider-Man 2 (2014) - Rating: 6.7
3. Star Wars: Episode III - Revenge of the Sith (2005) - Rating: 7.6
4. Star Wars: Episode II - Attack of the Clones (2002) - Rating: 6.7
5. Star Wars: Episode I - The Phantom Menace (1999) - Rating: 6.5

Enter choice: 8
Enter first movie title: The Dark Knight
Enter second movie title: Inception

=== Shortest Path ===
Path length: 2 connections

The Dark Knight (2008)
  |
  v
The Dark Knight Rises (2012)
  |
  v
Inception (2010)
```

## 🛠️ Build & Run

**g++ (Linux / macOS / Windows with MinGW):**
```bash
g++ -std=c++17 -O2 -o movies Source.cpp
./movies          # on Windows: movies.exe
```

**Visual Studio:** create an empty C++ console project, add `Source.cpp` and all the `.h` files, then build and run.

> `movie_metadata.csv` must be in the folder you run the program from. In Visual Studio, that's the project folder. Choose option **1** first to load the data.

## 📁 Project Structure

```
.
├── Source.cpp          # Main menu / entry point
├── IMDB.h              # Controller: owns the AVL tree, hash table and graph; CSV loading
├── MovieNode.h         # Shared movie record + CSV parsing and string cleaning helpers
├── AVL.h               # AVL tree (insert, search, rotations, filtered traversals)
├── Hash.h              # Actor hash table + per-actor linked list of movies
├── Graph.h             # Movie graph: BFS/DFS recommendations, shortest path
└── movie_metadata.csv  # IMDb 5000 Movie Dataset
```

## ⏱️ Complexity

| Operation | Structure | Time |
|---|---|---|
| Search by title | AVL tree | O(log n) |
| Search by actor | Hash table | O(1) average |
| Insert movie | AVL + hash | O(log n) |
| Recommend / shortest path | Graph (adjacency list) | O(V + E) |
| Genre / year / rating filter | AVL traversal | O(n) |

## 🔧 Changes After Submission

These fixes were made after the original course submission:
- **Actor search:** names are now hashed in lowercase when they're inserted too, so case-insensitive search finds them.
- **Graph traversals:** the BFS, DFS and shortest-path arrays were capped at 1,000 movies. They're now allocated to fit the whole graph (all 5,043 movies) and freed after use.

## 📊 Dataset

[IMDb 5000 Movie Dataset](https://www.kaggle.com/datasets/carolzhangdc/imdb-5000-movie-dataset) on Kaggle. The program uses these fields: title, genres, director, the top 3 actors, IMDb score and release year.

## 👥 Team

- **Muhammad Huzaifa**: [GitHub](https://github.com/your-username)
- **Irtaza Shahid**: [GitHub](https://github.com/partner-username)
