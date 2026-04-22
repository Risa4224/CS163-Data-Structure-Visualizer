#include "Graph.hpp"

// --- Triển khai Disjoint Set ---
void DisjointSet::init(int n) {
    parent.resize(n);
    rank.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int i) {
    if (parent[i] == i) return i;
    return parent[i] = find(parent[i]); // Path compression
}

void DisjointSet::unite(int i, int j) {
    int rootI = find(i);
    int rootJ = find(j);
    if (rootI != rootJ) {
        if (rank[rootI] < rank[rootJ]) {
            parent[rootI] = rootJ;
        } else if (rank[rootI] > rank[rootJ]) {
            parent[rootJ] = rootI;
        } else {
            parent[rootJ] = rootI;
            rank[rootI]++;
        }
    }
}

// --- Triển khai Graph ---
void Graph::addNode(int id, float x, float y) {
    nodes.push_back({id, x, y});
}

void Graph::addEdge(int u, int v, int weight) {
    edges.push_back({u, v, weight});
}

void Graph::clear() {
    nodes.clear();
    edges.clear();
}

std::vector<GraphState> Graph::getKruskalMSTStates() {
    std::vector<GraphState> states;
    std::vector<Edge> currentMST;
    
    // Tạo trạng thái ban đầu
    GraphState initialState;
    initialState.highlightLine = 1; // Ví dụ: Dòng 1 là "Sắp xếp các cạnh"
    initialState.message = "Bat dau thuat toan Kruskal. Sap xep cac canh...";
    initialState.isFinished = false;
    states.push_back(initialState);

    // 1. Sắp xếp các cạnh tăng dần theo trọng số
    std::vector<Edge> sortedEdges = edges;
    std::sort(sortedEdges.begin(), sortedEdges.end());

    // 2. Khởi tạo Disjoint Set
    DisjointSet ds;
    ds.init(nodes.size());

    // 3. Duyệt qua từng cạnh
    for (const auto& edge : sortedEdges) {
        GraphState stepState;
        stepState.currentMST = currentMST;
        stepState.evaluatingEdge = edge;
        stepState.highlightLine = 2; // Ví dụ: Dòng 2 là "Xet canh tiep theo"
        stepState.message = "Dang xet canh noi " + std::to_string(edge.u) + " va " + std::to_string(edge.v) + " (Trong so: " + std::to_string(edge.weight) + ")";
        stepState.isFinished = false;
        states.push_back(stepState); // Lưu trạng thái đang xét cạnh

        int rootU = ds.find(edge.u);
        int rootV = ds.find(edge.v);

        if (rootU != rootV) {
            // Nếu không tạo chu trình, thêm vào MST
            ds.unite(rootU, rootV);
            currentMST.push_back(edge);
            
            GraphState addState;
            addState.currentMST = currentMST;
            addState.evaluatingEdge = edge;
            addState.highlightLine = 3; // Ví dụ: Dòng 3 là "Them vao cay khung"
            addState.message = "Khong tao chu trinh. Them vao cay khung nho nhat.";
            addState.isFinished = false;
            states.push_back(addState); // Lưu trạng thái sau khi thêm thành công
        } else {
            // Nếu tạo chu trình, bỏ qua
            GraphState skipState;
            skipState.currentMST = currentMST;
            skipState.evaluatingEdge = edge;
            skipState.highlightLine = 4; // Ví dụ: Dòng 4 là "Bo qua canh"
            skipState.message = "Tao chu trinh! Bo qua canh nay.";
            skipState.isFinished = false;
            states.push_back(skipState); // Lưu trạng thái bỏ qua
        }

        // Dừng sớm nếu MST đã đủ (V - 1) cạnh
        if (currentMST.size() == nodes.size() - 1) {
            break;
        }
    }

    // Trạng thái kết thúc
    GraphState finalState;
    finalState.currentMST = currentMST;
    finalState.highlightLine = 5; // Dòng kết thúc
    finalState.message = "Hoan thanh thuat toan Minimum Spanning Tree!";
    finalState.isFinished = true;
    states.push_back(finalState);

    return states;
}