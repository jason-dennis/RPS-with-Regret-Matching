#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

namespace ui {

// ── Culori ANSI ────────────────────────────────────────────────
inline const std::string RESET   = "\033[0m";
inline const std::string BOLD    = "\033[1m";
inline const std::string DIM     = "\033[2m";
inline const std::string RED     = "\033[31m";
inline const std::string GREEN   = "\033[32m";
inline const std::string YELLOW  = "\033[33m";
inline const std::string BLUE    = "\033[34m";
inline const std::string CYAN    = "\033[36m";

inline const char* ACTION_NAME[3] = {"Rock", "Paper", "Scissors"};
inline const std::string ACTION_COLOR[3] = {RED, BLUE, YELLOW};

inline const int W = 56;  // latimea interioara a casetei

inline void enableAnsi() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

inline void clear() { std::cout << "\033[2J\033[H"; }

inline std::string repeat(const std::string& s, int n) {
    std::string r;
    for (int i = 0; i < n; ++i) r += s;
    return r;
}

inline std::string fmt(double x, int prec = 2) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(prec) << x;
    return os.str();
}

inline std::string pad(const std::string& s, int width) {
    return s + std::string(std::max(0, width - (int) s.size()), ' ');
}

// ── Chenar ─────────────────────────────────────────────────────
inline void boxTop()    { std::cout << CYAN << "╭" << repeat("─", W) << "╮" << RESET << "\n"; }
inline void boxBottom() { std::cout << CYAN << "╰" << repeat("─", W) << "╯" << RESET << "\n"; }
inline void boxSep()    { std::cout << CYAN << "├" << repeat("─", W) << "┤" << RESET << "\n"; }

// plain = textul vizibil (pentru aliniere), colored = acelasi text cu culori
inline void boxLine(const std::string& plain, const std::string& colored) {
    std::cout << CYAN << "│" << RESET << colored
              << std::string(std::max(0, W - (int) plain.size()), ' ')
              << CYAN << "│" << RESET << "\n";
}
inline void boxLine(const std::string& plain) { boxLine(plain, plain); }

// Bara de probabilitate, p intre 0 si 1
inline void renderBar(const std::string& label, double p, const std::string& color) {
    const int BAR = 26;
    p = std::clamp(p, 0.0, 1.0);
    int filled = (int) (p * BAR + 0.5);
    std::string plain = "  " + pad(label, 9) + std::string(BAR, '#') + "  " + fmt(p);
    std::string colored = "  " + pad(label, 9) + color + repeat("█", filled) + RESET +
                          DIM + repeat("░", BAR - filled) + RESET + "  " + BOLD + fmt(p) + RESET;
    boxLine(plain, colored);
}

// ── Sectiuni de ecran ──────────────────────────────────────────
inline void renderHeader() {
    boxTop();
    std::string title = "  ROCK  /  PAPER  /  SCISSORS";
    boxLine(title, BOLD + title + RESET);
    std::string sub = "  vs. regret-matching bot";
    boxLine(sub, DIM + sub + RESET);
    boxSep();
}

// Afiseaza runda curenta si scorul
inline void renderStatus(int round, int wins, int losses, int draws) {
    std::string left = "  Bot learns from your moves";
    std::string roundTxt = "Round " + std::to_string(round) + "  ";
    int gap = std::max(0, W - (int) left.size() - (int) roundTxt.size());
    boxLine(left + std::string(gap, ' ') + roundTxt,
            DIM + left + RESET + std::string(gap, ' ') + BOLD + roundTxt + RESET);

    std::string plain = "  You " + std::to_string(wins) + "   Bot " + std::to_string(losses) +
                        "   Draws " + std::to_string(draws);
    std::string colored = "  You " + GREEN + BOLD + std::to_string(wins) + RESET +
                          "   Bot " + RED + BOLD + std::to_string(losses) + RESET +
                          "   Draws " + YELLOW + BOLD + std::to_string(draws) + RESET;
    boxLine(plain, colored);
    boxSep();
}

// human/bot = actiunile din ultima runda; trimite human = -1 daca nu s-a jucat inca nimic
inline void renderLastRound(int human, int bot, int result) {
    if (human < 0 || bot < 0) {
        std::string plain = "  Make your first move!";
        boxLine(plain, DIM + plain + RESET);
        return;
    }
    std::string resTxt = result == 1 ? "YOU WIN" : (result == -1 ? "BOT WINS" : "DRAW");
    std::string resCol = result == 1 ? GREEN : (result == -1 ? RED : YELLOW);
    std::string plain = "  You " + std::string(ACTION_NAME[human]) + "  vs  Bot " +
                        ACTION_NAME[bot] + "  ->  " + resTxt;
    std::string colored = "  You " + ACTION_COLOR[human] + BOLD + ACTION_NAME[human] + RESET +
                          "  vs  Bot " + ACTION_COLOR[bot] + BOLD + ACTION_NAME[bot] + RESET +
                          "  ->  " + resCol + BOLD + resTxt + RESET;
    boxLine(plain, colored);
}

// results: rezultatele rundelor (1 / -1 / 0); se afiseaza ultimele 20
inline void renderHistory(const std::vector<int>& results) {
    std::string plain = "  Last: ", colored = "  Last: ";
    size_t start = results.size() > 20 ? results.size() - 20 : 0;
    for (size_t i = start; i < results.size(); ++i) {
        int r = results[i];
        char ch = r == 1 ? 'W' : (r == -1 ? 'L' : 'D');
        std::string col = r == 1 ? GREEN : (r == -1 ? RED : YELLOW);
        plain += std::string(1, ch) + " ";
        colored += col + BOLD + std::string(1, ch) + RESET + " ";
    }
    boxLine(plain, colored);
    boxSep();
}

// probs: 3 valori (Rock, Paper, Scissors); note: text optional sub bare
inline void renderStrategy(const std::string& title, const std::vector<double>& probs,
                           const std::string& note = "") {
    std::string t = "  " + title;
    boxLine(t, BOLD + t + RESET);
    for (int i = 0; i < 3; ++i) renderBar(ACTION_NAME[i], probs[i], ACTION_COLOR[i]);
    if (!note.empty()) {
        std::string n = "  " + note;
        boxLine(n, DIM + n + RESET);
    }
    boxSep();
}

inline void renderControls() {
    std::string c = "  [r] Rock   [p] Paper   [s] Scissors   [q] Quit";
    boxLine(c, DIM + c + RESET);
    boxBottom();
}

// message: text optional deasupra prompt-ului (ex. eroare de input)
inline void renderPrompt(const std::string& message = "") {
    if (!message.empty()) std::cout << "  " << YELLOW << message << RESET << "\n";
    std::cout << "  " << BOLD << "> " << RESET << std::flush;
}

}  // namespace ui