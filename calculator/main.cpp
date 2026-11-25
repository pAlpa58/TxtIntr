#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kEpsilon = 1e-12;

void printUsage(const char* program) {
    std::cout << "Тригонометрический калькулятор (вариант 9)\n"
              << "Использование: " << program << " -o <tan|cot> -m <deg|rad> <угол>\n"
              << "        или:   " << program << " --operation <tan|cot> --mode <deg|rad> <угол>\n\n"
              << "Операции: tan — тангенс; cot — котангенс.\n"
              << "Режим deg задаёт угол в градусах, rad — в радианах.\n"
              << "Примеры:\n"
              << "  " << program << " -o tan -m deg 45\n"
              << "  " << program << " --operation cot --mode rad 0.7853981634\n";
}

bool parseNumber(const std::string& text, double& value) {
    try {
        std::size_t used = 0;
        value = std::stod(text, &used);
        return used == text.size() && std::isfinite(value);
    } catch (...) {
        return false;
    }
}
}

int main(int argc, char* argv[]) {
    if (argc == 1 || (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))) {
        printUsage(argv[0]);
        return 0;
    }

    std::string operation;
    std::string mode;
    std::string operand;
    bool haveOperation = false;
    bool haveMode = false;
    bool haveOperand = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-o" || arg == "--operation") {
            if (haveOperation || i + 1 >= argc) {
                std::cerr << "Ошибка: параметр операции задан некорректно.\n";
                printUsage(argv[0]);
                return 1;
            }
            operation = argv[++i];
            haveOperation = true;
        } else if (arg == "-m" || arg == "--mode") {
            if (haveMode || i + 1 >= argc) {
                std::cerr << "Ошибка: параметр режима задан некорректно.\n";
                printUsage(argv[0]);
                return 1;
            }
            mode = argv[++i];
            haveMode = true;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Ошибка: неизвестный параметр '" << arg << "'.\n";
            printUsage(argv[0]);
            return 1;
        } else if (!haveOperand) {
            operand = arg;
            haveOperand = true;
        } else {
            std::cerr << "Ошибка: для операции требуется ровно один угол.\n";
            return 1;
        }
    }

    if (!haveOperation || !haveMode || !haveOperand) {
        std::cerr << "Ошибка: необходимо задать операцию, режим и один угол.\n";
        printUsage(argv[0]);
        return 1;
    }
    if (operation != "tan" && operation != "cot") {
        std::cerr << "Ошибка: неизвестная операция '" << operation << "'. Допустимы tan и cot.\n";
        return 1;
    }
    if (mode != "deg" && mode != "rad") {
        std::cerr << "Ошибка: режим должен быть deg (градусы) или rad (радианы).\n";
        return 1;
    }

    double angle;
    if (!parseNumber(operand, angle)) {
        std::cerr << "Ошибка: угол должен быть конечным числом.\n";
        return 1;
    }
    if (mode == "deg") angle *= kPi / 180.0;

    const double sine = std::sin(angle);
    const double cosine = std::cos(angle);
    const double denominator = operation == "tan" ? cosine : sine;
    if (std::abs(denominator) < kEpsilon) {
        std::cerr << "Ошибка: значение не определено для этого угла (деление на ноль).\n";
        return 1;
    }

    const double result = operation == "tan" ? sine / cosine : cosine / sine;
    std::cout << std::setprecision(10) << "Результат: " << result << '\n';
    return 0;
}
