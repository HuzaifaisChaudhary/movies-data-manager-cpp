#pragma once
#include "AVL.h"
#include "Hash.h"
#include "Graph.h"


class IMDB {
    MovieAVL* avlTree;
    ActorHashTable* hashTable;
    MovieGraph* graph;

public:
    IMDB() {
        avlTree = new MovieAVL();
        hashTable = new ActorHashTable(1000);
        graph = new MovieGraph();
    }

    void insertMovie(string t, string g, string d, float r, int y, string a1, string a2, string a3) {
        MovieNode* newMovie = new MovieNode(t, g, d, r, y, a1, a2, a3);
        avlTree->insert(newMovie);
        hashTable->insertMovieReference(newMovie);
        graph->addVertex(newMovie);
    }

    void parseData(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file.\n";
            return;
        }
        string line;
        getline(file, line);

        cout << "Loading movies...\n";
        int count = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            const int MAX_COLS = 30;
            string row[MAX_COLS];
            parseCSVLine(line, row, MAX_COLS);

            string title = cleaned(row[11]);
            string genre = cleaned(row[9]);
            string director = cleaned(row[1]);
            string a1 = cleaned(row[10]);
            string a2 = cleaned(row[6]);
            string a3 = cleaned(row[14]);

            int year = 0;
            if (!row[23].empty()) try { year = stoi(row[23]); }
            catch (...) {}

            float rating = 0.0f;
            if (!row[25].empty()) try { rating = stof(row[25]); }
            catch (...) {}

            insertMovie(title, genre, director, rating, year, a1, a2, a3);
            count++;
        }

        cout << "Loaded " << count << " movies successfully!\n";
    }

    void findMovie(string title) {
        MovieNode* m = avlTree->search(title);
        if (m != nullptr) {
            cout << "\n--- Movie Found ---\n";
            m->display();
        }
        else {
            cout << "\nMovie '" << title << "' not found.\n";
        }
    }

    void findActor(string name) {
        ActorNode* a = hashTable->searchActor(name);
        if (a != nullptr) {
            cout << "\n--- Actor Found: " << a->getActorName() << " ---\n";
            MovieNodeHash* m = a->getMovies();
            if (m == nullptr) cout << "No movies recorded.\n";
            while (m != nullptr) {
                cout << "  -> " << m->movie->title << " (" << m->movie->year << ")\n";
                m = m->next;
            }
            cout << "-----------------------\n";
        }
        else {
            cout << "\nActor '" << name << "' not found.\n";
        }
    }

    void searchGenre(string genre) {
        avlTree->searchByGenre(genre);
    }

    void searchYear(int year) {
        avlTree->searchByYear(year);
    }

    void searchRatingRange(float minRating, float maxRating) {
        avlTree->searchByRatingRange(minRating, maxRating);
    }

    void displaySortedMovies() {
        cout << "\n--- All Movies (Sorted by Title) ---\n";
        avlTree->displaySorted();
    }

    void displayActors() {
        cout << "\n--- All Actors ---\n";
        hashTable->displayAll();
    }

    void recommendMovies(string title, bool useBFS = true) {
        MovieNode* m = avlTree->search(title);
        if (m == nullptr) {
            cout << "\nMovie '" << title << "' not found.\n";
            return;
        }

        graph->buildConnectionsForMovie(m);

        if (useBFS) {
            graph->recommendMoviesBFS(m);
        }
        else {
            graph->recommendMoviesDFS(m);
        }
    }

    void findMoviePath(string title1, string title2) {
        MovieNode* m1 = avlTree->search(title1);
        MovieNode* m2 = avlTree->search(title2);

        if (m1 == nullptr) {
            cout << "\nMovie '" << title1 << "' not found.\n";
            return;
        }
        if (m2 == nullptr) {
            cout << "\nMovie '" << title2 << "' not found.\n";
            return;
        }

        graph->buildConnectionsForMovie(m1);
        graph->buildConnectionsForMovie(m2);

        graph->findShortestPath(m1, m2);
    }
};