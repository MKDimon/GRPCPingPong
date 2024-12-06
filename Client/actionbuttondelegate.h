#ifndef ACTIONBUTTONDELEGATE_H
#define ACTIONBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

/**
 * Класс для кнопки в таблице
 */
class ActionButtonDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ActionButtonDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void actionClicked(const QModelIndex &index);
};


#endif // ACTIONBUTTONDELEGATE_H
