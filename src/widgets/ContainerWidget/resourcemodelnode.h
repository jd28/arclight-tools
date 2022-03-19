#ifndef EXPOREMODELITEM_H
#define EXPOREMODELITEM_H

#include "../util/restypeicons.h"

#include <nw/resources/Directory.hpp>
#include <nw/resources/Erf.hpp>
#include <nw/resources/Key.hpp>

#include <QIcon>
#include <QString>
#include <QVariant>

#include <filesystem>
#include <memory>
#include <vector>

class ResourceModelNode {
public:
    explicit ResourceModelNode(ResourceModelNode* parent = nullptr);
    virtual ~ResourceModelNode() { }

    void appendChild(ResourceModelNode* child);
    virtual ResourceModelNode* child(int row) { return children_[static_cast<size_t>(row)].get(); }
    virtual int childCount() const { return static_cast<int>(children_.size()); }
    virtual int columnCount() const { return 3; }
    virtual QVariant data(int col = 0) const { return {}; }
    virtual QVariant icon() const { return {}; }
    virtual int row() const { return 0; }
    ResourceModelNode* parent() { return parent_; }

private:
    std::vector<std::unique_ptr<ResourceModelNode>> children_;
    ResourceModelNode* parent_ = nullptr;
};

class ResourceNode : public ResourceModelNode {
public:
    ResourceNode() = default;
    ResourceNode(nw::ResourceDescriptor res, ResourceModelNode* parent)
        : ResourceModelNode(parent)
        , res_{res}
        , filename_cache_{QString::fromStdString(res.name.filename())}
    {
    }

    virtual QVariant data(int col = 0) const override
    {
        switch (col) {
        default:
            return {};
        case 0:
            return filename_cache_;
        case 1:
            return static_cast<int>(res_.size);
        }
    }

    virtual QVariant icon() const override { return restypeToIcon(res_.name.type); }

    nw::ResourceDescriptor res_;
    QString filename_cache_;
};

class CategoryNode : public ResourceModelNode {
public:
    CategoryNode(QString category, ResourceModelNode* parent);
    QString category_;

    virtual QVariant data(int col = 0) const override { return category_; }
};

class DirectoryNode : public ResourceModelNode {
public:
    DirectoryNode(QString name, std::filesystem::path path, ResourceModelNode* parent = nullptr);
    virtual QVariant data(int col = 0) const override { return name_; }
    virtual QVariant icon() const override;

private:
    QString name_;
    nw::Directory dir_;
    std::filesystem::path path_;
};

class ErfNode : public ResourceModelNode {
public:
    ErfNode(std::filesystem::path path, ResourceModelNode* parent = nullptr);
    virtual QVariant data(int col = 0) const override { return QString::fromStdString(path_.string()); }

    std::filesystem::path path_;
    nw::Erf erf_;
};

class KeyNode : public ResourceModelNode {
public:
    KeyNode(QString path, ResourceModelNode* parent = nullptr);
    virtual QVariant data(int col = 0) const override { return path_; }

    QString path_;
    nw::Key key_;
};

class NWSyncNode : public ResourceModelNode {
public:
    NWSyncNode(std::filesystem::path path, ResourceModelNode* parent);
    virtual QVariant data(int col = 0) const override { return QString::fromStdString(path_.string()); }

private:
    std::filesystem::path path_;
};

struct NWNFileSystemModel : public ResourceModelNode {
    NWNFileSystemModel(std::filesystem::path install, std::filesystem::path user);

    std::filesystem::path install_;
    std::filesystem::path user_;
};

#endif // EXPOREMODELITEM_H
