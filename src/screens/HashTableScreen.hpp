#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "../ui/Button.hpp"

struct AnimationStep
{
    std::string message;
    int highlightLine = -1;
    std::vector<std::vector<int>> tableSnapshot;
    int activeBucket = -1;
    int activeNode = -1;
};

class HashTableScreen
{
private:
    static constexpr int FIXED_N = 10;
    const sf::Font &m_font;

    std::vector<std::vector<int>> m_table;
    std::vector<AnimationStep> m_steps;
    size_t m_currentStep = 0;
    std::vector<std::string> m_pseudocode;

    Button m_createButton, m_searchButton, m_insertButton, m_removeButton, m_backButton;
    Button m_prevButton, m_nextButton, m_playPauseButton;

    sf::Text m_messageText, m_inputText, m_topInfoText, m_speedText;
    sf::RectangleShape m_inputBox, m_background, m_leftBar;

    sf::RectangleShape m_sliderBar;
    sf::CircleShape m_sliderKnob;

    bool m_isPlaying = false;
    sf::Clock m_autoTimer;
    float m_speedScale = 1.0f;
    std::string m_input;
    int m_elementCount = 0;

private:
    // Thêm một bước trạng thái vào danh sách mô phỏng hoạt động
    void addStep(const std::string &msg, int line, int bucket = -1, int node = -1);
    // Thiết lập nội dung mã giả hiển thị trên màn hình tùy theo thao tác
    void setupPseudocode(const std::string &action);
    // Khởi tạo bảng băm mới với một vài giá trị ngẫu nhiên
    void createTable();
    // Tính toán chỉ số bucket bằng phép toán modulo: value % 10
    int hashFunction(int value) const;

public:
    explicit HashTableScreen(const sf::Font &font);
    // Xử lý sự kiện từ người dùng (click chuột, nhập phím, kéo slider)
    void handleEvent(const sf::Event &event, sf::RenderWindow &window, bool &goBack);
    // Cập nhật logic hoạt ảnh (tự động chuyển bước theo thời gian)
    void update(const sf::RenderWindow &window);
    // Vẽ toàn bộ các thành phần (bảng băm, nút bấm, mã giả) lên cửa sổ 
    void draw(sf::RenderWindow &window) const;

    // Chuẩn bị các bước mô phỏng cho thao tác Tìm kiếm
    void prepareSearch(int value);
    // Chuẩn bị các bước mô phỏng cho thao tác Chèn (xử lý trùng lặp)
    void prepareInsert(int value);
    // Chuẩn bị các bước mô phỏng cho thao tác Xóa
    void prepareRemove(int value);
};