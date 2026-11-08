#pragma once
#include "MovieNode.h"

class MovieGraph {
private:
    struct Edge {
        MovieNode* movie;
        Edge* next;
        Edge(MovieNode* m) : movie(m), next(nullptr) {}
    };

    struct GraphNode {
        MovieNode* movie;
        Edge* edges;
        GraphNode* next;
        GraphNode(MovieNode* m) : movie(m), edges(nullptr), next(nullptr) {}
    };

    GraphNode* head;
    int vertexCount;   // number of movies in the graph, used to size traversal arrays

    GraphNode* findNode(MovieNode* movie) {
        GraphNode* temp = head;
        while (temp != nullptr) {
            if (temp->movie == movie) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    bool hasEdge(GraphNode* node, MovieNode* target) {
        Edge* temp = node->edges;
        while (temp != nullptr) {
            if (temp->movie == target) return true;
            temp = temp->next;
        }
        return false;
    }

    bool shareActors(MovieNode* m1, MovieNode* m2) {
        for (int i = 0; i < 3; i++) {
            if (m1->actors[i].empty()) continue;
            for (int j = 0; j < 3; j++) {
                if (m1->actors[i] == m2->actors[j]) return true;
            }
        }
        return false;
    }

    bool shareGenre(MovieNode* m1, MovieNode* m2) {
        return m1->genre == m2->genre && !m1->genre.empty();
    }

public:
    MovieGraph() : head(nullptr), vertexCount(0) {}

    void addVertex(MovieNode* movie) {
        if (findNode(movie) == nullptr) {
            GraphNode* newNode = new GraphNode(movie);
            newNode->next = head;
            head = newNode;
            vertexCount++;
        }
    }

    void addEdge(MovieNode* m1, MovieNode* m2) {
        GraphNode* node1 = findNode(m1);
        GraphNode* node2 = findNode(m2);

        if (node1 == nullptr || node2 == nullptr || m1 == m2) return;

        if (!hasEdge(node1, m2)) {
            Edge* edge1 = new Edge(m2);
            edge1->next = node1->edges;
            node1->edges = edge1;
        }

        if (!hasEdge(node2, m1)) {
            Edge* edge2 = new Edge(m1);
            edge2->next = node2->edges;
            node2->edges = edge2;
        }
    }

    void buildConnectionsForMovie(MovieNode* movie) {
        GraphNode* movieNode = findNode(movie);
        if (movieNode == nullptr) return;

        GraphNode* temp = head;
        while (temp != nullptr) {
            if (temp->movie != movie) {
                if (shareActors(movie, temp->movie) || shareGenre(movie, temp->movie)) {
                    addEdge(movie, temp->movie);
                }
            }
            temp = temp->next;
        }
    }

    void recommendMoviesBFS(MovieNode* startMovie, int maxRecommendations = 5) {
        GraphNode* startNode = findNode(startMovie);
        if (startNode == nullptr) {
            cout << "Movie not found in graph.\n";
            return;
        }

        // Arrays are sized to the whole graph so every movie can be visited
        const int MAX_QUEUE = vertexCount + 1;
        MovieNode** queue = new MovieNode*[MAX_QUEUE];
        bool* visited = new bool[MAX_QUEUE];
        MovieNode** allMovies = new MovieNode*[MAX_QUEUE];
        int movieIndex = 0;

        GraphNode* temp = head;
        while (temp != nullptr && movieIndex < MAX_QUEUE) {
            allMovies[movieIndex] = temp->movie;
            visited[movieIndex] = false;
            movieIndex++;
            temp = temp->next;
        }

        int front = 0, rear = 0;
        queue[rear++] = startMovie;

        for (int i = 0; i < movieIndex; i++) {
            if (allMovies[i] == startMovie) {
                visited[i] = true;
                break;
            }
        }

        cout << "\n=== Recommended Movies (BFS) ===\n";
        int count = 0;

        while (front < rear && count < maxRecommendations) {
            MovieNode* current = queue[front++];

            GraphNode* currentNode = findNode(current);
            if (currentNode == nullptr) continue;

            Edge* edge = currentNode->edges;
            while (edge != nullptr && count < maxRecommendations) {
                int idx = -1;
                for (int i = 0; i < movieIndex; i++) {
                    if (allMovies[i] == edge->movie) {
                        idx = i;
                        break;
                    }
                }

                if (idx != -1 && !visited[idx]) {
                    visited[idx] = true;
                    queue[rear++] = edge->movie;

                    if (edge->movie != startMovie) {
                        cout << ++count << ". " << edge->movie->title
                            << " (" << edge->movie->year << ") - Rating: "
                            << edge->movie->rating << endl;
                    }
                }
                edge = edge->next;
            }
        }

        if (count == 0) {
            cout << "No recommendations found.\n";
        }

        delete[] queue;
        delete[] visited;
        delete[] allMovies;
    }

    void recommendMoviesDFS(MovieNode* startMovie, int maxRecommendations = 5) {
        GraphNode* startNode = findNode(startMovie);
        if (startNode == nullptr) {
            cout << "Movie not found in graph.\n";
            return;
        }

        // Arrays are sized to the whole graph; the stack gets extra room because
        // a movie can be pushed more than once before it is visited
        const int MAX_STACK = 2 * vertexCount + 1;
        MovieNode** stack = new MovieNode*[MAX_STACK];
        bool* visited = new bool[MAX_STACK];
        MovieNode** allMovies = new MovieNode*[MAX_STACK];
        int movieIndex = 0;

        GraphNode* temp = head;
        while (temp != nullptr && movieIndex < MAX_STACK) {
            allMovies[movieIndex] = temp->movie;
            visited[movieIndex] = false;
            movieIndex++;
            temp = temp->next;
        }

        int top = -1;
        stack[++top] = startMovie;

        cout << "\n=== Recommended Movies (DFS) ===\n";
        int count = 0;

        while (top >= 0 && count < maxRecommendations) {
            MovieNode* current = stack[top--];

            int idx = -1;
            for (int i = 0; i < movieIndex; i++) {
                if (allMovies[i] == current) {
                    idx = i;
                    break;
                }
            }

            if (idx != -1 && !visited[idx]) {
                visited[idx] = true;

                if (current != startMovie) {
                    cout << ++count << ". " << current->title
                        << " (" << current->year << ") - Rating: "
                        << current->rating << endl;
                }

                GraphNode* currentNode = findNode(current);
                if (currentNode != nullptr) {
                    Edge* edge = currentNode->edges;
                    while (edge != nullptr) {
                        int edgeIdx = -1;
                        for (int i = 0; i < movieIndex; i++) {
                            if (allMovies[i] == edge->movie) {
                                edgeIdx = i;
                                break;
                            }
                        }

                        if (edgeIdx != -1 && !visited[edgeIdx] && top < MAX_STACK - 1) {
                            stack[++top] = edge->movie;
                        }
                        edge = edge->next;
                    }
                }
            }
        }

        if (count == 0) {
            cout << "No recommendations found.\n";
        }

        delete[] stack;
        delete[] visited;
        delete[] allMovies;
    }

    void findShortestPath(MovieNode* start, MovieNode* end) {
        if (start == nullptr || end == nullptr) {
            cout << "Invalid movies.\n";
            return;
        }

        if (start == end) {
            cout << "Same movie!\n";
            return;
        }

        // Arrays are sized to the whole graph so every movie can be reached
        const int MAX_SIZE = vertexCount + 1;
        MovieNode** queue = new MovieNode*[MAX_SIZE];
        MovieNode** parent = new MovieNode*[MAX_SIZE];
        bool* visited = new bool[MAX_SIZE];
        MovieNode** allMovies = new MovieNode*[MAX_SIZE];
        int movieIndex = 0;

        GraphNode* temp = head;
        while (temp != nullptr && movieIndex < MAX_SIZE) {
            allMovies[movieIndex] = temp->movie;
            visited[movieIndex] = false;
            parent[movieIndex] = nullptr;
            movieIndex++;
            temp = temp->next;
        }

        int front = 0, rear = 0;
        queue[rear++] = start;

        int startIdx = -1, endIdx = -1;
        for (int i = 0; i < movieIndex; i++) {
            if (allMovies[i] == start) startIdx = i;
            if (allMovies[i] == end) endIdx = i;
        }

        if (startIdx == -1 || endIdx == -1) {
            cout << "Movies not in graph.\n";
            delete[] queue; delete[] parent; delete[] visited; delete[] allMovies;
            return;
        }

        visited[startIdx] = true;
        bool found = false;

        while (front < rear && !found) {
            MovieNode* current = queue[front++];

            GraphNode* currentNode = findNode(current);
            if (currentNode == nullptr) continue;

            Edge* edge = currentNode->edges;
            while (edge != nullptr) {
                int neighborIdx = -1;
                for (int i = 0; i < movieIndex; i++) {
                    if (allMovies[i] == edge->movie) {
                        neighborIdx = i;
                        break;
                    }
                }

                if (neighborIdx != -1 && !visited[neighborIdx]) {
                    visited[neighborIdx] = true;
                    parent[neighborIdx] = current;
                    queue[rear++] = edge->movie;

                    if (edge->movie == end) {
                        found = true;
                        break;
                    }
                }
                edge = edge->next;
            }
        }

        if (!found) {
            cout << "\nNo path found between '" << start->title
                << "' and '" << end->title << "'.\n";
            delete[] queue; delete[] parent; delete[] visited; delete[] allMovies;
            return;
        }

        MovieNode** path = new MovieNode*[MAX_SIZE];
        int pathLen = 0;
        MovieNode* crawl = end;

        while (crawl != nullptr) {
            path[pathLen++] = crawl;
            int crawlIdx = -1;
            for (int i = 0; i < movieIndex; i++) {
                if (allMovies[i] == crawl) {
                    crawlIdx = i;
                    break;
                }
            }
            crawl = (crawlIdx != -1) ? parent[crawlIdx] : nullptr;
        }

        cout << "\n=== Shortest Path ===\n";
        cout << "Path length: " << pathLen - 1 << " connections\n\n";

        for (int i = pathLen - 1; i >= 0; i--) {
            cout << path[i]->title << " (" << path[i]->year << ")";
            if (i > 0) cout << "\n  |\n  v\n";
        }
        cout << "\n";

        delete[] path;
        delete[] queue; delete[] parent; delete[] visited; delete[] allMovies;
    }
};