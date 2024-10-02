#include "funciones.h"
using namespace std;
int randFun(int beg, int end)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(beg, end);

    return distrib(gen);
}
vector<pair<int, int>> genPathRand(pair<int, int> startPoint,  int size)
{

    
    vector<pair<int, int>> path;
    path.push_back(make_pair(startPoint.first, startPoint.second));
    int filc = startPoint.first, colc = startPoint.second;

    vector<int> opf = {0, 1};
    vector<int> opc = {-1, 0, 1};
    do
    {
        bool esta;
        pair<int, int> last;
            last = path.back();
            
        do
        {

            int addfil = opf[randFun(0, 1)];

            int addcol = opc[randFun(0, 2)];
            
            if (addfil==0)
            {
                filc = last.first + addfil;
                colc = last.second + addcol;
                if (colc<0 ||colc==size)
                {
                    colc-=addcol;
                }
                
                
            }else{
                if (addcol==0)
                {
                    filc = last.first + addfil;
                    colc = last.second + addcol;
                }else{
                    filc = last.first ;
                    colc = last.second ;
                }
                
                
            }


            esta = std::find(path.begin(), path.end(), make_pair(filc, colc)) != path.end();
        } while (esta);
        path.push_back(make_pair(filc, colc));

    } while (!(filc == size - 1));

    

    /* for (int i = 0; i < path.size(); i++)
    {
        cout << path[i].first << " " << path[i].second << endl;
    } */

    printVectorPair(path,size);
    return path;
}
vector<pair<int, int>> ordenarVPar(vector<pair<int, int>> v)
{
    std::sort(v.begin(), v.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
              {
        if (a.first != b.first)
            return a.first < b.first;
        return a.second < b.second; });

    return v;
}
void printVectorPair(vector<pair<int, int>> v, int size)
{
    cout<<"jj"<<endl;
    vector<pair<int, int>> ve = ordenarVPar(v);
    
    int cont=0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            auto it = std::find(ve.begin(), ve.end(), make_pair(i, j));

            if (it != ve.end()){
                cout << ve[cont].first << ',' << ve[cont].second << " ";
                cont++;}
            else
            {
                cout << "    ";
            }
        }
        cout << endl;
    }
}
void printMatriz(vector<vector<int>> mk)
{
    for (int i = 0; i < mk.size(); i++)
    {
        for (int j = 0; j < mk.size(); j++)
        {
            cout << mk[i][j] << " ";
        }
        cout << endl;
    }
}
void randomMapGen(int size, int finishPointFila, int startPointFila)
{

    
    
    int startPox = randFun(0, size - 1);
    int startPoy = startPointFila;
    cout << startPox;

    genPathRand(make_pair(startPoy, startPox), size);
    
}

int main(){
    randomMapGen(7, 6, 0);
    return 1;
}
