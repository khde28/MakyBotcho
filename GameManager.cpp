#include "GameManager.h"
#include "constantes.h"
using namespace std;
GameManager::GameManager()
{

}
template <size_t N, size_t R, size_t C>
bool GameManager::loadMapa(const string &filename, int (&mapas)[N][R][C])
{
    ifstream in(filename);
    if (!in)
    {
        cerr << "Error al abrir " << filename << endl;
        return false;
    }

    int n, r, c;
    std::string line;

    while (std::getline(in, line))
    {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        ss >> n >> r >> c;
        break;
    }

    if (n != N || r != R || c != C)
    {
        cerr << "Dimensiones incorrectas en " << filename << endl;
        return false;
    }

    int k = 0, i = 0, j = 0;
    while (std::getline(in, line) && k < N) {
        if (line.empty()) continue; // omite líneas vacías
        std::stringstream ss(line);
        int value;
        while (ss >> value) {
            mapas[k][i][j] = value;
            j++;
            if (j == C) { j = 0; i++; }
            if (i == R) { i = 0; k++; }
            if (k == N) break;
        }
    }

    return true;
}

bool GameManager::initMapas(){
    for (int level = 0; level < Constantes::MAX_LEVELS; level++)
    {
        for (int i = 0; i < Constantes::GRIDSIZE; i++)
        {
            for (int j = 0; j < Constantes::GRIDSIZE; j++)
            {
                Constantes::mapas2d[level][i][j] = 0;
                Constantes::mapas3d[level][i][j] = 0;
            }
        }
    }
    // Load maps from files
    if (!loadMapa("mapas3d.txt", Constantes::mapas3d))
    {
        std::cerr << "Failed to load 3D maps" << std::endl;
        return 0;
    }
    
    if (!loadMapa("mapas2d.txt", Constantes::mapas2d))
    {
        std::cerr << "Failed to load 2D maps" << std::endl;
        return 0;
    }
    
    std::cout << "Successfully loaded " << Constantes::MAX_LEVELS << " levels" << std::endl;
    return 1;

}
void GameManager::InitGame()
{
    initMapas();

}

GameManager::~GameManager(){}