#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#define ARDUINOJSON_ENABLE_STD_STRING 1
#include "ArduinoJson.h"

using namespace std;

struct Score {
    string nick;
    int score;
};

void gra(vector<Score>& scores) {
    cout << "----- Nowa gra -----" << endl;
    string nick;
    cout << "Wprowadz swoj nick: ";
    cin >> nick;
    int number = rand() % 100;
    cout << "Wylosowano liczbe: " << number << endl;
    scores.push_back({ nick, number });
    sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
        return a.score > b.score;
        });
    if (scores.size() > 10) {
        scores.resize(10);
    }
    cout << "Gra zakonczona. Twoj wynik zosta³ dodany do tabeli." << endl;
    cout << "--------------------" << endl;
}

void za³aduj(vector<Score>& scores) {
    ifstream file("scores.json");
    if (file.is_open()) {
        string jsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, jsonData);
        if (error) {
            cout << "B³¹d podczas deserializacji pliku JSON: " << error.c_str() << endl;
            return;
        }

        scores.clear();
        JsonArray root = doc.as<JsonArray>();
        for (const auto& score : root) {
            string nick = score["nick"].as<string>();
            int scoreValue = score["score"].as<int>();
            scores.push_back({ nick, scoreValue });
        }
    }
}


void Tabela(const vector<Score>& scores) {
    cout << "&&&Tabela wynikow&&&" << endl;
    for (const auto& score : scores) {
        cout << score.nick << ": " << score.score << endl;
    }
    cout << "--------------------------" << endl;
}

void zapis(const vector<Score>& scores) {
    DynamicJsonDocument doc(1024);
    JsonArray root = doc.to<JsonArray>();

    for (const auto& score : scores) {
        JsonObject scoreObj = root.createNestedObject();
        scoreObj["nick"] = score.nick;
        scoreObj["score"] = score.score;
    }

    ofstream file("scores.json");
    if (file.is_open()) {
        serializeJson(root, file);
        file.close();
    }
}

void clean(std::vector<Score>& scores) {
    scores.clear();
    std::cout << "Wyniki zostaly wyczyszczone." << std::endl;
}

void opcje()
{
    cout << "----- Opcje -----" << endl;
    cout << "1. Ustawienia graficzne" << endl;
    cout << "2. Ustawienia dzwieku" << endl;
    cout << "3. Ustawienia sterowania" << endl;
    cout << "-----------------" << endl;
    int a;
    cin >> a;
    switch (a)
    {
    case 1:
        cout << "zmieniono grafike" << endl;
        break;
    case 2:
        cout << "zmieniono dzwiek" << endl;
        break;
    case 3:
        cout << "zmieniono sterowanie" << endl;
        break;
    default:
        cout << "Zly wybor" << endl;
        break;
    }
}

int main() {
    vector<Score> scores;
    za³aduj(scores);

    while (true) {
        cout << " &&&Menu&&& " << endl;
        cout << "Wybierz opcje: " << endl;
        cout << "1. Nowa gra" << endl;
        cout << "2. Tabela wynikow" << endl;
        cout << "3. Wyczysc tabela wynikow" << endl;
        cout << "4. opcje" << endl;
        cout << "5. Wyjscie" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            gra(scores);
            break;
        case 2:
            Tabela(scores);
            break;
        case 3:
            clean(scores);
            break;
        case 4:
            opcje();
            break;
        case 5:
            zapis(scores);
            cout << "Do zobaczenia!" << endl;
            return 0;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
        }
        zapis(scores); // Dodana instrukcja zapisu przed kolejnym przejœciem pêtli
    }

    return 0;
}
