#include <iostream>
#include <list>
#include <fstream>
#include <string>
using namespace std;


bool isNum(char c) {
    return ('0' <= c && c <= '9');
}

int convertVal(char c) {
    int val = 0;
    if (isNum(c)) {
        val = c - '1';
    }
    else if (c == 'T') {
        val = 9;
    }
    else if (c == 'J') {
        val = 0;
    }
    else if (c == 'Q') {
        val = 10;
    }
    else if (c == 'K') {
        val = 11;
    }
    else if (c == 'A') {
        val = 12;
    }

    return val;
}


class Hand {
private:
    int cards[5];
    long bid;
    int firstRank;
    int jokers;

    void calcFirstRank() {
        firstRank = 0;
        int matches[13];

        for (int i = 0; i < 13; i++) {
            matches[i] = 0;
        }

        int pairsFound = 0;
        int tripleFound = 0;
        int fourFound = 0;

        if (jokers >= 4) {
            firstRank = 7;
        }

        if (firstRank == 0) {
            for (int card=0; card < 5; card++) {
                if (cards[card] != convertVal('J')) {
                    matches[cards[card]]++;
                    int currVal = matches[cards[card]];
                    if (currVal >= 5) {
                        firstRank = 7;
                    }
                    else if (currVal == 4) {
                        fourFound++;
                    }
                    else if (currVal == 3) {
                        tripleFound++;
                        pairsFound--;
                    }
                    else if (currVal == 2) {
                        pairsFound++;
                    }
                }
            }
        }

        if (firstRank == 0) {
            if (fourFound > 0) {
                if (jokers == 1) {
                    firstRank = 7;
                }
                else {
                    firstRank = 6;
                }
            }
            else if (tripleFound > 0) {
                if (jokers == 2)
                {
                    firstRank = 7;
                }
                else if (jokers == 1) {
                    firstRank = 6;
                }
                else if (pairsFound > 0) {
                    firstRank = 5;
                }
                else {
                    firstRank = 4;
                }
            }
            else if (pairsFound > 0) {
                if (pairsFound == 2) {
                    if (jokers == 1) {
                        firstRank = 5;
                    }
                    else {
                        firstRank = 3;
                    }
                }
                else { // pairsFound == 1
                    if (jokers == 3) {
                        firstRank = 7;
                    }
                    else if (jokers == 2) {
                        firstRank = 6;
                    }
                    else if (jokers == 1) {
                        firstRank = 4;
                    }
                    else {
                        firstRank = 2;
                    }
                }
            }
            else {
                if (jokers == 3) {
                    firstRank = 6;
                }
                else if (jokers == 2) {
                    firstRank = 4;
                }
                else if (jokers == 1) {
                    firstRank = 2;
                }
                else {
                    firstRank = 1;
                }
            }
        }
    }

public:
    Hand(string line) {
        jokers = 0;
        long bidVal = 0;
        for (int pos = 0; pos < line.length(); pos++) {
            if (pos < 5) {
                cards[pos] = convertVal(line[pos]);
                if (line[pos] == 'J') {
                    jokers++;
                }
            }
            else if (isNum(line[pos])) {
                bidVal = (bidVal * 10) + (line[pos] - '0');
            }
        }
        calcFirstRank();
        bid = bidVal;
    }

    int getFirstRank() {
        return firstRank;
    }

    int getCardVal(int pos) {
        if (0 <= pos && pos <= 4) {
            return cards[pos];
        }
        else {
            return 0;
        }
    }

    bool operator< (Hand &hand1) {
        bool isLess = false;

        if (firstRank < hand1.getFirstRank()) {
            isLess = true;
        }
        else if (firstRank == hand1.getFirstRank()) {
            bool foundDiff = false;
            for (int pos = 0; pos < 5 && !foundDiff; pos++) {
                if (cards[pos] < hand1.getCardVal(pos)) {
                    isLess = true;
                    foundDiff = true;
                }
                else if (cards[pos] > hand1.getCardVal(pos)) {
                    isLess = false;
                    foundDiff = true;
                }
            }
        }

        return isLess;
    }

    bool operator> (Hand &hand1) {
        bool isGreater = false;

        if (firstRank > hand1.getFirstRank()) {
            isGreater = true;
        }
        else if (firstRank == hand1.getFirstRank()) {
            bool foundDiff = false;
            for (int pos = 0; pos < 5 && !foundDiff; pos++) {
                if (cards[pos] > hand1.getCardVal(pos)) {
                    isGreater = true;
                    foundDiff = true;
                }
                else if (cards[pos] < hand1.getCardVal(pos)) {
                    isGreater = false;
                    foundDiff = true;
                }
            }
        }

        return isGreater;
    }

    bool operator== (Hand &hand1) {
        bool isEqual = false;

        if (firstRank == hand1.getFirstRank()) {
            bool foundDiff = false;
            for (int pos = 0; pos < 5 && !foundDiff; pos++) {
                if (cards[pos] > hand1.getCardVal(pos)) {
                    isEqual = false;
                    foundDiff = true;
                }
                else if (cards[pos] < hand1.getCardVal(pos)) {
                    isEqual = false;
                    foundDiff = true;
                }
            }
            isEqual = !foundDiff;
        }
        else {
            isEqual = false;
        }
        return isEqual;
    }

    long getBid() {
        return bid;
    }

    string getHand() {
        string val = "";
        for (int pos = 0; pos < 5; pos ++) {
            if (1 <= cards[pos] && cards[pos] <= 7) {
                val.push_back(cards[pos] + '1');
            }
            else if (cards[pos] == 9) {
                val.push_back('T');
            }
            else if (cards[pos] == 0) {
                val.push_back('J');
            }
            else if (cards[pos] == 10) {
                val.push_back('Q');
            }
            else if (cards[pos] == 11) {
                val.push_back('K');
            }
            else if (cards[pos] == 12) {
                val.push_back('A');
            }
        }
        return val;
    }
};


int main(int argc, char **argv) {
    ifstream input_file;
    input_file.open("input.txt");
    string line;

    list<Hand> game;

    while (getline(input_file, line)) {
        Hand hand(line);
        game.push_back(hand);
    }
    input_file.close();

    game.sort();

    long i = 1;
    long winnings = 0;

    for (list<Hand>::iterator it = game.begin(); it != game.end(); i++, it++) {
        winnings += i * it->getBid();
    }

    cout << "Winnings = " << winnings << '\n';

    return 0;
}
