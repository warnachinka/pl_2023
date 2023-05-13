#include "UI.h"

#include "logger.h"

UI::UI() {
    initscr();
    noecho();
    cbreak();

    scrollok(stdscr, true);

    start_color();

    use_default_colors();

    init_pair(1, -1, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_CYAN, -1);
    keypad(stdscr, true);
}

UI::~UI() { endwin(); }

std::string UI::readString(std::string message) {
    echo();
    clear();
    curs_set(1);

    clear();

    printw("%s\n", message.c_str());

    std::array<char, 64> input;

    getnstr(input.data(), input.size());

    return input.data();
}

uint32_t UI::readNumber(std::string message) {
    return std::stoul(readString(message));
}

std::vector<std::string> UI::readList(std::string message) {
    std::string str = readString(message);

    std::vector<std::string> list;
    std::stringstream stream(str);
    std::string item;

    while (std::getline(stream, item, ' ')) {
        list.push_back(item);
    }

    return list;
}

void UI::printMessage(std::string message) {
    noecho();
    clear();
    curs_set(0);

    addstr(message.c_str());
    addstr("\n\nPress any key to continue...");

    getch();
}
