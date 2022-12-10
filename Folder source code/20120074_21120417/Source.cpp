#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Số file đã được chia nhỏ từ file gốc
#define NUMBER_OF_FILES 10

string txt_local_name_file = "local_name_file.txt";
string title_row; // để lưu hành tiêu đề từ file csv đầu tiên

struct BookRating {
    string id;
    string book_other_information;
};

// Hàm ghi đường dẫn của file csv vào file txt
void write_local_name_file(string filename) {
    fstream outfile;
    outfile.open(filename, ios_base::out | ios_base::binary);

    for (int i = 1; i <= NUMBER_OF_FILES; i++) {
        outfile << "Books_rating/Books_rating_" << i << ".csv" << endl;
    }

    outfile.close();
}

// Hàm in id sách trong 1 vector đọc được từ 1 file csv
void print_list_book(vector<BookRating> list) {
    for (int i = 0; i < list.size(); i++) {
        cout << i + 1 << " " << list[i].id << endl;
    }
    cout << endl;
}

// Hàm in id sách trong vector 2 chiều chứa các vector 1 chiều đọc từ 1 file csv -> in id sách của tất cả các file csv gộp lại
void print_multi_list_book(vector<vector<BookRating>> list) {
    for (int i = 0; i < list.size(); i++)print_list_book(list[i]);
}

// Hàm đọc thông tin 1 book
void read_one_book(fstream& _file, BookRating& book) {
    getline(_file, book.id, ',');
    getline(_file, book.book_other_information);
}

// Hàm đọc danh sách book trong 1 file csv
void read_list_book(fstream& _file, vector<BookRating>& list) {
    while (!_file.eof()) {
        BookRating book;
        read_one_book(_file, book);
        list.push_back(book);
    }
}

// Hàm đọc danh sách book trong tất cả các file csv (đọc tuần tự từng file)
void read_multi_file(vector<vector<BookRating>>& list) {
    
    fstream local_name_file;
    local_name_file.open(txt_local_name_file, ios_base::in | ios_base::binary);

    if (!local_name_file.is_open()) {
        cout << "Can not open " << txt_local_name_file << endl;
        return;
    }
    else {
        cout << "Open " << txt_local_name_file << " successfully\n";
    }

    string path; // để lưu đường dẫn lấy từ file local_name_file.txt

    // Đọc file đầu tiên
    getline(local_name_file, path);

    fstream csv_file;
    csv_file.open(path, ios_base::in | ios_base::binary);
    if (!csv_file.is_open()) {
        cout << "Can not open " << path << "\n";
    }
    else {
        cout << "Open " << path << " successfully\n";
        cout << "Reading...\n";

        getline(csv_file, title_row); // đọc dòng tiêu đề của file csv đầu tiên vào title_row

        vector<BookRating> books;
        read_list_book(csv_file, books);
        list.push_back(books);

        cout << "Read successfully\n";

        csv_file.close();
    }

    // Đọc các file tiếp theo
    while (!local_name_file.eof()) {
        getline(local_name_file, path);

        if (path != "") {
            csv_file.open(path, ios_base::in | ios_base::binary);
            if (!csv_file.is_open()) {
                cout << "Can not open " << path << "\n";
            }
            else {
                cout << "Open " << path << " successfully\n";
                cout << "Reading...\n";

                vector<BookRating> books;
                read_list_book(csv_file, books);
                list.push_back(books);

                cout << "Read successfully\n";

                csv_file.close();
            }
        }
    }

    local_name_file.close();
}

// Hàm merge phục vụ thuật toán merge sort
void merge(vector<BookRating>& list, int left, int right, int mid) {
    int nLeft = mid - left + 1;
    int nRight = right - mid;

    vector<BookRating> Left;
    vector<BookRating> Right;

    for (int i = 0; i < nLeft; i++)
        Left.push_back(list[left + i]);
    for (int i = 0; i < nRight; i++)
        Right.push_back(list[mid + 1 + i]);

    int i = 0, j = 0, k = left;

    while (i < nLeft && j < nRight) {
        if (Left[i].id < Right[j].id) {
            list[k] = Left[i];
            i++;
        }
        else {
            list[k] = Right[j];
            j++;
        }
        k++;
    }

    while (i < nLeft) {
        list[k] = Left[i];
        i++;
        k++;
    }

    while (j < nRight) {
        list[k] = Right[j];
        j++;
        k++;
    }
}

// Hàm merge sort 1 vector
void merge_sort(vector<BookRating>& list, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(list, left, mid);
        merge_sort(list, mid + 1, right);
        merge(list, left, right, mid);
    }
}

// Hàm sort tất cả các vector (1 vector là 1 list book đọc từ 1 file csv)
void sort_multi_list(vector<vector<BookRating>>& list) {

    cout << "Sorting...\n";

    for (int i = 0; i < list.size(); i++)merge_sort(list[i], 0, list[i].size() - 1);

    cout << "Sort done\n";
}

// Hàm gộp 2 vector đã sắp xếp thành vector mới cũng đã được sắp xếp
vector<BookRating> two_pointer(vector<BookRating> list_1, vector<BookRating> list_2) {

    vector<BookRating> result;

    int i = 0;
    int j = 0;

    while (i < list_1.size() && j < list_2.size()) {
        if (list_1[i].id < list_2[j].id) {
            result.push_back(list_1[i]);
            i++;
        }
        else {
            result.push_back(list_2[j]);
            j++;
        }
    }

    while (i < list_1.size()) {
        result.push_back(list_1[i]);
        i++;
    }

    while (j < list_2.size()) {
        result.push_back(list_2[j]);
        j++;
    }

    return result;
}

// Hàm gộp các danh sách đã sắp xếp thành danh sách mới cũng đã được sắp xếp
vector<BookRating> merge_list(vector<vector<BookRating>>& list) {

    cout << "Merging...\n";

    vector<BookRating> result = list[0];

    for (int i = 1; i < list.size(); i++) {
        result = two_pointer(result, list[i]);
    }

    cout << "Merge done\n";

    return result;
}

// Hàm ghi kết quả vào file csv
void write_list(vector<BookRating> list) {

    cout << "Writing...\n";

    fstream csv_file_out;
    csv_file_out.open("sorted_books_rating.csv", ios_base::out | ios_base::binary);

    csv_file_out << title_row << endl; // ghi hàng tiêu đề

    for (int i = 0; i < list.size(); i++) {
        if (list[i].id == "") {
            continue;
        }
        csv_file_out << list[i].id + ',' + list[i].book_other_information + '\n';
    }

    cout << "Write successfully\n";

    csv_file_out.close();
}

int main() {
    write_local_name_file(txt_local_name_file);

    vector<vector<BookRating>> list_book;
    read_multi_file(list_book);
    sort_multi_list(list_book);

    vector<BookRating> result = merge_list(list_book);

    write_list(result);

    system("pause");
    return 0;
}