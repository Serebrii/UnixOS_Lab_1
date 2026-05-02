#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Bucket {
    string name;
    long long min_size;
    long long max_size;
    long long count;
};

int main() {
    vector<Bucket> buckets = {
        {"0 B", 0, 0, 0},
        {"1 B - 1 KB", 1, 1024, 0},
        {"1 KB - 10 KB", 1025, 10240, 0},
        {"10 KB - 100 KB", 10241, 102400, 0},
        {"100 KB - 1 MB", 102401, 1048576, 0},
        {"1 MB - 10 MB", 1048577, 10485760, 0},
        {"10 MB - 100 MB", 10485761, 104857600, 0},
        {"100 MB - 1 GB", 104857601, 1073741824, 0},
        {"> 1 GB", 1073741825, -1, 0}
    };

    ifstream file("result.txt");
    if (!file.is_open()) {
        cerr << "Помилка: не вдалося відкрити файл result.txt" << endl;
        return 1;
    }

    string line;
    long long total_files = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            long long size = stoll(line);
            total_files++;
            
            for (auto& b : buckets) {
                if (size >= b.min_size && (b.max_size == -1 || size <= b.max_size)) {
                    b.count++;
                    break;
                }
            }
        } catch (...) {
        }
    }
    file.close();

    cout << "Всього оброблено файлів: " << total_files << "\n\n";
    cout << left << setw(18) << "Діапазон" << setw(12) << "Кількість" << setw(12) << "Відсоток" << "Гістограма\n";
    cout << string(80, '-') << "\n";

    long long max_count = 0;
    for (const auto& b : buckets) {
        if (b.count > max_count) max_count = b.count;
    }

    for (const auto& b : buckets) {
        double percent = total_files > 0 ? (double)b.count / total_files * 100.0 : 0.0;
        int bar_length = max_count > 0 ? (b.count * 40) / max_count : 0;
        string bar(bar_length, '#');

        cout << left << setw(18) << b.name
             << setw(12) << b.count
             << fixed << setprecision(2) << percent << "%\t"
             << bar << "\n";
    }

    return 0;
}
