#include "HashTable.hpp"

#include <iostream>

HashTable::Node::Node(int k, int v) : key(k), value(v), next(nullptr) {}

HashTable::HashTable()
    : rng_(std::random_device{}()),
      keyDist_(0, 99),
      valueDist_(100, 999)
{
}

HashTable::~HashTable()
{
    clear();
}

int HashTable::hashFunction(int key) const
{
    return key % static_cast<int>(TableSize);
}

int HashTable::insert(int key, int value)
{
    int index = hashFunction(key);

    Node* newNode = new Node(key, value);
    newNode->next = buckets_[index];
    buckets_[index] = newNode;

    std::cout << "Inserted node: key = " << key
              << ", value = " << value
              << ", bucket = " << index << '\n';

    return index;
}

HashTable::InsertResult HashTable::insertRandom()
{
    int key = keyDist_(rng_);
    int value = valueDist_(rng_);
    int bucket = insert(key, value);

    return {key, value, bucket};
}

void HashTable::draw(sf::RenderWindow& window) const
{
    const float startX = 60.f;
    const float startY = 70.f;
    const float bucketWidth = 90.f;
    const float bucketHeight = 50.f;
    const float gapX = 18.f;

    const float nodeHeight = 20.f;
    const float nodeGap = 6.f;
    const float nodeTopGap = 12.f;

    for (std::size_t i = 0; i < TableSize; i++)
    {
        float x = startX + static_cast<float>(i) * (bucketWidth + gapX);

        sf::RectangleShape bucketBox({bucketWidth, bucketHeight});
        bucketBox.setPosition({x, startY});
        bucketBox.setFillColor(sf::Color(45, 45, 45));
        bucketBox.setOutlineThickness(2.f);
        bucketBox.setOutlineColor(sf::Color::White);
        window.draw(bucketBox);

        Node* current = buckets_[i];
        int level = 0;

        while (current != nullptr)
        {
            sf::RectangleShape nodeBox({bucketWidth, nodeHeight});
            nodeBox.setPosition({
                x,
                startY + bucketHeight + nodeTopGap + level * (nodeHeight + nodeGap)
            });
            nodeBox.setFillColor(sf::Color(80, 170, 255));
            nodeBox.setOutlineThickness(1.5f);
            nodeBox.setOutlineColor(sf::Color::White);
            window.draw(nodeBox);

            current = current->next;
            level++;
        }
    }
}

void HashTable::clear()
{
    for (Node*& head : buckets_)
    {
        while (head != nullptr)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
}