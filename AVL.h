#pragma once
#include "MovieNode.h"

// ==========================================
// AVL Tree Node (Wraps Pointer)
// ==========================================
class MovieNodeAVL {
public:
    MovieNode* movie;
    MovieNodeAVL* left;
    MovieNodeAVL* right;
    int height;

    MovieNodeAVL(MovieNode* m) : movie(m), left(nullptr), right(nullptr), height(1) {}
};

class MovieAVL {
    MovieNodeAVL* root;

    int getHeight(MovieNodeAVL* n) { return (n == nullptr) ? 0 : n->height; }
    int maxVal(int a, int b) { return (a > b) ? a : b; }
    int getBalance(MovieNodeAVL* n) { return (n == nullptr) ? 0 : getHeight(n->left) - getHeight(n->right); }

    MovieNodeAVL* rightRotate(MovieNodeAVL* y) {
        MovieNodeAVL* x = y->left;
        MovieNodeAVL* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    MovieNodeAVL* leftRotate(MovieNodeAVL* x) {
        MovieNodeAVL* y = x->right;
        MovieNodeAVL* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }

    MovieNodeAVL* insertHelper(MovieNodeAVL* node, MovieNode* data) {
        if (node == nullptr) return new MovieNodeAVL(data);

        if (data->title < node->movie->title)
            node->left = insertHelper(node->left, data);
        else if (data->title > node->movie->title)
            node->right = insertHelper(node->right, data);
        else
            return node;

        node->height = 1 + maxVal(getHeight(node->left), getHeight(node->right));
        int balance = getBalance(node);

        if (balance > 1 && data->title < node->left->movie->title)
            return rightRotate(node);
        if (balance < -1 && data->title > node->right->movie->title)
            return leftRotate(node);
        if (balance > 1 && data->title > node->left->movie->title) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && data->title < node->right->movie->title) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void inorderHelper(MovieNodeAVL* root) {
        if (root != nullptr) {
            inorderHelper(root->left);
            root->movie->display();
            inorderHelper(root->right);
        }
    }

    // Search by genre helper
    void searchByGenreHelper(MovieNodeAVL* node, string genre, int& count) {
        if (node == nullptr) return;

        searchByGenreHelper(node->left, genre, count);

        if (toLower(node->movie->genre).find(toLower(genre)) != string::npos) {
            cout << ++count << ". ";
            node->movie->display();
        }

        searchByGenreHelper(node->right, genre, count);
    }

    // Search by year helper
    void searchByYearHelper(MovieNodeAVL* node, int year, int& count) {
        if (node == nullptr) return;

        searchByYearHelper(node->left, year, count);

        if (node->movie->year == year) {
            cout << ++count << ". ";
            node->movie->display();
        }

        searchByYearHelper(node->right, year, count);
    }

    // Search by rating range helper
    void searchByRatingHelper(MovieNodeAVL* node, float minRating, float maxRating, int& count) {
        if (node == nullptr) return;

        searchByRatingHelper(node->left, minRating, maxRating, count);

        if (node->movie->rating >= minRating && node->movie->rating <= maxRating) {
            cout << ++count << ". ";
            node->movie->display();
        }

        searchByRatingHelper(node->right, minRating, maxRating, count);
    }

public:
    MovieAVL() : root(nullptr) {}

    void insert(MovieNode* movieData) {
        root = insertHelper(root, movieData);
    }

    void displaySorted() {
        inorderHelper(root);
    }

    string toLower(string str) {
        for (int i = 0; i < str.length(); i++)
            str[i] = tolower(str[i]);
        return str;
    }

    MovieNode* search(string title) {
        string searchLower = toLower(title);
        MovieNodeAVL* temp = root;

        while (temp != nullptr) {
            string movieTitleLower = toLower(temp->movie->title);

            if (searchLower == movieTitleLower) {
                return temp->movie;
            }
            else if (searchLower < movieTitleLower) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return nullptr;
    }

    // Public search methods
    void searchByGenre(string genre) {
        cout << "\n=== Movies in Genre: " << genre << " ===\n";
        int count = 0;
        searchByGenreHelper(root, genre, count);
        if (count == 0) {
            cout << "No movies found in this genre.\n";
        }
        else {
            cout << "\nTotal: " << count << " movie(s) found.\n";
        }
    }

    void searchByYear(int year) {
        cout << "\n=== Movies from Year: " << year << " ===\n";
        int count = 0;
        searchByYearHelper(root, year, count);
        if (count == 0) {
            cout << "No movies found from this year.\n";
        }
        else {
            cout << "\nTotal: " << count << " movie(s) found.\n";
        }
    }

    void searchByRatingRange(float minRating, float maxRating) {
        cout << "\n=== Movies with Rating " << minRating << " - " << maxRating << " ===\n";
        int count = 0;
        searchByRatingHelper(root, minRating, maxRating, count);
        if (count == 0) {
            cout << "No movies found in this rating range.\n";
        }
        else {
            cout << "\nTotal: " << count << " movie(s) found.\n";
        }
    }
};