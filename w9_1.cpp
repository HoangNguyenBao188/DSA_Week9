#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <filesystem>

// khai bao Node
struct Node {
    std::string name; // ten
    size_t size; // kich thuoc
    std::time_t modifiedTime; //chinh sua cuoi cung
    Node* next;

    // constructor cua Node de khoi tao gia tri cho cac bien
    Node(const std::string& fileName, size_t fileSize, std::time_t modTime)
        : name(fileName), size(fileSize), modifiedTime(modTime), next(nullptr) {}
};

//quan ly danh sach tep tin
class FileCollection {
private:
    Node* start;

public:
    FileCollection() : start(nullptr) {}

    // them mot tep tin moi vao danh sach theo thu tu thoi gian chinh sua
    void addFileToList(const std::string& fileName, size_t fileSize, std::time_t modTime) {
        Node* newNode = new Node(fileName, fileSize, modTime);

        // kiem tra neu danh sach rong hoac thoi gian chinh sua cua tep moi nho hon node dau
        if (!start || difftime(start->modifiedTime, modTime) > 0) {
            newNode->next = start;
            start = newNode;
        } else {
            // tim vi tri phu hop de chen node moi
            Node* current = start;
            while (current->next && difftime(current->next->modifiedTime, modTime) < 0) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    // tinh tong kich thuoc cac tep trong danh sach
    size_t totalFileSize() const {
        size_t total = 0;
        Node* current = start;
        while (current) {
            total += current->size; // cong kich thuoc cua moi tep vao bien total
            current = current->next;
        }
        return total;
    }

    // gioi han tong kich thuoc cac tep trong danh sach bang cach xoa cac tep nho nhat
    void limitTotalSize(size_t maxSize) {
        while (totalFileSize() > maxSize) {
            Node* smallest = start; // khoi tao node nho nhat
            Node* prevSmallest = nullptr;
            Node* current = start;
            Node* previous = nullptr;

            // tim tep tin co kich thuoc nho nhat
            while (current) {
                if (current->size < smallest->size) {
                    smallest = current;
                    prevSmallest = previous;
                }
                previous = current;
                current = current->next;
            }

            // xoa tep tin nho nhat khoi danh sach
            if (prevSmallest) {
                prevSmallest->next = smallest->next;
            } else {
                start = smallest->next;
            }
            delete smallest;
        }
    }

    // hien thi thong tin ve cac tep trong danh sach
    void showFiles() const {
        Node* current = start;
        while (current) {
            std::cout << "File: " << current->name
                      << ", Size: " << current->size << " bytes"
                      << ", Last Modified: " << std::ctime(&current->modifiedTime);
            current = current->next;
        }
    }
};

// ham copy va them tep tin vao danh sach
void copyAndAddToCollection(FileCollection& collection, const std::string& src, const std::string& dest, size_t fileSize, std::time_t modTime) {
    #ifdef _WIN32
        std::system(("copy " + src + " " + dest).c_str()); // lenh copy tren windows
    #else
        std::system(("cp " + src + " " + dest).c_str()); // lenh copy tren linux
    #endif
    collection.addFileToList(dest, fileSize, modTime); // them tep tin vao danh sach
}

// ham main
int main() {
    FileCollection files;

    // them mot so tep tin vao danh sach
    files.addFileToList("file1.txt", 1000, std::time(nullptr) - 100);
    files.addFileToList("file2.txt", 2000, std::time(nullptr) - 200);
    files.addFileToList("file3.txt", 500, std::time(nullptr) - 50);

    std::cout << "Files in collection:\n";
    files.showFiles(); // hien thi cac tep trong danh sach

    // copy va them tep tin moi vao danh sach
    copyAndAddToCollection(files, "source_file.txt", "destination_file.txt", 1500, std::time(nullptr) - 150);

    std::cout << "\nFiles after copy:\n";
    files.showFiles(); // hien thi cac tep sau khi copy

    // gioi han tong kich thuoc cua danh sach toi da 32GB
    files.limitTotalSize(static_cast<size_t>(32) * 1024 * 1024 * 1024);

    std::cout << "\nFiles after limiting to 32GB:\n";
    files.showFiles(); // hien thi cac tep sau khi gioi han kich thuoc

    return 0;
}
