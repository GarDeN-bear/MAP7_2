#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <Windows.h>


using namespace std::chrono_literals;
std::mutex m;
void setCursor(int x, int y);
void progressBar(const std::chrono::milliseconds timeCalculation);
void calculation(const int numberThread, const std::chrono::milliseconds timeCalculation);

int main() {
	std::chrono::milliseconds timeCalculation = 5000ms;
	int countThreads = 4;
	std::vector<std::thread> vectorThreads;
	for (int i = 0; i < countThreads; ++i) {
		vectorThreads.push_back(std::thread(calculation, i + 1, timeCalculation));
	}
	for (int i = 0; i < countThreads; ++i) {
		vectorThreads.at(i).join();
	}
	return 0;
}


void setCursor(int x, int y)
{
	COORD coordinates = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

// ��������, ���� ���������������� ��� ������� �� �����. ������������ �������� �����������, ��, ������ (���-�� � ���������� ������� � ������� ����������� ��������), ������� ���������� ������ �������
//void progressBar(const std::chrono::milliseconds timeCalculation) 
//{
//	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
//	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
//	char fillingChar = '|';
//	char leftBoundary = '[';
//	char rightBoundary = ']';
//	std::cout << leftBoundary;
//	int x = consoleInfo.dwCursorPosition.X;
//	int y = consoleInfo.dwCursorPosition.Y;
//	setCursor(x + 101, y);
//	std::cout << rightBoundary;
//	setCursor(x + 1, y);
//	std::chrono::milliseconds percent = timeCalculation / 100;
//	bool flag = true;
//	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
//	auto previousDuration = 0ms;
//	int cursorPosition = x + 1;
//	int count = 1;
//	while (true) {
//		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
//		auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (currentTime - start); // ��������� ����� ���������� � �������������
//		if ((duration.count() % percent.count()) == 0 && previousDuration != duration) {
//			setCursor(cursorPosition, y);
//			std::cout << fillingChar;
//			previousDuration = duration;
//			setCursor(x + 105, y);
//			std::cout << static_cast<double>(duration.count()) / timeCalculation.count() * 100 << "%";
//			count++;
//			cursorPosition = x + count;
//		}
//		else if (abs(duration.count() - timeCalculation.count()) <= 0.1) {
//			break;
//		}
//	}
//	std::cout << std::endl;
//}

void progressBar(const std::chrono::milliseconds timeCalculation) 
{
	char fillingChar = '|';
	char leftBoundary = '[';
	char rightBoundary = ']';
	int width = 50; // ������ ��������-���� � ��������
	int current = 0; // ������� ������� ��������-����
	int total = 100; // ����� ���������� �����
	int step = total / width; // ���������� ����� �� ���� ��� ��������-����
	int progress = 0; // ������� ���������� ����������� �����
	while (progress <= total) {
		// ���������� �������� ����������
        int percent = (progress * 100) / total;

        // ���������� ���������� �������� ��� ���������� ��������-����
        int fill = (percent * width) / 100;

        // ����� ��������-����
        std::cout << "\r" <<leftBoundary;
        for (int i = 0; i < fill; i++) {
            std::cout << fillingChar;
        }
        for (int i = fill; i < width; i++) {
            std::cout << " ";
        }
        std::cout << rightBoundary << " " << percent << "%";

        // ���������� ������� ������� ��������-����
        current += step;
        progress += step;

        // �������� �� �����, ����������� ��� ���������� ��������-����
        std::this_thread::sleep_for(timeCalculation / (28.5*step));

	}
	std::cout << std::endl;
}

void calculation(const int numberThread, const std::chrono::milliseconds timeCalculation)
{
	std::lock_guard<std::mutex> lm(m);
	std::cout << "Thread's number: " << numberThread << "\n";
	auto start = std::chrono::steady_clock::now();
	std::cout << "Thread's id: " << std::this_thread::get_id() << '\n';
	std::cout << "Calculation progress: \n";
	progressBar(timeCalculation);
	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Total time thread's work: " << duration.count() << "ms\n";
}