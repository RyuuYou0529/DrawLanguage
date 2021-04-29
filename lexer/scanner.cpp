//
// Created by RyuuYou on 2020/12/2.
//

#include "scanner.hpp"

//初始化词法分析器
int Lexer::InitScanner(const char* FileName){
    LineNo=1;
    InFile=fopen(FileName,"r");
    if(InFile!=nullptr)
        return 1;
    else
        return 0;
}

//关闭词法分析器
void Lexer::CloseScanner(){
    if(InFile!=nullptr)
        fclose(InFile);
}

//从源文件中读取一个字符
char Lexer::GetChar(){
    int Char=getc(InFile);
    return toupper(Char);//返回大改字符的大写形式
}

//把预读的字符退回到源文件中
void Lexer::BackChar(char Char){
    if(Char!=EOF)
        ungetc(Char,InFile);
}

//把读到的记号加入到记号缓冲区
void Lexer::AddCharTokenString(char Char){
    int TokenLength=strlen(TokenBuffer);
    if(TokenLength+1>=sizeof(TokenBuffer))
        return;
    TokenBuffer[TokenLength]=Char;
    TokenBuffer[TokenLength+1]='\0';
}

//清空记号缓冲区
void Lexer::EmptyTokenString(){
    memset(TokenBuffer,0,TOKEN_LEN);
}

//判断预读的记号字符串是否在符号表中
Token Lexer::JudgeKeyToken(const char* IDString){
    int loop;
    for(loop=0;loop<sizeof(TokenTab)/sizeof(TokenTab[0]);++loop){
        if(strcmp(TokenTab[loop].lexeme,IDString)==0)
            return TokenTab[loop];
    }
    return {ERRTOKEN, nullptr,0.0, nullptr};
}

//获取一个记号
Token Lexer::GetToken(){
    Token token;
    char Char;
    memset(&token,0,sizeof(Token));
    EmptyTokenString();
    token.lexeme=TokenBuffer;

    //过滤空白符，并判断是否到达源文件结尾
    for(;;){
        Char=GetChar();
        if(Char==EOF){              //判断为源文件结尾
            token.type=NONTOKEN;
            return token;
        }
        if(Char=='\n')              //判断为换行符，行号+1
            LineNo++;
        if(!isspace(Char))          //遇到非空白符，退出
            break;
    }
    AddCharTokenString(Char);       //将刚才读到的非空白符加入到记号缓冲区
    if(isalpha(Char)){              //若"第一个非空白字符"为字母，则进行函数、关键字、PI、E等判断
        for(;;){                    //继续读取后续字符
            Char=GetChar();
            if(isalnum(Char))       //若后续字符为数字或字母，则加入到缓冲区，否则退出
                AddCharTokenString(Char);
            else
                break;
        }
        BackChar(Char);                     //将刚才读到到非数字或字母字符，回退到源文件中
        token=JudgeKeyToken(TokenBuffer);   //判断当前缓冲区的记号字符串种类
        token.lexeme=TokenBuffer;
        return token;
    } else if(isdigit(Char)){       //若"第一个非空白字符"为数字，则进行常量判断
        for(;;){                    //继续读取后续字符
            Char=GetChar();
            if(isdigit(Char))       //若后续字符为数字则加入到缓冲区，否则退出
                AddCharTokenString(Char);
            else
                break;
        }
        if(Char=='.'){                  //判断常量的小数部分
            AddCharTokenString(Char);   //把小数点加入到缓冲区
            for(;;){
                Char=GetChar();
                if(isdigit(Char))
                    AddCharTokenString(Char);
                else
                    break;
            }
        }
        BackChar(Char);                 //回退最后一个非数字字符
        token.type=CONST_ID;
        token.value=atof(TokenBuffer);  //将缓冲区的字符转为double类型的常量
        return token;
    } else{                                         //其他情况，对运算符或分隔符的判断
        switch (Char) {
            case ';':token.type=SEMICO;break;
            case '(':token.type=L_BRACKET;break;
            case ')':token.type=R_BRACKET;break;
            case ',':token.type=COMMA;break;
            case '+':token.type=PLUS;break;
            case '-':                               //判断为一元减运算
                Char=GetChar();
                if(Char=='-'){                      //判断为注释
                    while (Char!='\n'&&Char!=EOF)
                        Char=GetChar();
                    BackChar(Char);                 //回退换行符和文件终结符
                    return GetToken();              //递归判断后续记号
                } else{                             //后续无一元减运算
                    BackChar(Char);                 //回退预读符号
                    token.type=MINUS;
                    break;
                }
            case '/':                               //判断为除法运算
                Char=GetChar();                     //获取后续字符，判断是否为除法
                if(Char=='/'){                      //判断为注释
                    while (Char!='\n'&&Char!=EOF)
                        Char=GetChar();
                    BackChar(Char);
                    return GetToken();
                } else{                             //后续无除法运算，回退预读符号
                    BackChar(Char);
                    token.type=DIV;
                    break;
                }
            case '*':                       //判断是否为乘法运算
                Char=GetChar();             //读取后续字符
                if(Char=='*'){              //判断为指数运算
                   token.type=POWER;
                    break;
                } else{                     //判断为乘法元算，回退预读字符
                    BackChar(Char);
                    token.type=MUL;
                    break;
                }
            default:token.type=ERRTOKEN;break;  //其他情况则为出错
        }
        return token;
    }
}