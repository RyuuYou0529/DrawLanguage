//
// Created by RyuuYou on 2020/12/9.
//

//
// Created by RyuuYou on 2020/12/4.
//
#include "../lexer/scanner.hpp"
#include "../parser/parser.hpp"
#include "../semantics/semantics.hpp"
#include <iostream>
#include <QApplication>
using namespace std;
int main(int argc,char *argv[]){
    if(argc<2){                 //参数有问题
        cout<<"Please input Source File!"<<endl;
        return -1;
    }
    QApplication application(argc,argv);
    Semantics semantics(argv[1]);
    try {
        semantics.Semantics_exec();
        //semantics.show();
        semantics.showMaximized();
    }catch (GException GE){
        GE.PrintMessage();
        exit(-1);
    }
    return QApplication::exec();
}
