#pragma once

#include <ncurses.h>

#include <array>
#include <cstdint>
#include <map>
#include <set>
#include <string>

#include "Route.h"

class UI {
   public:
    UI();
    ~UI();

    template <typename V, size_t N>
    V select(const std::array<std::pair<V, const char *>, N> &options,
             V exitValue);

    std::string readString(std::string message);
    uint32_t readNumber(std::string message);
    std::vector<std::string> readList(std::string message);

    void printMessage(std::string message);
};

template <typename V, size_t N>
V UI::select(const std::array<std::pair<V, const char *>, N> &options,
             V exitValue) {
    noecho();
    curs_set(0);

    size_t selected = 0;
    for (;;) {
        clear();

        size_t i = 0;
        for (const auto &[value, label] : options) {
            if (selected == i) {
                attron(A_REVERSE);
            }

            printw("%li. %s\n", i + 1, label);

            if (selected == i) {
                attroff(A_REVERSE);
            }

            i++;
        }

        if (selected == options.size()) {
            attron(A_REVERSE);
        }

        printw("%s\n", "Выйти");

        if (selected == options.size()) {
            attroff(A_REVERSE);
        }

        switch (getch()) {
            case KEY_UP:
                selected -= selected > 0;
                break;
            case KEY_DOWN:
                selected += selected < options.size();
                break;
            case '\n':
                return selected == options.size() ? exitValue
                                                  : options[selected].first;
                break;
        }
    }
}