#include <windows.h>
#include <iostream>

int main() {
    // Створення каналу для читання з стандартного вводу
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdIn == INVALID_HANDLE_VALUE) {
        std::cerr << "Не вдалося отримати дескриптор стандартного вводу!" << std::endl;
        return 1;
    }

    char buffer[128];
    DWORD bytesRead;

    // Читання з каналу і виведення на екран
    while (ReadFile(hStdIn, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Завершення рядка
        std::cout << "Дочірній процес отримав: " << buffer << std::endl;
    }

    return 0;
}
