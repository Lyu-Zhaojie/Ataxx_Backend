/**
 * @file Greedy_Ataxx.c
 * @author Lyu_Zhaojie
 * @version V1.0
 * @brief 使用贪心算法的同化棋AI
 */

#include <stdio.h>

const int BLACK = 0, WHITE = 1;

/**
 * @brief 一个宏。用于关闭`source`的第`position`个二进制位。以最低位为第0位。
 * @param source
 * @param position
 * @par Sample
 * @code
 *     int k = 12; // aka 0b1100
 *     disableBit(k, 3);
 *     assert(k == 4);
 * @endcode
 */
#define disableBit(source, position) \
    __asm__ volatile("btr %1, %0;"   \
                     : "+r"(source)  \
                     : "r"(position))

/**
 * @brief 一个宏。用于计算`source`的二进制的尾随零个数
 * @param source
 * @param result
 * @par Sample
 * @code
 *     int k = 24，res = 0; // k = 0b11000
 *     countTrailingZero(k, res);
 *     assert(res == 3);
 * @endcode
 */
#define countTrailingZero(source, result) \
    __asm__ volatile("tzcnt %1, %0;"      \
                     : "=r"(result)       \
                     : "r"(source))

/**
 * @brief 一个宏。用于计算`source`的二进制中`1`的个数
 * @param `source`
 * @param `result`
 * @par Sample
 * @code
 *     int k = 24，res = 0; // k = 0b11000
 *     populationCount(k, res);
 *     assert(res == 2);
 * @endcode
 */
#define populationCount(source, result) \
    __asm__ volatile("popcnt %1, %0;"   \
                     : "=r"(result)     \
                     : "r"(source))

const unsigned long long MASK = 0x007f7f7f7f7f7f7f;
const unsigned long long IN_RADIUS_1[64] = {
    0x302,
    0x705,
    0xe0a,
    0x1c14,
    0x3828,
    0x7050,
    0x6020,
    0,

    0x30203,
    0x70507,
    0xe0a0e,
    0x1c141c,
    0x382838,
    0x705070,
    0x602060,
    0,

    0x3020300,
    0x7050700,
    0xe0a0e00,
    0x1c141c00,
    0x38283800,
    0x70507000,
    0x60206000,
    0,

    0x302030000,
    0x705070000,
    0xe0a0e0000,
    0x1c141c0000,
    0x3828380000,
    0x7050700000,
    0x6020600000,
    0,

    0x30203000000,
    0x70507000000,
    0xe0a0e000000,
    0x1c141c000000,
    0x382838000000,
    0x705070000000,
    0x602060000000,
    0,

    0x3020300000000,
    0x7050700000000,
    0xe0a0e00000000,
    0x1c141c00000000,
    0x38283800000000,
    0x70507000000000,
    0x60206000000000,
    0,

    0x2030000000000,
    0x5070000000000,
    0xa0e0000000000,
    0x141c0000000000,
    0x28380000000000,
    0x50700000000000,
    0x20600000000000,
    0,

    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0};

const unsigned long long IN_RADIUS_2[64] = {
    0x70404,
    0xf0808,
    0x1f1111,
    0x3e2222,
    0x7c4444,
    0x780808,
    0x701010,
    0,

    0x7040404,
    0xf080808,
    0x1f111111,
    0x3e222222,
    0x7c444444,
    0x78080808,
    0x70101010,
    0,

    0x704040407,
    0xf0808080f,
    0x1f1111111f,
    0x3e2222223e,
    0x7c4444447c,
    0x7808080878,
    0x7010101070,
    0,

    0x70404040700,
    0xf0808080f00,
    0x1f1111111f00,
    0x3e2222223e00,
    0x7c4444447c00,
    0x780808087800,
    0x701010107000,
    0,

    0x7040404070000,
    0xf0808080f0000,
    0x1f1111111f0000,
    0x3e2222223e0000,
    0x7c4444447c0000,
    0x78080808780000,
    0x70101010700000,
    0,

    0x4040407000000,
    0x808080f000000,
    0x1111111f000000,
    0x2222223e000000,
    0x4444447c000000,
    0x8080878000000,
    0x10101070000000,
    0,

    0x4040700000000,
    0x8080f00000000,
    0x11111f00000000,
    0x22223e00000000,
    0x44447c00000000,
    0x8087800000000,
    0x10107000000000,
    0,

    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0};

typedef struct
{
    int player;
    unsigned long long table[2];
} ChessBoard;

void move(ChessBoard *this, int start, int destination)
{
    this->table[this->player] ^= (IN_RADIUS_2[destination] & (1ULL << start)) | (1ULL << destination);
    unsigned long long change = this->table[this->player ^ 1] & IN_RADIUS_1[destination];
    this->table[this->player] |= change;
    this->table[this->player ^ 1] ^= change;
}

/**
 * @brief 对当前棋盘估值。
 * @param `this` :
 */
int evaluate(const ChessBoard *this)
{
    long long black = 0, white = 0;
    populationCount(this->table[BLACK], black);
    populationCount(this->table[WHITE], white);
    return white - black;
}

void initChessBoard(ChessBoard *this)
{
    this->player = BLACK;
    this->table[0] = 0x40000000000001;
    this->table[1] = 0x1000000000040;
}

typedef struct
{
    int start, destination, value;
} Operation;

void initOperation(Operation *this)
{
    this->start = 0;
    this->destination = 0;
    this->value = 0;
}

void initOperation_i32_i32_i32(Operation *this, int s, int des, int v)
{
    this->start = s;
    this->destination = des;
    this->value = v;
}

void search(const ChessBoard *chessBoard, Operation *answer);

int main(void)
{
    ChessBoard mainChessBoard;
    initChessBoard(&mainChessBoard);
    Operation answer;
    initOperation(&answer);
    int turnNumber = 0;
    scanf("%d", &turnNumber);
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    for (int i = 0; i < turnNumber - 1; ++i)
    {
        scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
        if (x0 >= 0)
        {
            move(&mainChessBoard, x0 << 3 | y0, x1 << 3 | y1);
            mainChessBoard.player ^= 1;
        }
        scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
        move(&mainChessBoard, x0 << 3 | y0, x1 << 3 | y1);
        mainChessBoard.player ^= 1;
    }
    scanf("%d %d %d %d", &y0, &x0, &y1, &x1);
    if (x0 >= 0)
    {
        move(&mainChessBoard, x0 << 3 | y0, x1 << 3 | y1);
        mainChessBoard.player ^= 1;
    }
    search(&mainChessBoard, &answer);
    printf("%d %d %d %d",
           answer.start & 7,
           answer.start >> 3,
           answer.destination & 7,
           answer.destination >> 3);
    return 0;
}

void search(const ChessBoard *chessBoard, Operation *answer)
{
    unsigned long long empty = (chessBoard->table[BLACK] | chessBoard->table[WHITE]) ^ MASK;
    unsigned long long starts = 0, start = 0, destination = 0;
    ChessBoard tempChessboard;
    initChessBoard(&tempChessboard);
    Operation tempOperation;
    initOperation(&tempOperation);
    if (chessBoard->player == WHITE)
    {
        answer->value = -64;
        while (empty)
        {
            countTrailingZero(empty, destination);
            starts = IN_RADIUS_1[destination] & chessBoard->table[chessBoard->player];
            if (starts)
            {
                countTrailingZero(starts, start);
                tempChessboard = *chessBoard;
                move(&tempChessboard, start, destination);
                initOperation_i32_i32_i32(&tempOperation, start, destination, evaluate(&tempChessboard));
                if (tempOperation.value > answer->value)
                    *answer = tempOperation;
            }
            starts = IN_RADIUS_2[destination] & chessBoard->table[chessBoard->player];
            while (starts)
            {
                countTrailingZero(starts, start);
                tempChessboard = *chessBoard;
                move(&tempChessboard, start, destination);
                initOperation_i32_i32_i32(&tempOperation, start, destination, evaluate(&tempChessboard));
                if (tempOperation.value > answer->value)
                    *answer = tempOperation;
                disableBit(starts, start);
            }
            disableBit(empty, destination);
        }
    }
    else
    {
        answer->value = 64;
        while (empty)
        {
            countTrailingZero(empty, destination);
            starts = IN_RADIUS_1[destination] & chessBoard->table[chessBoard->player];
            if (starts)
            {
                countTrailingZero(starts, start);
                tempChessboard = *chessBoard;
                move(&tempChessboard, start, destination);
                initOperation_i32_i32_i32(&tempOperation, start, destination, evaluate(&tempChessboard));
                if (tempOperation.value < answer->value)
                    *answer = tempOperation;
            }
            starts = IN_RADIUS_2[destination] & chessBoard->table[chessBoard->player];
            while (starts)
            {
                countTrailingZero(starts, start);
                tempChessboard = *chessBoard;
                move(&tempChessboard, start, destination);
                initOperation_i32_i32_i32(&tempOperation, start, destination, evaluate(&tempChessboard));
                if (tempOperation.value < answer->value)
                    *answer = tempOperation;
                disableBit(starts, start);
            }
            disableBit(empty, destination);
        }
    }
}
