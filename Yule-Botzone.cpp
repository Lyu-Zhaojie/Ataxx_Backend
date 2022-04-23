#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>

constexpr unsigned long long MASK{0b00000000'01111111'01111111'01111111'01111111'01111111'01111111'01111111};
constexpr unsigned long long IN_RADIUS_1[64]{
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000011'00000010,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000111'00000101,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00001110'00001010,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00011100'00010100,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00111000'00101000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'01110000'01010000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'01100000'00100000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000000'00000000'00000011'00000010'00000011,
    0b00000000'00000000'00000000'00000000'00000000'00000111'00000101'00000111,
    0b00000000'00000000'00000000'00000000'00000000'00001110'00001010'00001110,
    0b00000000'00000000'00000000'00000000'00000000'00011100'00010100'00011100,
    0b00000000'00000000'00000000'00000000'00000000'00111000'00101000'00111000,
    0b00000000'00000000'00000000'00000000'00000000'01110000'01010000'01110000,
    0b00000000'00000000'00000000'00000000'00000000'01100000'00100000'01100000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000000'00000011'00000010'00000011'00000000,
    0b00000000'00000000'00000000'00000000'00000111'00000101'00000111'00000000,
    0b00000000'00000000'00000000'00000000'00001110'00001010'00001110'00000000,
    0b00000000'00000000'00000000'00000000'00011100'00010100'00011100'00000000,
    0b00000000'00000000'00000000'00000000'00111000'00101000'00111000'00000000,
    0b00000000'00000000'00000000'00000000'01110000'01010000'01110000'00000000,
    0b00000000'00000000'00000000'00000000'01100000'00100000'01100000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000011'00000010'00000011'00000000'00000000,
    0b00000000'00000000'00000000'00000111'00000101'00000111'00000000'00000000,
    0b00000000'00000000'00000000'00001110'00001010'00001110'00000000'00000000,
    0b00000000'00000000'00000000'00011100'00010100'00011100'00000000'00000000,
    0b00000000'00000000'00000000'00111000'00101000'00111000'00000000'00000000,
    0b00000000'00000000'00000000'01110000'01010000'01110000'00000000'00000000,
    0b00000000'00000000'00000000'01100000'00100000'01100000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000011'00000010'00000011'00000000'00000000'00000000,
    0b00000000'00000000'00000111'00000101'00000111'00000000'00000000'00000000,
    0b00000000'00000000'00001110'00001010'00001110'00000000'00000000'00000000,
    0b00000000'00000000'00011100'00010100'00011100'00000000'00000000'00000000,
    0b00000000'00000000'00111000'00101000'00111000'00000000'00000000'00000000,
    0b00000000'00000000'01110000'01010000'01110000'00000000'00000000'00000000,
    0b00000000'00000000'01100000'00100000'01100000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000011'00000010'00000011'00000000'00000000'00000000'00000000,
    0b00000000'00000111'00000101'00000111'00000000'00000000'00000000'00000000,
    0b00000000'00001110'00001010'00001110'00000000'00000000'00000000'00000000,
    0b00000000'00011100'00010100'00011100'00000000'00000000'00000000'00000000,
    0b00000000'00111000'00101000'00111000'00000000'00000000'00000000'00000000,
    0b00000000'01110000'01010000'01110000'00000000'00000000'00000000'00000000,
    0b00000000'01100000'00100000'01100000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000010'00000011'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000101'00000111'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00001010'00001110'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00010100'00011100'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00101000'00111000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'01010000'01110000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00100000'01100000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000};
constexpr unsigned long long IN_RADIUS_2[64]{
    0b00000000'00000000'00000000'00000000'00000000'00000111'00000100'00000100,
    0b00000000'00000000'00000000'00000000'00000000'00001111'00001000'00001000,
    0b00000000'00000000'00000000'00000000'00000000'00011111'00010001'00010001,
    0b00000000'00000000'00000000'00000000'00000000'00111110'00100010'00100010,
    0b00000000'00000000'00000000'00000000'00000000'01111100'01000100'01000100,
    0b00000000'00000000'00000000'00000000'00000000'01111000'00001000'00001000,
    0b00000000'00000000'00000000'00000000'00000000'01110000'00010000'00010000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000000'00000111'00000100'00000100'00000100,
    0b00000000'00000000'00000000'00000000'00001111'00001000'00001000'00001000,
    0b00000000'00000000'00000000'00000000'00011111'00010001'00010001'00010001,
    0b00000000'00000000'00000000'00000000'00111110'00100010'00100010'00100010,
    0b00000000'00000000'00000000'00000000'01111100'01000100'01000100'01000100,
    0b00000000'00000000'00000000'00000000'01111000'00001000'00001000'00001000,
    0b00000000'00000000'00000000'00000000'01110000'00010000'00010000'00010000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000111'00000100'00000100'00000100'00000111,
    0b00000000'00000000'00000000'00001111'00001000'00001000'00001000'00001111,
    0b00000000'00000000'00000000'00011111'00010001'00010001'00010001'00011111,
    0b00000000'00000000'00000000'00111110'00100010'00100010'00100010'00111110,
    0b00000000'00000000'00000000'01111100'01000100'01000100'01000100'01111100,
    0b00000000'00000000'00000000'01111000'00001000'00001000'00001000'01111000,
    0b00000000'00000000'00000000'01110000'00010000'00010000'00010000'01110000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000111'00000100'00000100'00000100'00000111'00000000,
    0b00000000'00000000'00001111'00001000'00001000'00001000'00001111'00000000,
    0b00000000'00000000'00011111'00010001'00010001'00010001'00011111'00000000,
    0b00000000'00000000'00111110'00100010'00100010'00100010'00111110'00000000,
    0b00000000'00000000'01111100'01000100'01000100'01000100'01111100'00000000,
    0b00000000'00000000'01111000'00001000'00001000'00001000'01111000'00000000,
    0b00000000'00000000'01110000'00010000'00010000'00010000'01110000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000111'00000100'00000100'00000100'00000111'00000000'00000000,
    0b00000000'00001111'00001000'00001000'00001000'00001111'00000000'00000000,
    0b00000000'00011111'00010001'00010001'00010001'00011111'00000000'00000000,
    0b00000000'00111110'00100010'00100010'00100010'00111110'00000000'00000000,
    0b00000000'01111100'01000100'01000100'01000100'01111100'00000000'00000000,
    0b00000000'01111000'00001000'00001000'00001000'01111000'00000000'00000000,
    0b00000000'01110000'00010000'00010000'00010000'01110000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000100'00000100'00000100'00000111'00000000'00000000'00000000,
    0b00000000'00001000'00001000'00001000'00001111'00000000'00000000'00000000,
    0b00000000'00010001'00010001'00010001'00011111'00000000'00000000'00000000,
    0b00000000'00100010'00100010'00100010'00111110'00000000'00000000'00000000,
    0b00000000'01000100'01000100'01000100'01111100'00000000'00000000'00000000,
    0b00000000'00001000'00001000'00001000'01111000'00000000'00000000'00000000,
    0b00000000'00010000'00010000'00010000'01110000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000100'00000100'00000111'00000000'00000000'00000000'00000000,
    0b00000000'00001000'00001000'00001111'00000000'00000000'00000000'00000000,
    0b00000000'00010001'00010001'00011111'00000000'00000000'00000000'00000000,
    0b00000000'00100010'00100010'00111110'00000000'00000000'00000000'00000000,
    0b00000000'01000100'01000100'01111100'00000000'00000000'00000000'00000000,
    0b00000000'00001000'00001000'01111000'00000000'00000000'00000000'00000000,
    0b00000000'00010000'00010000'01110000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,

    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
    0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000,
};
constexpr long long MIN{-64}, MAX{64};
constexpr int BLACK{0}, WHITE{1};

inline long long max(long long a, long long b) { return a > b ? a : b; }

inline long long min(long long a, long long b) { return a < b ? a : b; }

#define disableBit(source, position) \
    __asm__ volatile("btr %1, %0;"   \
                     : "+r"(source)  \
                     : "r"(position))

#define countTrailingZero(source, result) \
    __asm__ volatile("tzcnt %1, %0;"      \
                     : "=r"(result)       \
                     : "r"(source))

#define populationCount(source, result) \
    __asm__ volatile("popcnt %1, %0;"   \
                     : "=r"(result)     \
                     : "r"(source))

class ChessBoard
{
public:
    ChessBoard() : player{BLACK}, table{0b00000000'01000000'00000000'00000000'00000000'00000000'00000000'00000001,
                                        0b00000000'00000001'00000000'00000000'00000000'00000000'00000000'01000000} {}

    void move(unsigned long long start, unsigned long long destination)
    {
        table[player] ^= (IN_RADIUS_2[destination] & (1ULL << start)) | (1ULL << destination);
        unsigned long long change{table[player ^ 1] & IN_RADIUS_1[destination]};
        table[player] |= change;
        table[player ^ 1] ^= change;
    }

    long long evaluate()
    {
        if (table[BLACK] == 0)
            return MAX + 1;
        if (table[WHITE] == 0)
            return MIN - 1;
        long long black{0}, white{0};
        populationCount(table[BLACK], black);
        populationCount(table[WHITE], white);
        return white - black;
    }

    int cannotMove()
    {
        unsigned long long tmp{0};
        if (player == BLACK)
        {
            populationCount(table[BLACK], tmp);
            return tmp > 24ULL ? MIN - 1 : MAX + 1;
        }
        populationCount(table[WHITE], tmp);
        return tmp > 24ULL ? MAX + 1 : MIN - 1;
    }

    inline void changePlayer() { player ^= 1; }

    int player;
    unsigned long long table[2];
};

class Operation
{
public:
    unsigned long long start, destination;
    long long value;

    Operation() : start{0}, destination{0}, value{0} {}

    Operation(unsigned long long s, unsigned long long des) : start{s}, destination{des}, value{0} {}

    Operation(unsigned long long s, unsigned long long des, long long v) : start{s}, destination{des}, value{v} {}

    bool operator!=(const Operation &op) const { return start != op.start || destination != op.destination; }
};

class Array
{
public:
    Operation operations[128];
    int length;

    void sort()
    {
        std::sort(operations + 1, operations + length,
                  [](const Operation &op1, const Operation &op2)
                  { return op1.value < op2.value; });
    }

    void _sort()
    {
        std::sort(operations + 1, operations + length,
                  [](const Operation &op1, const Operation &op2)
                  { return op1.value > op2.value; });
    }

    Array() : operations(), length{1} {}
};

class State
{
public:
    ChessBoard chessBoard;
    Operation operation;

    State() : chessBoard(), operation() {}

    explicit State(const ChessBoard inputChessBoard) : chessBoard(inputChessBoard), operation() {}
};

Array getChoices(ChessBoard &chessBoard, int depth, Operation &lastBestOperation);

long long miniMaxSearch(State &st, long long alpha, long long beta, int depth);

void miniMaxSelect(State &st, long long alpha, long long beta, int depth);

std::mutex mtx;

int main()
{
    ChessBoard mainChessBoard;
    State answer;
    int turnNumber{0};
    scanf("%d", &turnNumber);
    int x0{0}, y0{0}, x1{0}, y1{0};
    for (int i{0}; i < turnNumber - 1; i++)
    {
        scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
        if (x0 >= 0)
        {
            mainChessBoard.move(x0 << 3 | y0, x1 << 3 | y1);
            mainChessBoard.changePlayer();
        }
        scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
        mainChessBoard.move(x0 << 3 | y0, x1 << 3 | y1);
        mainChessBoard.changePlayer();
    }
    scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
    if (x0 >= 0)
    {
        mainChessBoard.move(x0 << 3 | y0, x1 << 3 | y1);
        mainChessBoard.changePlayer();
    }
    answer.chessBoard = mainChessBoard;
    auto fun = [&answer]() -> void
    {
        miniMaxSearch(answer, MIN, MAX, 1);
        for (int i{2};; i++)
            miniMaxSelect(answer, MIN, MAX, i);
    };
    std::thread computationThread(fun);
    computationThread.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(950));
    mtx.lock();
    printf("%d %d %d %d",
           answer.operation.start & 7ULL,
           answer.operation.start >> 3,
           answer.operation.destination & 7ULL,
           answer.operation.destination >> 3);
    return 0;
}

Array getChoices(ChessBoard &chessBoard, int depth, Operation &lastBestOperation)
{
    Array operationList;
    unsigned long long empty{(chessBoard.table[BLACK] | chessBoard.table[WHITE]) ^ MASK};
    unsigned long long current{chessBoard.table[chessBoard.player]};
    unsigned long long start{0}, destination{0};
    ChessBoard tempChessBoard;
    while (empty)
    {
        tempChessBoard = chessBoard;
        countTrailingZero(empty, destination);
        unsigned long long starts{IN_RADIUS_1[destination] & current};
        if (starts)
        {
            countTrailingZero(starts, start);
            tempChessBoard.move(start, destination);
            tempChessBoard.changePlayer();
            Operation tempOp(start, destination);
            if (lastBestOperation != tempOp)
            {
                State newState(tempChessBoard);
                tempOp.value = miniMaxSearch(newState, MIN, MAX, depth >> 1);
                operationList.operations[operationList.length] = tempOp;
                operationList.length++;
            }
        }
        starts = IN_RADIUS_2[destination] & current;
        while (starts)
        {
            countTrailingZero(starts, start);
            tempChessBoard = chessBoard;
            tempChessBoard.move(start, destination);
            tempChessBoard.changePlayer();
            Operation tempOp(start, destination);
            if (lastBestOperation != tempOp)
            {
                State newSt(tempChessBoard);
                tempOp.value = miniMaxSearch(newSt, MIN, MAX, depth >> 1);
                operationList.operations[operationList.length] = tempOp;
                operationList.length++;
            }
            disableBit(starts, start);
        }
        disableBit(empty, destination);
    }
    if (chessBoard.player == WHITE)
        operationList._sort();
    else
        operationList.sort();
    operationList.operations[0] = lastBestOperation;
    return operationList;
}

long long miniMaxSearch(State &st, long long alpha, long long beta, int depth)
{
    if (depth == 0)
        return st.chessBoard.evaluate();
    unsigned long long flag = 0ULL;
    ChessBoard cb(st.chessBoard);
    unsigned long long empty{(cb.table[BLACK] | cb.table[WHITE]) ^ MASK};
    unsigned long long current{cb.table[cb.player]};
    unsigned long long start{0}, destination{0};
    long long value{cb.player == WHITE ? MIN : MAX};
    while (empty)
    {
        countTrailingZero(empty, destination);
        unsigned long long starts{IN_RADIUS_1[destination] & current};
        if (starts)
        {
            flag |= starts;
            countTrailingZero(starts, start);
            State newCb(st.chessBoard);
            newCb.chessBoard.move(start, destination);
            newCb.chessBoard.changePlayer();
            if (cb.player == WHITE)
            {
                value = max(value, miniMaxSearch(newCb, alpha, beta, depth - 1));
                if (value > alpha)
                {
                    alpha = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
            else
            {
                value = min(value, miniMaxSearch(newCb, alpha, beta, depth - 1));
                if (value < beta)
                {
                    beta = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
        }
        starts = IN_RADIUS_2[destination] & current;
        flag |= starts;
        while (starts)
        {
            countTrailingZero(starts, start);
            State newChessBoard(st.chessBoard);
            newChessBoard.chessBoard.move(start, destination);
            newChessBoard.chessBoard.changePlayer();
            if (cb.player == WHITE)
            {
                value = max(value, miniMaxSearch(newChessBoard, alpha, beta, depth - 1));
                if (value > alpha)
                {
                    alpha = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
            else
            {
                value = min(value, miniMaxSearch(newChessBoard, alpha, beta, depth - 1));
                if (value < beta)
                {
                    beta = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
            disableBit(starts, start);
        }
        disableBit(empty, destination);
    }
    return flag ? value : cb.cannotMove();
}

void miniMaxSelect(State &st, long long alpha, long long beta, int depth)
{
    Array opList = getChoices(st.chessBoard, depth, st.operation);
    if (st.chessBoard.player == WHITE)
    {
        long long value{MIN};
        for (int i{0}; i < opList.length; i++)
        {
            State nextSt(st.chessBoard);
            nextSt.chessBoard.move(opList.operations[i].start, opList.operations[i].destination);
            nextSt.chessBoard.changePlayer();
            long long tempValue = miniMaxSearch(nextSt, alpha, beta, depth - 1);
            opList.operations[i].value = tempValue;
            value = max(value, tempValue);
            if (value > alpha)
            {
                alpha = value;
                mtx.lock();
                st.operation = opList.operations[i];
                mtx.unlock();
            }
            else if (tempValue == alpha && (srand(static_cast<unsigned>(time(nullptr))), rand() & 1))
            {
                mtx.lock();
                st.operation = opList.operations[i];
                mtx.unlock();
            }
            if (beta < alpha)
                break;
        }
    }
    else
    {
        long long value{MAX};
        for (int i{0}; i < opList.length; i++)
        {
            State nextSt(st.chessBoard);
            nextSt.chessBoard.move(opList.operations[i].start, opList.operations[i].destination);
            nextSt.chessBoard.changePlayer();
            long long tempValue = miniMaxSearch(nextSt, alpha, beta, depth - 1);
            opList.operations[i].value = tempValue;
            value = min(value, tempValue);
            if (value < beta)
            {
                beta = value;
                mtx.lock();
                st.operation = opList.operations[i];
                mtx.unlock();
            }
            else if (tempValue == beta && (srand(static_cast<unsigned>(time(nullptr))), rand() & 1))
            {
                mtx.lock();
                st.operation = opList.operations[i];
                mtx.unlock();
            }
            if (beta < alpha)
                break;
        }
    }
}