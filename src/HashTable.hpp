#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <random>

class HashTable
{
public:
    struct InsertResult
    {
        int key;
        int value;
        int bucket;
    };

    static constexpr std::size_t TableSize = 10;

    HashTable();
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    int insert(int key, int value);
    InsertResult insertRandom();
    void draw(sf::RenderWindow& window) const;
    void clear();

private:
    struct Node
    {
        int key;
        int value;
        Node* next;

        Node(int k, int v);
    };

    std::array<Node*, TableSize> buckets_{};

    std::mt19937 rng_;
    std::uniform_int_distribution<int> keyDist_;
    std::uniform_int_distribution<int> valueDist_;

    int hashFunction(int key) const;
};