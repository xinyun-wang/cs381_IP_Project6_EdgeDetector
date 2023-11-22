#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Edge {
public:
	int numR;
	int numC;
	int minV;
	int maxV;
    int** mirrorFramedAry;
    int RobertVertical[2][2] = { {1,-1},{1,-1} };
    int RobertHorizontal[2][2] = { {1,1},{-1,-1} };
    int RobertRightDiag[2][2] = { {1,-1},{-1,1} };
    int RobertLeftDiag[2][2]= { {-1,1},{1,-1} };
    int** RobertEdgeAry;
    int SobelVertical[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };
    int SobelHorizontal[3][3] = { {1,2,1},{0,0,0},{-1,-2,-1} };
    int SobelRightDiag[3][3] = { {2,1,0},{1,0,-1},{0,-1,-2} };
    int SobelLeftDiag[3][3] = { {0,1,2},{-1,0,1},{-2,-1,0} };
    int** SobelEdgeAry;
    int* histRobertAry;
    int* histSobelAry;

    Edge() {

    }

    void loadImage(ifstream& in) {
        for (int i = 1; i < numR+1; i++) {
            for (int j = 1; j < numC+1 ; j++) {
                in >> mirrorFramedAry[i][j];
            }
        }
    }

    void mirrorFraming() {
        for (int i = 0; i < numC+1; i++) {
            mirrorFramedAry[0][i] = mirrorFramedAry[1][i];
            mirrorFramedAry[numR + 1][i] = mirrorFramedAry[numR][i];
        }

        for (int i = 0; i<= numR + 1; i++) {
            mirrorFramedAry[i][0] = mirrorFramedAry[i][1];
            mirrorFramedAry[i][numC+1] = mirrorFramedAry[i][numC];

        }
    }

    int computeRobert(int i, int j, int(*mask)[2]) {
        int c = mirrorFramedAry[i][j] * mask[0][0] + mirrorFramedAry[i][j + 1] * mask[0][1] + mirrorFramedAry[i + 1][j] * mask[1][0] + mirrorFramedAry[i + 1][j + 1] * mask[1][1];
        return c;
    }

    int computeSobel(int i, int j, int(*mask)[3]) {
        int c = 0;
        for (int x = -1; x < 2; x++) {
            for (int y = -1; y < 2; y++) {
                c += mirrorFramedAry[i + x][j + y] * mask[x+1][y+1];
            }
        }
        return c;
    }

    void imgReformat(int** inAry, ofstream& out) {
        out << numR << " " << numC << " " << minV << " " << maxV << endl;
        string s = to_string(maxV);

        int w = s.length();
        int r = 1;
        while (r < numR + 1) {
            int c = 1;
            while (c < numC + 1) {
                out << inAry[r][c];
                s = to_string(inAry[r][c]);
                int ww = s.length();
                while (ww <= w) {
                    out << " ";
                    ww++;
                }
                c++;
            }
            out << endl;
            r++;
        }
    }

    void RobertEdgeDetector(ofstream& de) {
        de << "entering RobertEdgeDetector method" << endl;
        int newMax = 0;
        int newMin = 99999;
        int i = 1;

        while (i <=numR)
        {
            int j =1;
            while (j <= numC)
            {
                int tmpV = abs(computeRobert(i, j, RobertVertical));
                int tmpH = abs(computeRobert(i, j, RobertHorizontal));
                int tmpR = abs(computeRobert(i, j, RobertRightDiag));
                int tmpL = abs(computeRobert(i, j, RobertLeftDiag));

                RobertEdgeAry[i][j] = tmpV + tmpH + tmpR + tmpL;
                if (RobertEdgeAry[i][j] > newMax) 
                    newMax = RobertEdgeAry[i][j];
                if (RobertEdgeAry[i][j] > newMin)
                    newMin = RobertEdgeAry[i][j];
                
                j++;
            }
            i++;
        }
        maxV = newMax;
        minV = newMax;

        de << "Leaving RobertEdgeDetector method" << endl;
    }

    void SobelEdgeDetector(ofstream& de) {
        de << "entering SobelEdgeDetector method" << endl;
        int newMax = 0;
        int newMin = 99999;
        int i = 1;

        while (i <= numR)
        {
            int j = 1;
            while (j <= numC)
            {
                int tmpV = abs(computeSobel(i, j, SobelVertical));
                int tmpH = abs(computeSobel(i, j, SobelHorizontal));
                int tmpR = abs(computeSobel(i, j, SobelRightDiag));
                int tmpL = abs(computeSobel(i, j, SobelLeftDiag));

                SobelEdgeAry[i][j] = tmpV + tmpH + tmpR + tmpL;
             
                if (SobelEdgeAry[i][j] > newMax) 
                    newMax = SobelEdgeAry[i][j];
                if (SobelEdgeAry[i][j] > newMin)
                    newMin = SobelEdgeAry[i][j];
                
                j++;
            }
            i++;
        }
        maxV = newMax;
        minV = newMax;

        de << "Leaving SobelEdgeDetector method"<<endl;
    }

    void computeHist(int** inAry, int* histAry, ofstream& de) {
        de << "Entering computeHist method" << endl;

        int i = 1;
        while (i < numR) {
            int j = 1;
            while (j < numC) {
                histAry[inAry[i][j]]++;
                j++;
            }
            i++;
        }

        de << "Leaving computeHist method" << endl;
    }

    void printHist(int* histAry, ofstream& histFile, ofstream& de) {
        de << "Entering printHist method" << endl;
        histFile << numR << " " << numC << " " << minV << " " << maxV << endl;
        int index = 0;
        while (index <= maxV) {
            histFile << index << " " << histAry[index] << endl;
            index++;
        }

        de << "Leaving printHist method" << endl;
    }
};

int main(int argc, char* argv[]) {
    ifstream in(argv[1]);
    string choice = argv[2];
    ofstream out(argv[3]);
    ofstream debug(argv[4]);

    Edge e;
    in >> e.numR >> e.numC >> e.minV >> e.maxV;
    
    e.mirrorFramedAry = new int* [e.numR + 2];
    e.RobertEdgeAry = new int* [e.numR + 2];
    e.SobelEdgeAry = new int* [e.numR + 2];

    for (int j = 0; j < e.numR + 2; j++) {
        e.mirrorFramedAry[j] = new int[e.numC + 2];
        e.RobertEdgeAry[j] = new int[e.numC + 2];
        e.SobelEdgeAry[j] = new int[e.numC + 2];
    }

    e.loadImage(in);
    e.mirrorFraming();
    e.imgReformat(e.mirrorFramedAry, out);

    if (choice._Equal("1") ){   
       
        e.RobertEdgeDetector(debug); 
        e.histRobertAry = new int[e.maxV + 1];
        for (int i = 0; i < e.maxV + 1; i++) {
            e.histRobertAry[i] = 0;
        }
        e.computeHist(e.RobertEdgeAry, e.histRobertAry,debug);

        string nameRobertEdge = (string)argv[1] + "_Robert_edge.txt";
        ofstream RobertEdgeFile(nameRobertEdge); 

        e.imgReformat(e.RobertEdgeAry, out);

        RobertEdgeFile << e.numR << " " << e.numC << " " << e.minV << " " << e.maxV << endl;

        for (int i = 1; i <= e.numR; ++i) {
            for (int j = 1; j <= e.numC; ++j) {
                RobertEdgeFile << e.RobertEdgeAry[i][j] << " ";
            }
            RobertEdgeFile << endl;
        }

        string RobertHist = (string)argv[1] + "_Robert_hist.txt";
        ofstream histRobertFile(RobertHist);

        e.printHist(e.histRobertAry, histRobertFile, debug);
        //histRobertFile.close();
    }
    if (choice._Equal("2")) {
        e.SobelEdgeDetector(debug);
        e.histSobelAry = new int[e.maxV + 1];
        for (int i = 0; i < e.maxV + 1; i++) {
            e.histSobelAry[i] = 0;
        }
        e.computeHist(e.SobelEdgeAry, e.histSobelAry, debug);

        string nameSobelEdge = (string)argv[1] + "_Sobel_edge.txt";
        ofstream SobelEdgeFile(nameSobelEdge);

        e.imgReformat(e.SobelEdgeAry, out);

        SobelEdgeFile << e.numR << " " << e.numC << " " << e.minV << " " << e.maxV << endl;

        for (int i = 1; i <= e.numR; ++i) {
            for (int j = 1; j <= e.numC; ++j) {
                SobelEdgeFile << e.SobelEdgeAry[i][j] << " ";
            }
            SobelEdgeFile << endl;
        }

        string SobelHist = (string)argv[1] + "_Sobel_hist.txt";
        ofstream histSobelFile(SobelHist); 

        e.printHist(e.histSobelAry, histSobelFile, debug);
        SobelEdgeFile.close();
    }

    in.close();
    out.close();
    debug.close();
}