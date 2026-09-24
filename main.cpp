#include <iostream>
#include <vector>
#include <random>
#include "rps_ui.h"


void RegretMatching(int opponent_action, int player_action, std::vector<double>& strategy, std::vector<int>& regret) {
    std::vector<int> rewards(3);
    if (opponent_action == 0) { // Rock
        rewards = {0, 1, -1};
    }
    else if (opponent_action == 1) {
        rewards = {-1, 0, 1};
    }
    else {
        rewards = {1, -1, 0};
    }
    int totals = 0;
    for (int i{}; i < 3; ++i) {
        regret[i] += rewards[i] - rewards[player_action];
        totals += std::max(0, regret[i]);
    }

    if (totals == 0) {
        for (int i{}; i < 3; ++i) {
            double prob = (double) 1 / 3 * 1.0;
            strategy[i] = prob;
        }
        return;
    }
    for (int i{}; i < 3; ++i) {
        if (regret[i] > 0) {
            double prob = (double) regret[i] / totals * 1.0;
            strategy[i] = prob;
        }
        else {
            strategy[i] = 0.0;
        }
    }
}
void FixedOpponent() {
    double fixed_prob = (double) 1/ 3 *1.0 ;
    // { prob to choose R, prob to choose P, prob to choose S}
    std::vector<double> opponent_strategy = {0.4, 0.3, 0.3};
    std::vector<double> player_strategy = {fixed_prob, fixed_prob, fixed_prob};
    std::vector<double> player_average_strategy = {0, 0, 0};
    std::vector<int> regret = {0, 0, 0};
    std::mt19937  gen(std::random_device{}());
    std::discrete_distribution<int> opponent_dist(opponent_strategy.begin(),opponent_strategy.end());
    for (int i{}; i < 100000; ++i) {
        std::discrete_distribution<int> player_dist(player_strategy.begin(), player_strategy.end());
        int opponent_action = opponent_dist(gen);
        int player_action = player_dist(gen);
        for (int j{}; j < 3; ++j) {
            player_average_strategy[j] += player_strategy[j];
        }
        RegretMatching(opponent_action, player_action, player_strategy, regret);
    }
    double total = 0.0;
    for (int i{}; i < 3; ++i) {
        double prob_strategy = (double)(player_average_strategy[i] / 100000.0);
        total += prob_strategy;
        std::cout<<prob_strategy <<" ";
    }
    std::cout<<"\n" << total;

}
void SelfPlay() {
    std::vector<double> player_one_strategy = {0.3, 0.4, 0.3};
    std::vector<double> player_two_strategy = {0.4, 0.3, 0.3};
    std::vector<double> player_one_average_strategy = {0.0, 0.0, 0.0};
    std::vector<double> player_two_average_strategy = {0.0, 0.0, 0.0};
    std::vector<int> player_one_regret = {0, 0, 0};
    std::vector<int> player_two_regret = {0, 0, 0};
    std::mt19937 gen(std::random_device{}());

    for (int i{}; i < 100000; ++i) {
        std::discrete_distribution<int> player_one_dist(player_one_strategy.begin(), player_one_strategy.end());
        std::discrete_distribution<int> player_two_dist(player_two_strategy.begin(), player_two_strategy.end());
        int player_one_action = player_one_dist(gen);
        int player_two_action = player_two_dist(gen);
        for (int j{}; j < 3; ++j) {
            player_one_average_strategy[j] += player_one_strategy[j];
            player_two_average_strategy[j] += player_two_strategy[j];
        }
        RegretMatching(player_two_action, player_one_action, player_one_strategy, player_one_regret);
        RegretMatching(player_one_action, player_two_action, player_two_strategy, player_two_regret);
    }

    std::cout<<"Player One Average Strategy\n";
    for (int i{}; i < 3; ++i) {
        double prob_strategy = (double) (player_one_average_strategy[i] / 100000.0);
        std::cout<<prob_strategy <<" ";
    }
    std::cout<<"\nPlayer Two Average Strategy\n";
    for (int i{}; i < 3; ++i) {
        double prob_strategy = (double) (player_two_average_strategy[i] / 100000.0);
        std::cout<<prob_strategy <<" ";
    }
}
void Play() {
    ui::enableAnsi();
    int round = 0;
    int wins = 0;
    int losses = 0;
    int draws = 0;
    int human = -1;
    int bot = -1;
    int result = 0;
    std::vector<double> bot_strategy = {0.3, 0.3, 0.4};
    std::vector<double> bot_average_strategy = {0, 0, 0};
    std::vector<int> regret = {0, 0, 0};
    std::mt19937  gen(std::random_device{}());
    std::vector<int>results;
    while (1){
        std::vector<double> bot_average_display(3);
        for (int j{}; j < 3; ++j) {
           bot_average_display[j] = (double) bot_average_strategy[j] / round *1.0;
        }
        ui::clear();
        ui::renderHeader();
        ui::renderStatus(round, wins, losses, draws);
        ui::renderLastRound(human, bot, result);
        ui::renderHistory(results);
        ui::renderStrategy("Bot Strategy",bot_strategy);   // strategia botului
        std::discrete_distribution<int> bot_dist(bot_strategy.begin(), bot_strategy.end());
        ui::renderStrategy("Bot Average Strategy", bot_average_display);
        ui::renderControls();
        ui::renderPrompt();
        char action;
        std::cin>>action;
        int player_action;
        if (action == 'r') {
            player_action = 0;
        }
        else if (action == 'p') {
            player_action = 1;
        }
        else if (action == 's') {
            player_action = 2;
        }
        else if (action == 'q') {
            break;
        }
        else {
            continue;
        }
        int bot_action = bot_dist(gen);
        for (int j{}; j < 3; ++j) {
            bot_average_strategy[j] += bot_strategy[j];
        }
        RegretMatching(player_action, bot_action, bot_strategy, regret);
        if (bot_action ==  0) {
            if (player_action == 0) {
                draws++;
                result = 0;
            }
            if (player_action == 1) {
                wins++;
                result = 1;
            }
            if (player_action == 2) {
                losses ++;
                result = -1;
            }
        }
        else if (bot_action == 1) {
            if (player_action == 0) {
                losses++;
                result = -1;
            }
            if (player_action == 1) {
                draws ++;
                result = 0;
            }
            if (player_action == 2) {
                wins ++;
                result = 1;
            }
        }
        else {
            if (player_action == 0) {
                wins++;
                result = 1;
            }
            if (player_action == 1) {
                losses++;
                result = -1;
            }
            if (player_action == 2) {
                draws ++;
                result = 0;
            }
        }
        human = player_action;
        bot = bot_action;
        round ++;
        results.push_back(result);
    }
}
int main() {
    // FixedOpponent();
    // SelfPlay();

    Play();
}