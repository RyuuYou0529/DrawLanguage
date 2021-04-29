//
// Created by RyuuYou on 2020/12/9.
//

#include "semantics.hpp"

void Semantics::Semantics_exec() {
    theParser = new Parser;
    theParser->Parser_exec(this->SrcFilePtr);
    theParser->PrintSyntaxTree();
}

void Semantics::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    StatementNode *theStat;
    ProgramNode *theProg = this->theParser->Root;
    QPen pen(Qt::black,1);
    painter.setPen(pen);
    while (theProg != nullptr) {
        theStat = theProg->Statement;
        switch (theStat->Type) {
            case ORIGIN:
                double origin_res[2];
                theStat->Calculate(origin_res);
                theParser->Origin_X = origin_res[0];
                theParser->Origin_Y = origin_res[1];
                break;
            case SCALE:
                double scale_res[2];
                theStat->Calculate(scale_res);
                theParser->Scale_X = scale_res[0];
                theParser->Scale_Y = scale_res[1];
                break;
            case ROT:
                double rot_res;
                theStat->Calculate(&rot_res);
                theParser->Rot_Angle = rot_res;
                break;
            case FOR:
                double for_res[3];
                for_res[2]=1;
                while ((bool) for_res[2]) {
                    theStat->Calculate(for_res);
                    for_res[0]*=theParser->Scale_X;
                    for_res[1]*=theParser->Scale_Y;
                    double X = theParser->Origin_X + (for_res[0] * cos(theParser->Rot_Angle)
                                                      + for_res[1] * sin(theParser->Rot_Angle)) ;
                    double Y = theParser->Origin_Y + (for_res[1] * cos(theParser->Rot_Angle)
                                                      - for_res[0] * sin(theParser->Rot_Angle));
                    painter.drawPoint((int)X, (int)Y);
                }
                break;
            case COLOR:
                double color_res[3];
                theStat->Calculate(color_res);
                pen.setColor(QColor((int)color_res[0],(int)color_res[1],(int)color_res[2]));
                painter.setPen(pen);
                break;
            case WIDTH:
                double width_res;
                theStat->Calculate(&width_res);
                pen.setWidth((int)width_res);
                painter.setPen(pen);
                break;
        }
        theProg = theProg->Program;
    }
    this->setUpdatesEnabled(false);
}

