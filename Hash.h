#pragma once
#include "MovieNode.h"

class MovieNodeHash {
public:
    MovieNode* movie;
    MovieNodeHash* next;

    MovieNodeHash(MovieNode* m) : movie(m), next(nullptr) {}
};


class ActorNode {
    string actorName;
    MovieNodeHash* movies;

public:
    ActorNode* next;

    ActorNode(string name) : actorName(name), movies(nullptr), next(nullptr) {}

    string getActorName() { return actorName; }
    MovieNodeHash* getMovies() { return movies; }

    void addMovie(MovieNode* sharedMovie) {
        MovieNodeHash* newNode = new MovieNodeHash(sharedMovie);
        newNode->next = movies;
        movies = newNode;
    }
};


class ActorHashTable {
    int size;
    ActorNode** table;

public:
    ActorHashTable(int s) : size(s) {
        table = new ActorNode * [size];
        for (int i = 0; i < size; i++) table[i] = nullptr;
    }

    int hashFunction(string str) {
        unsigned long hash = 5381;
        for (char c : str) hash = ((hash << 5) + hash) + c;
        return hash % size;
    }

    ActorNode* getOrCreateActor(string name) {
        // Hash the lowercase name so searchActor() (which also lowercases) lands in the same bucket
        string nameLower = toLower(name);
        int index = hashFunction(nameLower);
        ActorNode* temp = table[index];
        while (temp != nullptr) {
            if (toLower(temp->getActorName()) == nameLower) return temp;
            temp = temp->next;
        }
        ActorNode* newNode = new ActorNode(name);
        newNode->next = table[index];
        table[index] = newNode;
        return newNode;
    }

    void insertMovieReference(MovieNode* sharedMovie) {
        for (int i = 0; i < 3; i++) {
            string aName = sharedMovie->actors[i];
            if (aName.empty() || aName == "Unknown") continue;

            ActorNode* actor = getOrCreateActor(aName);
            actor->addMovie(sharedMovie);
        }
    }

    string toLower(string str) {
        for (int i = 0; i < str.length(); i++)
            str[i] = tolower(str[i]);
        return str;
    }

    ActorNode* searchActor(string name) {
        string searchLower = toLower(name);
        int index = hashFunction(searchLower);
        ActorNode* temp = table[index];

        while (temp != nullptr) {
            if (toLower(temp->getActorName()) == searchLower) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void displayAll() {
        for (int i = 0; i < size; i++) {
            if (table[i] != nullptr) {
                ActorNode* current = table[i];
                while (current != nullptr) {
                    cout << "Actor: " << current->getActorName() << endl;
                    MovieNodeHash* m = current->getMovies();
                    while (m != nullptr) {
                        cout << "  -> " << m->movie->title << " (" << m->movie->year << ")\n";
                        m = m->next;
                    }
                    current = current->next;
                }
            }
        }
    }
};