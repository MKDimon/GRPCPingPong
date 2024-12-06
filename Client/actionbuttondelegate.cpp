#include "actionbuttondelegate.h"


ActionButtonDelegate::ActionButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void ActionButtonDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {
    if (index.column() == 3) {
        QPushButton button(index.data().toString());
        button.resize(option.rect.size());
        QPixmap pixmap(button.size());
        button.render(&pixmap);
        painter->drawPixmap(option.rect, pixmap);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool ActionButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (index.column() != 3)
        return false;

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (option.rect.contains(mouseEvent->pos())) {
            emit actionClicked(index);
            return true;
        }
    }
    return false;
}
