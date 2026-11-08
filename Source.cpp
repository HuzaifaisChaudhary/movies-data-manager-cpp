#include "IMDB.h"


int main()
{
    IMDB system;
    int choice;
    bool dataLoaded = false;

    do
    {
        cout << "\n======= Movies Data Manager =======\n";
        cout << "1. Load Movies from CSV\n";
        cout << "2. Search Movie by Title\n";
        cout << "3. Search Actor by Name\n";
        cout << "4. Display All Movies (Sorted)\n";
        cout << "5. Display All Actors\n";
        cout << "6. Recommend Movies (BFS)\n";
        cout << "7. Recommend Movies (DFS)\n";
        cout << "8. Find Shortest Path Between Movies\n";
        cout << "9. Search Movies by Genre\n";
        cout << "10. Search Movies by Year\n";
        cout << "11. Search Movies by Rating Range\n";
        cout << "12. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        if (choice == 1)
        {
            system.parseData("movie_metadata.csv");
            dataLoaded = true;
        }
        else if (choice == 2)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string title;
            cout << "Enter movie title: ";
            getline(cin, title);
            system.findMovie(title);
        }
        else if (choice == 3)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string actorName;
            cout << "Enter actor name: ";
            getline(cin, actorName);
            system.findActor(actorName);
        }
        else if (choice == 4)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            system.displaySortedMovies();
        }
        else if (choice == 5)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            system.displayActors();
        }
        else if (choice == 6)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string title;
            cout << "Enter movie title for recommendations: ";
            getline(cin, title);
            system.recommendMovies(title, true); // BFS
        }
        else if (choice == 7)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string title;
            cout << "Enter movie title for recommendations: ";
            getline(cin, title);
            system.recommendMovies(title, false); // DFS
        }
        else if (choice == 8)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string title1, title2;
            cout << "Enter first movie title: ";
            getline(cin, title1);
            cout << "Enter second movie title: ";
            getline(cin, title2);
            system.findMoviePath(title1, title2);
        }
        else if (choice == 9)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            string genre;
            cout << "Enter genre: ";
            getline(cin, genre);
            system.searchGenre(genre);
        }
        else if (choice == 10)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            int year;
            cout << "Enter year: ";
            cin >> year;
            system.searchYear(year);
        }
        else if (choice == 11)
        {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)!\n";
                continue;
            }
            float minR, maxR;
            cout << "Enter min rating: ";
            cin >> minR;
            cout << "Enter max rating: ";
            cin >> maxR;
            system.searchRatingRange(minR, maxR);
        }
        else if (choice == 12)
        {
            cout << "Exiting...\n";
        }
        else
        {
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 12);

    return 0;
}