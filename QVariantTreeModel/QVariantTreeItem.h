#pragma once

#include <vector>
#include <memory>
#include <QVariant>

class QVariantTreeItem {
public:
    QVariantTreeItem(QVariant key, QVariant value, int row, QVariantTreeItem* parent = nullptr);
    auto getChild(int index) const -> QVariantTreeItem*;
    auto toVariant() const -> QVariant;
    auto tryUpdateValue(QVariant value) -> bool;
    auto getDataDisplayRole(int column) const -> QVariant;
    auto getParent() const {
        return mParent;
    }
    auto getRow() const {
        return mRow;
    }
    auto childCount() const {
        return mChildrens.size();
    }
private:
    bool tryConvert(QVariant& value) const;
    void addChild(QVariant key, QVariant value);
    void setValue(QVariant value);
    void fromMap(QVariantMap map);
    void fromList(QVariantList list);
    QVariant toMap() const;
    QVariant toList() const;
    enum class Type {
        Map,
        List,
        Value
    };
    Type mType{Type::Map};
    QVariant mKey;
    QVariant mValue;
    int mRow{};
    QVariantTreeItem* mParent{};
    std::vector<std::unique_ptr<QVariantTreeItem>> mChildrens;
};

