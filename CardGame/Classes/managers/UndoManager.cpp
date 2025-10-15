#include "UndoManager.h"

// ���캯������ UndoModel ���ݴ洢
UndoManager::UndoManager(UndoModel* undoModel)
    : _undoModel(undoModel)
{
}

// ��¼һ���ƶ�����
void UndoManager::recordMove(const CardMoveOp& operation)
{
    _undoModel->pushOp(operation);
}

// �������һ�β���
CardMoveOp UndoManager::undo()
{
    return _undoModel->popOp();
}

// �ж��Ƿ�ɳ���
bool UndoManager::canUndo() const
{
    return _undoModel->canUndo();
}

// ��ճ�����ʷ
void UndoManager::clear()
{
    _undoModel->clear();
}

// ��ȡֻ��������ʷ
const std::stack<CardMoveOp>& UndoManager::getHistory() const
{
    return _undoModel->getHistory();
}
