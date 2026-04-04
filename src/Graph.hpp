#pragma once
#include <vector>
#include <string>
#include <algorithm>

// Cấu trúc Đỉnh (Node)
struct Node {
    int id;
    float x, y; // Lưu tọa độ để vẽ bằng SFML sau này
};

// Cấu trúc Cạnh (Edge)
struct Edge {
    int u, v;
    int weight;

    // Toán tử so sánh để sắp xếp cạnh theo trọng số (dùng cho Kruskal)
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Cấu trúc lưu trữ trạng thái đồ thị tại MỘT bước của thuật toán
// Điều này cực kỳ quan trọng để làm chức năng step-by-step
struct GraphState {
    std::vector<Edge> currentMST; // Danh sách các cạnh đã được chọn vào cây khung
    Edge evaluatingEdge;          // Cạnh đang được thuật toán xét tới
    int highlightLine;            // Dòng code ảo cần làm sáng (highlight) trên UI
    std::string message;          // Câu thông báo cho UI (vd: "Cạnh 1-2 tạo chu trình, bỏ qua")
    bool isFinished;              // Đánh dấu thuật toán đã xong chưa
};

// Cấu trúc Disjoint Set (Union-Find) để kiểm tra chu trình cho Kruskal
struct DisjointSet {
    std::vector<int> parent;
    std::vector<int> rank;

    void init(int n);
    int find(int i);
    void unite(int i, int j);
};

// Cấu trúc Đồ thị chính
struct Graph {
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    // Các hàm thao tác cơ bản
    void addNode(int id, float x, float y);
    void addEdge(int u, int v, int weight);
    void clear();

    // Thuật toán Kruskal trả về danh sách các trạng thái để UI render
    std::vector<GraphState> getKruskalMSTStates();
};