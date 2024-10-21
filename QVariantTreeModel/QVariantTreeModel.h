#pragma once

#include <QAbstractItemModel>
#include <memory>

class QVariantTreeItem;

class QVariantTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit QVariantTreeModel(QObject* parent = nullptr);
    ~QVariantTreeModel() override;
    void loadVariant(QVariant variant);
    QVariant saveVariant() const;
private:
    std::unique_ptr<QVariantTreeItem> mRoot;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
