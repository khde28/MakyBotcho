#include "genMapRand.h"
using namespace std;
int randFun(int beg, int end)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(beg, end);

    return distrib(gen);
}

vector<pair<int, int>> genPathRand2(pair<int, int> startPoint,  int size)
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
void printMatriz2(vector<vector<int>> mk)
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
void randomMapGen2(int size, int finishPointFila, int startPointFila)
{

    
    
    int startPox = randFun(0, size - 1);
    int startPoy = startPointFila;
    cout << startPox;

    genPathRand2(make_pair(startPoy, startPox), size);
    
}
//---------------------------------------------------------

const int WIDTH = 15;   
const int HEIGHT = 15;  

std::vector<std::vector<int>> maze(HEIGHT, std::vector<int>(WIDTH, 1));  

std::vector<std::pair<int, int>> movements = { {0, 2}, {0, -2}, {2, 0}, {-2, 0} };

void printMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << (maze[i][j] == 1 ? "#" : " ");
        }
        std::cout << std::endl;
    }
}

void generateMaze(int x, int y) {
    maze[y][x] = 0;  

    std::random_shuffle(movements.begin(), movements.end());

    std::default_random_engine engine(static_cast<unsigned int>(std::time(nullptr)));

    std::shuffle(movements.begin(), movements.end(), engine);
    for (auto& move : movements) {
        int nx = x + move.first;   
        int ny = y + move.second;  

        if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && maze[ny][nx] == 1) {
            maze[y + move.second / 2][x + move.first / 2] = 0;
            generateMaze(nx, ny);
        }
    }
}
//-----------------------------------------------------




/* int main(){

    int startX = 0;
    int startY = 0;
    generateMaze(1, randFun(1,14));

    maze[0][1] = 0;  
    maze[HEIGHT - 1][WIDTH - 2] = 0; 

    printMaze();
    //randomMapGen2(7, 6, 0);
    return 1;
} */
