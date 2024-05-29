#pragma once

#include "player.hpp"

#include <array>
#include <string>
#include <random>
#include <optional>
#include <functional>
#include <iostream>

class Game {
public:
    Game(const std::string& name1 = "player", const std::string& name2 = "computer")
    : player(name1)
    , computer(name2)
    , e1()
    , random_move(0, moves.size() - 1)
    {}

    enum class Choice {
        EXIT,
        MOVE,
        INVALID
    };

    std::string random_choice() noexcept
    {
        return moves.at(random_move(e1));
    }

    std::string select(std::string& selection)
    {
        std::transform(selection.begin(), selection.end(), selection.begin(), 
            [](char c){ return std::tolower(c); });

        if(auto iter = std::find(moves.begin(), moves.end(), selection); iter == moves.end())
        {
            throw std::invalid_argument{"There is no move such as '" + selection  + "'" };
        }

        return selection;
    }

    std::optional<std::reference_wrapper<Player>> winCheck() noexcept
    {
        if(player.choice == computer.choice)
        {
            return std::nullopt;
        }
        if(player.choice == "rock" && computer.choice == "scissors"
            || player.choice == "paper" && computer.choice == "rock"
            || player.choice == "scissors" && computer.choice == "paper")
        {
            return std::ref(player);
        }
        return std::ref(computer);
    }

    void print_score() noexcept
    {
        std::cout << player.name << "'s score: " << player.score << '\n';
        std::cout << computer.name << "'s score: " << computer.score << '\n';
    }

    int run()
    {
        std::cout << "Welcome to the Rock, Paper, Scissors Game!!\n"
            << "For exit please write exit\n"
            << "For scores please write score\n";

        std::cout << "\nEnter your choice: ";
        for(std::string choice; std::cin >> choice; std::cout << "\nEnter your choice: ")
        {
            switch (evaluate_choice(choice))
            {
            case Choice::EXIT:
                print_score();
                return EXIT_SUCCESS;            
            case Choice::INVALID:
                return EXIT_FAILURE;
            default:
                break;
            }
        }

        return EXIT_FAILURE;
    }

private:
    const std::array<std::string, 3> moves{"rock", "paper", "scissors"};
    
    Player player;
    Player computer;

    std::default_random_engine e1;
    std::uniform_int_distribution<int> random_move;

    Choice evaluate_choice(std::string& choice)
    {
        if (choice == "exit")
        {
            return Choice::EXIT;
        }

        try
        {
            player.choice = select(choice);
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << e.what() << '\n';
            return Choice::INVALID;
        }

        computer.choice = random_choice();

        std::cout << "\nYour choice is: " << player.choice << '\n';
        std::cout << "Computer's choice is: " << computer.choice << '\n';

        if(auto winner = winCheck(); winner.has_value())
        {
            winner.value().get().score++;
            std::cout << winner.value().get().name << " won!\n";
        }
        else
        {
            std::cout << "Draw!\n";
        }

        return Choice::MOVE;
    }
};