#include <iostream>

#include "bitmap.hpp"
#include "graph.hpp"
// #define DEBUG

void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename){
    Bitmap resbmp = graph.graphToBitmap();
    std::string originalF = "./images/original/"+filename+"Original.bmp";
    original.Save(originalF);
    #ifdef DEBUG
        std::cout << "original saved" << std::endl;
    #endif
    std::string resultF = "./images/segmented/"+filename+"Seg.bmp";
    resbmp.Save(resultF);
    #ifdef DEBUG
        std::cout << "segmented image saved" << std::endl;
    #endif

}

void test(std::vector<std::string>& files)
{
    for(auto filename: files)
    {   
        std::cout << std::endl << filename << ": " << std::endl;
        Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
        Graph g(bitmap);
        g.prMinCut();
        // g.minCut();
        ShowResult(bitmap, g, filename);
    }
}

Graph testGraph()
{
    Graph g(6, 0, 5);
    g.addEdge(0, 1,16, 0);
    g.addEdge(0, 2,13, 0);
    g.addEdge(1, 2,10, 0);
    g.addEdge(1, 3,12, 0);
    g.addEdge(2, 1, 4, 0);
    g.addEdge(2, 4,14, 0);
    g.addEdge(3, 2, 9, 0);
    g.addEdge(3, 5,20, 0);
    g.addEdge(4, 3, 7, 0);
    g.addEdge(4, 5, 4, 0);
    g.printTest();
    auto mf = g.prMaxFlow();
    std::cout << "max flow: " << mf << std::endl;
    g.printTest();
    return g;
}

int main(){
    // testGraph();
    // std::string filename;
    // #ifndef DEBUG
    //     std::cout << "image name (e.g. test1, test2, test3)?" << std::endl;
    // #endif
    // #ifdef DEBUG
    // #else
    //     filename = "test1";
    //     // std::cin >> filename;
    // #endif
    // Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
    // #ifdef DEBUG
    //     std::cout << std::endl << "bitmap initalized" << std::endl;
    // #endif
    // Graph g(bitmap);
    // #ifdef DEBUG
    //     std::cout << "graph initialized" << std::endl;
    //     g.printTest();
    // #endif
    // g.prMinCut();
    // // g.minCut();
    // #ifdef DEBUG
    //     g.printTest();
    //     std::cout << "minCut done" << std::endl;
    // #endif
    // ShowResult(bitmap, g, filename);

    std::vector<std::string> files = {"bird", "debug", "peppers", "sailboat", "test1","test2", "test3"};
    // // {"debug"};//
    test(files);
    return 0;
}
