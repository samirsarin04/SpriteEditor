#include "model.h"

#include<iostream>

Model::Model(QObject *parent)
    : QObject{parent}
{}

void Model::canvasClick(int x, int y, bool click){
  std::cout << "CLICK ON CANVAS" << std::endl;
}
