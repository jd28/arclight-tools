#include "resourcemodelnode.h"

ResourceModelNode::ResourceModelNode(ResourceModelNode *parent)
    : parent_{parent}
{
}

void ResourceModelNode::appendChild(ResourceModelNode *child)
{
    children_.emplace_back(child);
}
