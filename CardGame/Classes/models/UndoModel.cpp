#include "UndoModel.h"

// ���һ����������ʷջ
void UndoModel::pushOp(const CardMoveOp& operation)
{
    _history.push(operation);
}

// �������һ������
CardMoveOp UndoModel::popOp()
{
    if (_history.empty())
        return CardMoveOp(); // ����Ĭ�ϲ���

    CardMoveOp lastOp = _history.top();
    _history.pop();
    return lastOp;
}

// �ж��Ƿ��пɳ�������
bool UndoModel::canUndo() const
{
    return !_history.empty();
}

// ��ղ�����ʷ
void UndoModel::clear()
{
    while (!_history.empty()) _history.pop();
}

// ��ȡֻ����ʷջ
const std::stack<CardMoveOp>& UndoModel::getHistory() const
{
    return _history;
}
