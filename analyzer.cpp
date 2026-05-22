#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <cstdlib>

using namespace std;

// Функція для перевірки існування файлу
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

int main() {
    string filename = "result.txt";
    vector<long long> sizes;

    // 1. Перевірка наявності файлу та автоматичний збір даних
    if (!fileExists(filename)) {
        cout << "File " << filename << " not found.\n";
        cout << "Initialized automatic data collection for the file system...\n";

        // Виконання команди Linux безпосередньо з C++ програми
        int status = system("find / -type f -ls 2>/dev/null | awk '{print $7}' > result.txt");

        if (status != 0) {
            cerr << "Data collection failed with error code: " << status << "\n";
            return 1;
        }
        cout << "Data collection completed successfully.\n\n";
    }
    else {
        cout << "File " << filename << " found. Using existing data...\n\n";
    }

    // 2. Зчитування даних з файлу
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << "\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        try {
            long long size = stoll(line);
            sizes.push_back(size);
        }
        catch (...) {
            // Ігнорування пошкоджених рядків
        }
    }
    file.close();

    long long total_files = sizes.size();
    if (total_files == 0) {
        cout << "File is empty or does not contain valid numeric data.\n";
        return 1;
    }

    // 3. Аналіз та підготовка до візуалізації
    sort(sizes.begin(), sizes.end());
    long long max_size = sizes.back();
    if (max_size < 1) max_size = 1;

    // --- ТАБЛИЦЯ 1: Логарифмічна шкала (50 точок) ---
    cout << "========================================================\n";
    cout << " TABLE 1: LOGARITHMIC SCALE (ENTIRE FILE SYSTEM)\n";
    cout << "========================================================\n";
    cout << "Size(X)\tPercentage(Y)\n";

    double log_min = 0.0;
    double log_max = log10((double)max_size);
    double step = log_max / 49.0;

    for (int i = 0; i < 50; ++i) {
        long long bound = (i == 0) ? 0 : (long long)pow(10, log_min + i * step);
        if (i == 49) bound = max_size;

        auto it = upper_bound(sizes.begin(), sizes.end(), bound);
        long long count = distance(sizes.begin(), it);
        double cdf = (double)count / total_files * 100.0;

        cout << bound << "\t" << fixed << setprecision(2) << cdf << "\n";
    }

    // --- ТАБЛИЦЯ 2: Лінійна шкала мікрорівня (до 100 КБ, 50 точок) ---
    cout << "\n========================================================\n";
    cout << " TABLE 2: LINEAR SCALE UP TO 100 KB\n";
    cout << "========================================================\n";
    cout << "Size(X)\tPercentage(Y)\n";

    long long linear_max = 102400;
    long long linear_step = linear_max / 50;

    for (int i = 1; i <= 50; ++i) {
        long long bound = i * linear_step;

        auto it = upper_bound(sizes.begin(), sizes.end(), bound);
        long long count = distance(sizes.begin(), it);
        double cdf = (double)count / total_files * 100.0;

        cout << bound << "\t" << fixed << setprecision(2) << cdf << "\n";
    }

    return 0;
}
