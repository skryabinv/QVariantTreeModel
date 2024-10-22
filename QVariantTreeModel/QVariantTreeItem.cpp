#include "QVariantTreeItem.h"

QVariantTreeItem::QVariantTreeItem(QVariant key, QVariant value,
                                   int row,
                                   QVariantTreeItem* parent)
    : mKey{std::move(key)},
    mRow{row},
    mParent{parent} {
    setValue(std::move(value));
}

auto QVariantTreeItem::getChild(int index) const -> QVariantTreeItem* {
    if(index >= 0 && index < mChildrens.size()) {
        return mChildrens[index].get();
    }
    return nullptr;
}

auto QVariantTreeItem::toVariant() const -> QVariant {
    switch(mType) {
    case Type::Map:
        return toMap();
    case Type::List:
        return toList();
    case Type::Value:
        return mValue;
    }
    return {};
}

auto QVariantTreeItem::tryUpdateValue(QVariant value) -> bool {    
    auto canUpdate = tryConvert(value);
    if(canUpdate) {
        std::swap(mValue, value);        
    }
    return canUpdate;
}

auto QVariantTreeItem::getDataDisplayRole(int column) const -> QVariant {
    if(column == 0) {
        return mKey;
    }
    if(column == 1) {
        return mValue;
    }
    return {};
}

bool QVariantTreeItem::tryConvert(QVariant& value) const {
    return value.convert(mValue.metaType());
}

void QVariantTreeItem::addChild(QVariant key, QVariant value) {
    auto child = std::make_unique<QVariantTreeItem>(std::move(key),
                                                    std::move(value),
                                                    mChildrens.size(), this);
    mChildrens.emplace_back(std::move(child));
}

void QVariantTreeItem::setValue(QVariant value) {    
    switch(value.typeId()) {
    case QMetaType::QVariantMap:
        fromMap(value.toMap());
        break;
    case QMetaType::QVariantList:
        fromList(value.toList());
        break;
    default:
        mType = Type::Value;
        mValue = std::move(value);
    }
}

void QVariantTreeItem::fromMap(QVariantMap map) {
    mType = Type::Map;
    for(auto it = map.keyValueBegin(); it != map.keyValueEnd(); ++it) {
        addChild(std::move(it->first), std::move(it->second));
    }
}

void QVariantTreeItem::fromList(QVariantList list) {
    mType = Type::List;
    for(qsizetype i = 0; i < list.size(); ++i) {
        addChild(i, std::move(list[i]));
    }
}

QVariant QVariantTreeItem::toMap() const {
    assert(mType == Type::Map);
    auto result = QVariantMap{};
    for(const auto& children : mChildrens) {
        result[children->mKey.toString()] = children->toVariant();
    }
    return result;
}

QVariant QVariantTreeItem::toList() const {
    assert(mType == Type::List);
    auto result = QVariantList{};
    for(const auto& children : mChildrens) {
        result << children->toVariant();
    }
    return result;
}


