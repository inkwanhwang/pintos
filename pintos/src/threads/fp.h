// input: int형은 n, fp형은 x, y로 표기
// 핀토스 공식 문서의 Appendix B.6에 따름
#include <stdint.h>
#define f (1 << 14)

int convert_n_to_fp (int);
int convert_x_to_int_round_down (int);
int convert_x_to_int_round_off (int);
int add_fp_fp (int, int);
int sub_fp_fp (int, int);
int add_fp_int (int, int);
int sub_fp_int (int, int);
int mul_fp_fp (int, int);
int mul_fp_int (int, int);
int div_fp_by_fp (int, int);
int div_fp_by_int (int, int);

int convert_n_to_fp (int n)
{
    return n*f;
}

int convert_x_to_int_round_down (int x) // 정수부 끝자리에서 버림
{
    return x/f;
}

int convert_x_to_int_round_off (int x) // 정수부 끝자리에서 반올림
{
    if (x >= 0)
    {
        return (x + f/2) / f;
    }
    else
    {
        return (x - f/2) / f;
    }
}

int add_fp_fp (int x, int y)
{
    return x+y;
}

int sub_fp_fp (int x, int y)
{
    return x-y;
}

int add_fp_int (int x, int n)
{
    return x + n*f;
}

int sub_fp_int (int x, int n)
{
    return x - n*f;
}

int mul_fp_fp (int x, int y)
{
    return ((int64_t)x *y)/f;
}

int mul_fp_int (int x, int n)
{
    return x*n;
}

int div_fp_by_fp (int x, int y)
{
    return ((int64_t)x*f)/y;
}

int div_fp_by_int (int x, int n)
{
    return x/n;
}