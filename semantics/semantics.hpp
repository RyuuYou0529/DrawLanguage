//
// Created by RyuuYou on 2020/12/9.
//

#ifndef GRAPH_SEMANTICS_HPP
#define GRAPH_SEMANTICS_HPP
#include "../parser/parser.hpp"
#include <QWidget>
#include <QPainter>
#include <QTimer>
class Semantics:public QWidget {
protected:
    void paintEvent(QPaintEvent *event) override;
    Parser *theParser{};
    char *SrcFilePtr;
public:
    explicit Semantics(char *file):SrcFilePtr(file){}
    void Semantics_exec();
};
#endif //GRAPH_SEMANTICS_HPP
