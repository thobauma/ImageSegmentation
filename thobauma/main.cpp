#include <iostream>
#include <iomanip> // for setprecision

#include "bitmap.h"
#include "graph.h"

void ShowResult(const Bitmap& original, Graph& graph, const std::string& filename){
    Bitmap resbmp = graph.graphToBitmap();
    // std::stringstream s;
    // s << filename << ".bmp";
    std::string originalF = "/images/"+filename+".bmp";
    original.Save(originalF);
    std::string resultF = "/images/"+filename+"Seg.bmp";
    resbmp.Save(resultF);
}


int main(){
    std::cout << std::setprecision(3); // don't remove
    std::cout << "image name (e.g. bird, peppers or sailboat)?";
    std::string filename;
    std::cin >> filename;
    Bitmap bitmap = readpgm("./" + filename + ".pgm");
    Graph g(bitmap);
    g.minCut();
    ShowResult(bitmap, g, filename);
    return 0;
}
