#include <QApplication>
#include <QTreeView>
#include <QJsonDocument>
#include <QFile>
#include <QHeaderView>
#include "QVariantTreeModel/QVariantTreeModel.h"


auto loadJSON(const QString& filename) {
    auto file = QFile{filename};
    file.open(QIODevice::ReadOnly);
    return QJsonDocument::fromJson(file.readAll()).toVariant();
}

auto saveJSON(const QVariant& variant, const QString& filename) {
    auto file = QFile{filename};
    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument::fromVariant(variant).toJson());
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QVariantTreeModel model;
    model.loadVariant(loadJSON("../../in.json"));
    QTreeView treeView;
    treeView.header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView.setModel(&model);
    treeView.show();
    auto ret = a.exec();
    saveJSON(model.saveVariant(), "../../out.json");
    return ret;
}
