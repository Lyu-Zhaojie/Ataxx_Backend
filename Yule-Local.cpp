#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <windows.h>

int searchCount{0};
constexpr int64_t MIN{-64}, MAX{64};
constexpr int BLACK{0}, WHITE{1};
constexpr char MEMORY_NAME[32] = "65b83838cd2e593b";
constexpr uint64_t MASK{0b00000000'01111111'01111111'01111111'01111111'01111111'01111111'01111111};
constexpr uint64_t IN_RADIUS_1[64]{
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
constexpr uint64_t IN_RADIUS_2[64]{
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

inline int64_t max(int64_t a, int64_t b) { return a > b ? a : b; }

inline int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }

#define disableBit(source, position) \
    asm volatile("btr %1, %0;"       \
                 : "+r"(source)      \
                 : "r"(position))

#define countTrailingZero(source, result) \
    asm volatile("tzcnt %1, %0;"          \
                 : "=r"(result)           \
                 : "r"(source))

#define count1(source, result)    \
    asm volatile("popcnt %1, %0;" \
                 : "=r"(result)   \
                 : "r"(source))

class ChessBoard
{
public:
    ChessBoard() : player{BLACK}, table{0b00000000'01000000'00000000'00000000'00000000'00000000'00000000'00000001,
                                        0b00000000'00000001'00000000'00000000'00000000'00000000'00000000'01000000} {}

    void move(uint64_t start, uint64_t destination)
    {
        table[player] ^= (IN_RADIUS_2[destination] & (1ULL << start)) | (1ULL << destination);
        uint64_t change{table[player ^ 1] & IN_RADIUS_1[destination]};
        table[player] |= change;
        table[player ^ 1] ^= change;
    }

    int64_t evaluate()
    {
        if (table[BLACK] == 0)
            return MAX + 1;
        if (table[WHITE] == 0)
            return MIN - 1;
        int64_t black{0}, white{0};
        count1(table[BLACK], black);
        count1(table[WHITE], white);
        return white - black;
    }

    int cannotMove()
    {

        uint64_t tmp{0};
        if (player == BLACK)
        {
            count1(table[BLACK], tmp);
            return tmp > 24ULL ? MIN - 1 : MAX + 1;
        }
        else
        {
            count1(table[WHITE], tmp);
            return tmp > 24ULL ? MAX + 1 : MIN - 1;
        }
    }

    inline void changePlayer() { player ^= 1; }

    int player;
    uint64_t table[2];
};

class Operation
{
public:
    uint64_t start, destination;
    int64_t value;

    Operation() : start{0}, destination{0}, value{0} {}

    Operation(uint64_t s, uint64_t des) : start{s}, destination{des}, value{0} {}

    Operation(uint64_t st, uint64_t des, int64_t v) : start{st}, destination{des}, value{v} {}

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

    Array() : operations{}, length{1} {}
};

class State
{
public:
    ChessBoard chessBoard;
    Operation operation;

    State() : chessBoard(), operation() {}

    explicit State(const ChessBoard tcb) : chessBoard(tcb), operation() {}
};

Array branch(ChessBoard &cb, int initDepth, Operation &op);

int64_t miniMaxS(State &st, int64_t alpha, int64_t beta, int depth);

void miniMax(State &st, int64_t alpha, int64_t beta, int depth);

int main()
{
    char *shareMemoryPointer;
    char *fileMapPointer = reinterpret_cast<char *>(OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, MEMORY_NAME));
    shareMemoryPointer = reinterpret_cast<char *>(MapViewOfFile(fileMapPointer, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    unsigned long long *blackChessBoard = reinterpret_cast<unsigned long long *>(shareMemoryPointer);
    unsigned long long *whiteChessBoard = reinterpret_cast<unsigned long long *>(shareMemoryPointer + 1);
    signed char *startXInMemory = reinterpret_cast<signed char *>(shareMemoryPointer + 16);
    signed char *startYInMemory = reinterpret_cast<signed char *>(shareMemoryPointer + 17);
    signed char *destinationXInMemory = reinterpret_cast<signed char *>(shareMemoryPointer + 18);
    signed char *destinationYInMemory = reinterpret_cast<signed char *>(shareMemoryPointer + 19);
    signed char *player = reinterpret_cast<signed char *>(shareMemoryPointer + 20);
    signed char *state = reinterpret_cast<signed char *>(shareMemoryPointer + 24);
    ChessBoard mainCb;
    State ans;
    for (;;)
    {
        if (*state == 3)
        {
            printf("%d %d %d %d\n", *startXInMemory, *startYInMemory, *destinationXInMemory, *destinationYInMemory);
            mainCb.player = *player;
            mainCb.table[BLACK] = *blackChessBoard;
            mainCb.table[WHITE] = *whiteChessBoard;
            if (*startXInMemory != -1)
            {
                mainCb.changePlayer();
                mainCb.move(uint64_t(*startXInMemory << 3 | *startYInMemory),
                            uint64_t(*destinationXInMemory << 3 | *destinationYInMemory));
                mainCb.changePlayer();
            }
            ans.chessBoard = mainCb;
            miniMaxS(ans, MIN, MAX, 1);
            for (int i = 1; i < 7; ++i)
                miniMax(ans, MIN, MAX, i);
            printf("Search Count: %d\n", searchCount);
            searchCount = 0;
            *startXInMemory = ans.operation.start >> 3;
            *startYInMemory = ans.operation.start & 7ULL;
            *destinationXInMemory = ans.operation.destination >> 3;
            *destinationYInMemory = ans.operation.destination & 7ULL;
            mainCb.move(uint64_t(*startXInMemory << 3 | *startYInMemory),
                        uint64_t(*destinationXInMemory << 3 | *destinationYInMemory));
            *blackChessBoard = mainCb.table[BLACK];
            *whiteChessBoard = mainCb.table[WHITE];
            printf("%d %d %d %d\n", *startXInMemory, *startYInMemory, *destinationXInMemory, *destinationYInMemory);
            *state = 4;
        }
        else
        {
            if (*state == 0)
            {
                mainCb.table[BLACK] = 0b00000000'01000000'00000000'00000000'00000000'00000000'00000000'00000001;
                mainCb.table[WHITE] = 0b00000000'00000001'00000000'00000000'00000000'00000000'00000000'01000000;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            else
            {
                printf("noInput\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
        }
    }
    return 0;
}

Array branch(ChessBoard &cb, int initDepth, Operation &op)
{
    Array opList;
    uint64_t empty{(cb.table[BLACK] | cb.table[WHITE]) ^ MASK};
    uint64_t current{cb.table[cb.player]};
    uint64_t start{0}, destination{0};
    ChessBoard tcb;
    while (empty)
    {
        tcb = cb;
        countTrailingZero(empty, destination);
        uint64_t r1From{IN_RADIUS_1[destination] & current};
        if (r1From)
        {
            countTrailingZero(r1From, start);
            tcb.move(start, destination);
            tcb.changePlayer();
            Operation tempOp(start, destination);
            if (op != tempOp)
            {
                State newSt(tcb);
                tempOp.value = miniMaxS(newSt, MIN, MAX, initDepth >> 1);
                opList.operations[opList.length] = tempOp;
                opList.length++;
            }
        }
        uint64_t r2From{IN_RADIUS_2[destination] & current};
        while (r2From)
        {
            countTrailingZero(r2From, start);
            tcb = cb;
            tcb.move(start, destination);
            tcb.changePlayer();
            Operation tempOp(start, destination);
            if (op != tempOp)
            {
                State newSt(tcb);
                tempOp.value = miniMaxS(newSt, MIN, MAX, initDepth >> 1);
                opList.operations[opList.length] = tempOp;
                opList.length++;
            }
            disableBit(r2From, start);
        }
        disableBit(empty, destination);
    }
    if (cb.player == BLACK)
        opList.sort();
    else
        opList._sort();
    opList.operations[0] = op;
    return opList;
}

int64_t miniMaxS(State &st, int64_t alpha, int64_t beta, int depth)
{

    searchCount++;
    if (depth == 0)
        return st.chessBoard.evaluate();
    uint64_t flag = 0ULL;
    ChessBoard cb(st.chessBoard);
    uint64_t empty{(cb.table[BLACK] | cb.table[WHITE]) ^ MASK};
    uint64_t current{cb.table[cb.player]};
    uint64_t start{0}, destination{0};
    int64_t value{cb.player == WHITE ? MIN : MAX};
    while (empty)
    {
        countTrailingZero(empty, destination);
        uint64_t r1Start{IN_RADIUS_1[destination] & current};
        if (r1Start)
        {
            flag |= r1Start;
            countTrailingZero(r1Start, start);
            State newCb(st.chessBoard);
            newCb.chessBoard.move(start, destination);
            newCb.chessBoard.changePlayer();
            if (cb.player == WHITE)
            {
                value = max(value, miniMaxS(newCb, alpha, beta, depth - 1));
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
                value = min(value, miniMaxS(newCb, alpha, beta, depth - 1));
                if (value < beta)
                {
                    beta = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
        }
        uint64_t r2Start{IN_RADIUS_2[destination] & current};
        flag |= r2Start;
        while (r2Start)
        {
            countTrailingZero(r2Start, start);
            State newCb(st.chessBoard);
            newCb.chessBoard.move(start, destination);
            newCb.chessBoard.changePlayer();
            if (cb.player == WHITE)
            {
                value = max(value, miniMaxS(newCb, alpha, beta, depth - 1));
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
                value = min(value, miniMaxS(newCb, alpha, beta, depth - 1));
                if (value < beta)
                {
                    beta = value;
                    st.operation = Operation(start, destination, value);
                }
                if (beta < alpha)
                    break;
            }
            disableBit(r2Start, start);
        }
        disableBit(empty, destination);
    }
    return flag ? value : cb.cannotMove();
}

void miniMax(State &st, int64_t alpha, int64_t beta, int depth)
{
    Array opList = branch(st.chessBoard, depth, st.operation);
    if (st.chessBoard.player == WHITE)
    {
        int64_t value{MIN};
        for (int i = 0; i < opList.length; i++)
        {
            State nextSt(st.chessBoard);
            nextSt.chessBoard.move(opList.operations[i].start, opList.operations[i].destination);
            nextSt.chessBoard.changePlayer();
            int64_t tempValue = miniMaxS(nextSt, alpha, beta, depth - 1);
            opList.operations[i].value = tempValue;
            value = max(value, tempValue);
            if (value > alpha)
            {
                alpha = value;
                st.operation = opList.operations[i];
            }
            else if (tempValue == alpha /* && (srand((unsigned) time(nullptr)), rand() & 1)*/)
                st.operation = opList.operations[i];
            if (alpha > beta)
                break;
        }
    }
    else
    {
        int64_t value{MAX};
        for (int i = 0; i < opList.length; i++)
        {
            State nextSt(st.chessBoard);
            nextSt.chessBoard.move(opList.operations[i].start, opList.operations[i].destination);
            nextSt.chessBoard.changePlayer();
            int64_t tempValue = miniMaxS(nextSt, alpha, beta, depth - 1);
            opList.operations[i].value = tempValue;
            value = min(value, tempValue);
            if (value < beta)
            {
                beta = value;
                st.operation = opList.operations[i];
            }
            else if (tempValue == beta /* && (srand((unsigned) time(nullptr)), rand() & 1)*/)
                st.operation = opList.operations[i];
            if (alpha > beta)
                break;
        }
    }
}
