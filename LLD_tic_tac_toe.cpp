#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define M 1000000007
#define fio                           \
    ios_base::sync_with_stdio(false); \
    cin.tie(NULL);
#define d(x) cout << #x << " " << x << "\n";
#define min(x1, x2) (x1 > x2 ? x2 : x1)
#define max(x1, x2) (x1 < x2 ? x2 : x1)
#define min3(x1, x2, x3) (x3 > min(x1, x2) ? min(x2, x1) : x3)
#define max3(x1, x2, x3) (x3 < max(x1, x2) ? max(x1, x2) : x3)
#define ll long long int
#define ul unsigned long long int
#define p pair<ll, ll>
#define ld long double
#define dv(v)                            \
    cerr << #v << "  ";                  \
    for (int i = 0; i < (v).size(); i++) \
        cerr << v[i] << " ";             \
    cerr << "\n";
#define inf LONG_LONG_MAX
#define mp(x, y) make_pair(x, y)
#define oset tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
using namespace __gnu_pbds;
using namespace std;

class Game;
class Player;
class Move;

enum Cell
{
    Cross,
    Circle,
    Unoccupied
};

class Game
{
public:
    int total_moves;
    Cell board[3][3];
    Player *active_player;
    Player *passive_player;
    Player *winner;
    vector<Move *> moves;

    Game(Player *, Player *);
    bool make_move(Move);
    bool undo();
    void check_row(int);
    void check_col(int);
    void check_diagonals(int, int);
};

class Move
{
public:
    Player *move_by;
    int x, y;
    Move(int, int, Player *);
};

class Player
{
public:
    int id;
    string username;
    Cell symbol;
    int wins, losses;
    Player(int, string);
};

Game::Game(Player *player1, Player *player2)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            board[i][j] = Unoccupied;
        }
    }
    active_player = player1;
    passive_player = player2;
    winner = NULL;
    player1->symbol = Cross;
    player2->symbol = Circle;
    total_moves = 0;
}

bool Game::make_move(Move m)
{
    if (m.move_by != active_player || board[m.x][m.y] != Unoccupied || winner != NULL)
    {

        return 0;
    }
    ++total_moves;
    board[m.x][m.y] = active_player->symbol;
    moves.push_back(&m);
    check_row(m.x);
    check_col(m.y);
    check_diagonals(m.x, m.y);
    if (winner != NULL)
    {
        active_player->wins++;
        passive_player->losses++;
    }
    swap(active_player, passive_player);
    return 1;
}

void Game::check_row(int x)
{
    for (int i = 0; i < 3; ++i)
    {
        if (board[x][i] != active_player->symbol)
        {
            return;
        }
    }
    winner = active_player;
}

void Game::check_col(int y)
{
    for (int i = 0; i < 3; ++i)
    {
        if (board[i][y] != active_player->symbol)
        {
            return;
        }
    }
    winner = active_player;
}

void Game::check_diagonals(int x, int y)
{
    if ((x + y) % 2)
    {
        return;
    }
    int i, j;
    bool b;
    if (x == y)
    {
        i = 0, j = 0;
        b = 1;
        while (i != 3 && j != 3)
        {
            if (board[i][j] != active_player->symbol)
            {
                b = 0;
                break;
            }
            ++i;
            ++j;
        }
        if (b)
        {
            winner = active_player;
            return;
        }
    }
    if (x != y || (x == 1 && y == 1))
    {
        i = 0, j = 2;
        b = 1;
        while (i != 3)
        {
            if (board[i][j] != active_player->symbol)
            {
                b = 0;
                break;
            }
            ++i;
            --j;
        }
        if (b)
        {
            winner = active_player;
            return;
        }
    }
}

bool Game::undo()
{
    if (moves.empty())
    {
        return 0;
    }
    board[moves.back()->x][moves.back()->y] = Unoccupied;
    moves.pop_back();
    return 1;
}

Move::Move(int x, int y, Player *player)
{
    this->x = x;
    this->y = y;
    move_by = player;
}

Player::Player(int id, string name)
{
    this->id = id;
    username = name;
    symbol = Unoccupied;
    wins = 0, losses = 0;
}

int main()
{
    fio;
    ll t, n, x, y, z, k;

    Player p1 = Player(1, "sidak");
    Player p2 = Player(2, "sabby");
    Game game = Game(&p1, &p2);
    bool turn = 0;
    while (game.total_moves != 9 && game.winner == NULL)
    {
        cin >> x >> y;
        if (x == -1 && y == -1)
        {
            game.undo();
            turn = !turn;
            continue;
        }
        bool b = 0;
        if (turn == 0)
        {
            Move m = Move(x, y, &p1);
            b = game.make_move(m);
        }
        else
        {
            Move m = Move(x, y, &p2);
            b = game.make_move(m);
        }
        if (b)
        {
            turn = !turn;
        }
    }
    if (game.winner != NULL)
    {
        cout << game.winner->username << endl;
    }
    else
    {
        cout << "DRAW\n";
    }

    return 0;
}