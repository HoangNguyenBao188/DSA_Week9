#include <iostream>
#include <string>

using namespace std;

//node chua word va dem(count)
struct WordNode {
    string word;
    int count;
    WordNode* next;

    WordNode(const string& w) : word(w), count(1), next(nullptr) {}
};


class WordList {
private:
    WordNode* head;

public:
    WordList() : head(nullptr) {}

    //them tu vao list
    void addWord(const string& word) {
        WordNode* current = head;
        WordNode* previous = nullptr;

        while (current) {
            if (current->word == word) {
                current->count++;
                return;
            }
            previous = current;
            current = current->next;
        }

        WordNode* newNode = new WordNode(word);
        if (previous) {
            previous->next = newNode;
        } else {
            head = newNode;
        }
    }

    //in danh sach tu va dem
    void showList() const {
        WordNode* current = head;
        while (current) {
            cout << "Word: " << current->word << ", Count: " << current->count << endl;
            current = current->next;
        }
    }

    //tim tu co nhieu nhat
    string mostCommonWord() const {
        WordNode* current = head;
        string frequentWord;
        int highestCount = 0;

        while (current) {
            if (current->count > highestCount) {
                highestCount = current->count;
                frequentWord = current->word;
            }
            current = current->next;
        }
        return frequentWord;
    }

    //loai bo tu lay
    void removeRepeats() {
        WordNode* current = head;
        WordNode* previous = nullptr;

        while (current) {
            if (current->word.length() >= 4 &&
                current->word.substr(0, current->word.length() / 2) == current->word.substr(current->word.length() / 2)) {

                cout << "Removing reduplicative word: " << current->word << endl;

                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }

                delete current;
                current = previous ? previous->next : head;
            } else {
                previous = current;
                current = current->next;
            }
        }
    }

    // dem so tu trong list
    int totalWords() const {
        int wordCount = 0;
        WordNode* current = head;
        while (current) {
            wordCount++;
            current = current->next;
        }
        return wordCount;
    }
};

int main() {
    WordList words;

    cout << "Nhap mot cau Tieng Viet: ";
    string sentence;
    getline(cin, sentence);

    //chia cau van thanh nhieu doan nho
    size_t pos = 0;
    string delimiter = " ";

    while ((pos = sentence.find(delimiter)) != string::npos) {
        string word = sentence.substr(0, pos);
        words.addWord(word);
        sentence.erase(0, pos + delimiter.length());
    }
    words.addWord(sentence); //them tu cuoi

    cout << "\nTu dau:\n";
    words.showList();

    string commonWord = words.mostCommonWord();
    cout << "Tu xuat hien nhieu nhat: " << commonWord << endl;

    words.removeRepeats();
    cout << "\nCau van sau khi loai bo  tu lay: \n";
    words.showList();

    int wordCount = words.totalWords();
    cout << "So tu: " << wordCount << endl;

    return 0;
}
