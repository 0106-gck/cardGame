#include "UndoManager.h"

// 构造函数，绑定 UndoModel 数据存储
UndoManager::UndoManager(UndoModel* undoModel)
    : _undoModel(undoModel)
{
}

// 记录一次移动操作
void UndoManager::recordMove(const CardMoveOp& operation)
{
    _undoModel->pushOp(operation);
}

// 撤销最近一次操作
CardMoveOp UndoManager::undo()
{
    return _undoModel->popOp();
}

// 判断是否可撤销
bool UndoManager::canUndo() const
{
    return _undoModel->canUndo();
}

// 清空撤销历史
void UndoManager::clear()
{
    _undoModel->clear();
}

// 获取只读撤销历史
const std::stack<CardMoveOp>& UndoManager::getHistory() const
{
    return _undoModel->getHistory();
}
