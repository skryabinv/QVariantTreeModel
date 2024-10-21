#include "QVariantTreeModel.h"
#include "QVariantTreeItem.h"

inline auto getItemPtr(const QModelIndex& index) {
    return static_cast<QVariantTreeItem*>(index.internalPointer());
}

QVariantTreeModel::QVariantTreeModel(QObject* parent)
    : QAbstractItemModel{parent} {
}

QVariantTreeModel::~QVariantTreeModel() = default;

QVariant QVariantTreeModel::saveVariant() const {
    return mRoot->toVariant();
}

void QVariantTreeModel::loadVariant(QVariant variant) {
    beginResetModel();
    mRoot = std::make_unique<QVariantTreeItem>("", std::move(variant), 0, nullptr);
    endResetModel();
}

QModelIndex QVariantTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if(!hasIndex(row, column, parent)) {
        return {};
    }
    auto parentItem = parent.isValid()
            ? getItemPtr(parent)
            : mRoot.get();
    if(auto childItem = parentItem->getChild(row); childItem != nullptr) {
        return createIndex(row, column, childItem);
    }    
    return {};
}

QModelIndex QVariantTreeModel::parent(const QModelIndex& child) const {
    if(!child.isValid()) {
        return {};
    }
    auto childItem = getItemPtr(child);
    auto parentItem = childItem->getParent();
    if(parentItem == mRoot.get()) {
        return {};
    }
    return createIndex(parentItem->getRow(), 0, parentItem);
}

int QVariantTreeModel::rowCount(const QModelIndex& parent) const {
    if(parent.column() > 0) {
        return 0;
    }
    auto parentItem = parent.isValid()
            ? getItemPtr(parent)
            : mRoot.get();
    return parentItem->childCount();
}

int QVariantTreeModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant QVariantTreeModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid()) {
        return {};
    }    
    switch(role) {
        case Qt::DisplayRole:
            return getItemPtr(index)->getDataDisplayRole(index.column());
    }
    return {};
}


Qt::ItemFlags QVariantTreeModel::flags(const QModelIndex& index) const {
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }
    auto flags = QAbstractItemModel::flags(index);
    return index.column() == 1 ? flags | Qt::ItemIsEditable : flags;
}


bool QVariantTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if(!index.isValid() || role != Qt::EditRole) {
        return false;
    }
    return getItemPtr(index)->tryUpdateValue(value);
}


QVariant QVariantTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return {};
    }
    if(section == 0) {
        return tr("Key");
    }
    if(section == 1) {
        return tr("Value");
    }
    return {};
}
