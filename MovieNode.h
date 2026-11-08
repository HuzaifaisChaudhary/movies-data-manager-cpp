#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <cctype>

using namespace std;

inline void parseCSVLine(string line, string row[], int maxCols)
{
    int colIdx = 0;
    string cell;
    bool insideQuotes = false;

    for (int i = 0; i < line.length(); i++)
    {
        char c = line[i];
        if (colIdx >= maxCols) break;

        if (c == '"')
        {
            insideQuotes = !insideQuotes;
        }
        else if (c == ',' && !insideQuotes)
        {
            row[colIdx++] = cell;
            cell = "";
        }
        else {
            cell += c;
        }
    }
    if (colIdx < maxCols)
        row[colIdx] = cell;
}

inline string cleaned(string str)
{
    string result = "";

    for (int i = 0; i < str.length(); i++)
    {
        // storing alphabets only
        unsigned char c = str[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            result += c;
        }
        // storing numbers only in string
        else if (c >= '0' && c <= '9')
        {
            result += c;
        }
        // storing symbols also in string
        else if (c == ' ' || c == '.' || c == ',' || c == ':' ||
            c == '-' || c == '\'' || c == '!' || c == '?' ||
            c == '(' || c == ')' || c == '&')
        {
            result += c;
        }
    }

    int start = 0;
    int end = result.length() - 1;

    while (start <= end && result[start] == ' ')
        start++;

    while (end >= start && result[end] == ' ')
        end--;

    if (start <= end)
        return result.substr(start, end - start + 1);
    else
        return "";
}

struct MovieNode
{
    string title;
    string genre;
    string actors[3];
    string director;
    float rating;
    int year;

    MovieNode(string t, string g, string d, float r, int y, string a1, string a2, string a3)
        : title(t), genre(g), director(d), rating(r), year(y)
    {
        actors[0] = a1;
        actors[1] = a2;
        actors[2] = a3;
    }

    void display() const
    {
        cout << "Title: " << title << " (" << year << ")" << endl;
        cout << "Director: " << director << " | Genre: " << genre << endl;
        cout << "Rating: " << rating << endl;
        cout << "Actors: " << actors[0] << ", " << actors[1] << ", " << actors[2] << endl;
        cout << "----------------------------------------" << endl;
    }
};