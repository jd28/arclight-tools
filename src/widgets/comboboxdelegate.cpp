#include "comboboxdelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QStringList labels, QObject* parent)
    : QStyledItemDelegate{parent}
    , labels_{labels}
{
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
    QComboBox* combo_box = new QComboBox(parent);
    for (int i = 0; i < labels_.count(); i++) {
        combo_box->addItem(labels_[i]);
    }
    return combo_box;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString label = index.model()->data(index, Qt::EditRole).toString();
    QComboBox* combobox = static_cast<QComboBox*>(editor);

    for (int i = 0; i < labels_.count(); i++) {
        if (label == labels_[i]) {
            combobox->setCurrentIndex(i);
            break;
        }
    }

    connect(combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        [this, combobox]() {
            emit const_cast<ComboBoxDelegate*>(this)->commitData(combobox);
        });
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combo_box = static_cast<QComboBox*>(editor);
    model->setData(index, combo_box->currentText(), Qt::EditRole);
}

QSize ComboBoxDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);
    QComboBox comboBox;
    comboBox.addItem("int");
    comboBox.addItem("string");
    comboBox.addItem("float");
    return comboBox.sizeHint();
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}
