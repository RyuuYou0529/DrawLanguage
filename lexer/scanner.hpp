//
// Created by RyuuYou on 2020/12/2.
//

#ifndef GRAPH_SCANNER_HPP
#define GRAPH_SCANNER_HPP

#include<string>
#include<cstdio>
#include<cstdlib>
#include<cctype>
#include<cstdarg>
#include<cmath>

using namespace std;
//符号类型
enum Token_Type{
    PROGRAM,STATEMENT,                          //保留字
    ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,  //保留字
    COLOR,WIDTH,                                //保留字
    T,                                          //参数
    SEMICO,L_BRACKET,R_BRACKET,COMMA,           //分隔符号
    PLUS,MINUS,MUL,DIV,POWER,                   //运算符
    FUNC,                                       //函数
    CONST_ID,                                   //常数
    NONTOKEN,                                   //空记号
    ERRTOKEN                                    //错误记号
};
//返回值和参数均为double类型的函数指针
typedef double(*MathFuncPtr)(double);
//记号结构
struct Token{
    Token_Type type;                //记号类型
    const char *lexeme;             //构成记号的字符串
    double value;                   //记号的常数值
    double (*FuncPtr)(double);      //记号的函数指针
};
//符号表内容
static  Token TokenTab[]={
        {CONST_ID,"PI",3.1415926,nullptr},      //圆周率
        {CONST_ID,"E",2.71828,nullptr},         //自然底数
        {T,"T",0.0,nullptr},                    //参数
        {FUNC,"SIN",0.0,sin},                           //正弦函数
        {FUNC,"COS",0.0,cos},                           //余弦函数
        {FUNC,"TAN",0.0,tan},                           //正切函数
        {FUNC,"LN",0.0,log},                            //log函数
        {FUNC,"EXP",0.0,exp},                           //e指数函数
        {FUNC,"SQRT",0.0,sqrt},                         //开方函数
        {ORIGIN,"ORIGIN",0.0,nullptr},         //保留字ORIGIN
        {SCALE,"SCALE",0.0,nullptr},           //保留字SCALE
        {ROT,"ROT",0.0,nullptr},               //保留字ROT
        {IS,"IS",0.0,nullptr},                 //保留字IS
        {FOR,"FOR",0.0,nullptr},               //保留字FOR
        {FROM,"FROM",0.0,nullptr},             //保留字FROM
        {TO,"TO",0.0,nullptr},                 //保留字TO
        {STEP,"STEP",0.0,nullptr},             //保留字STEP
        {DRAW,"DRAW",0.0,nullptr},             //保留字DRAW
        {COLOR,"COLOR",0.0, nullptr},          //保留字COLOR
        {WIDTH,"WIDTH",0.0, nullptr}           //保留字WIDTH
};
#define TOKEN_LEN 100                   //记号最大长度

class Lexer{
private:
    FILE *InFile;                    //源文件
    char TokenBuffer[TOKEN_LEN];     //记号缓冲区
private:
    char GetChar();
    void BackChar(char Char);
    void AddCharTokenString(char Char);
    void EmptyTokenString();
    Token JudgeKeyToken(const char* IDString);
public:
    unsigned int LineNo;                //源文件中的记号的行号
    int InitScanner(const char*);       //初始化词法分析器
    Token GetToken();                   //获取记号
    void CloseScanner();                //关闭词法分析器
};
#endif //GRAPH_SCANNER_HPP
