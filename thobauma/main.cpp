#include <iostream>
#include <iomanip> // for setprecision

#include "bitmap.hpp"
#include "graph.hpp"
#define DEBUG

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

Graph testGraph()
{
    Graph g(6, 0, 5);
    g.addEdge(0, 1,16);
    g.addEdge(0, 2,13);
    g.addEdge(1, 2,10);
    g.addEdge(1, 3,12);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 4,14);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 5,20);
    g.addEdge(4, 3, 7);
    g.addEdge(4, 5, 4);
    g.printTest();
    auto mf = g.edmondsKarp();
    std::cout << "max flow: " << mf << std::endl;
    g.printTest();
    return g;
}
void test(std::vector<std::string>& files)
{
    std::cout << std::setprecision(3); // don't remove
    for(auto filename: files)
    {   
        std::cout << std::endl << filename << ": " << std::endl;
        Bitmap bitmap = readpgm("./images/" + filename + ".pgm");
        Graph g(bitmap);
        g.minCut();
        ShowResult(bitmap, g, filename);
    }
}

int main(){
    // Graph a = testGraph();
    // std::cout << std::setprecision(3); // don't remove
    // // std::cout << "image name (e.g. bird, peppers or sailboat)?";
    // std::cout << "image name (e.g. test1, test2, test3)?" << std::endl;
    // std::string filename;
    // #ifdef DEBUG
    //     filename = "debug";
    // #else
    //     std::cin >> filename;
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
    // g.minCut();
    // #ifdef DEBUG
    //     g.printTest();
    // // g.printInfo();
    //     std::cout << "minCut done" << std::endl;
    // #endif
    // ShowResult(bitmap, g, filename);

    std::vector<std::string> files = {"bird", "debug", "peppers", "sailboat", "test1","test2", "test3"};
    test(files);
    return 0;
}
