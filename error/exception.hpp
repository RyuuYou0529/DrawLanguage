//
// Created by RyuuYou on 2020/12/10.
//

#ifndef GRAPH_EXCEPTION_HPP
#define GRAPH_EXCEPTION_HPP
class GException: public exception{
private:
    string Info;
    unsigned int LineNo;
public:
    GException(string theInfo,unsigned int theLine):Info(theInfo),LineNo(theLine){}
    void PrintMessage(){
        cout<<"Error-Line_"<<LineNo<<": "<<Info<<endl;
    }
};
class SyntaxError: public GException{
public:
    SyntaxError(unsigned int theLine):GException("错误记号。",theLine){}
};
class UnexpectedSyntax: public GException{
public:
    UnexpectedSyntax(unsigned int theLine):GException("非预期记号。",theLine){}
};
class DivisorIsZero: public GException{
public:
    DivisorIsZero(unsigned int theLine):GException("除数为0。",theLine){}
};
#endif //GRAPH_EXCEPTION_HPP
