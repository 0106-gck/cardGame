#include "UndoModel.h"

// 添加一条操作到历史栈
void UndoModel::pushOp(const CardMoveOp& operation)
{
    _history.push(operation);
}

// 弹出最近一条操作
CardMoveOp UndoModel::popOp()
{
    if (_history.empty())
        return CardMoveOp(); // 返回默认操作

    CardMoveOp lastOp = _history.top();
    _history.pop();
    return lastOp;
}

// 判断是否还有可撤销操作
bool UndoModel::canUndo() const
{
    return !_history.empty();
}

// 清空操作历史
void UndoModel::clear()
{
    while (!_history.empty()) _history.pop();
}

// 获取只读历史栈
const std::stack<CardMoveOp>& UndoModel::getHistory() const
{
    return _history;
}
