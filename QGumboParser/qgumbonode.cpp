#include <cstring>
#include <QString>
#include <QDebug>
#include <QStringList>
#include "qgumbonode.h"
#include "qgumboattribute.h"

namespace {

const char* const ID_ATTRIBUTE 		= u8"id";
const char* const CLASS_ATTRIBUTE 	= u8"class";

template<typename TFunctor>
bool iterateTree(GumboNode* node, TFunctor& functor) {
    if (!node || node->type != GUMBO_NODE_ELEMENT)
        return false;

    if (functor(node))
        return true;

    for (uint i = 0; i < node->v.element.children.length; ++i) {
        if (iterateTree(static_cast<GumboNode*>(node->v.element.children.data[i]), functor))
            return true;
    }

    return false;
}

} /* namespace */

QGumboNode::QGumboNode()
{
}

QGumboNode::QGumboNode(GumboNode* node) :
    ptr_(node)
{
    if (!ptr_)
        throw std::runtime_error("can't create Node from nullptr");
}

QGumboNodes QGumboNode::getElementById(const QString& nodeId) const
{
    Q_ASSERT(ptr_);

    if (nodeId.isEmpty())
        throw std::invalid_argument("id can't be empty string");

    const QByteArray idName = nodeId.toUtf8();
    QGumboNodes nodes;

    auto functor = [&nodes, &idName] (GumboNode* node) {
        GumboAttribute* attr = gumbo_get_attribute(&node->v.element.attributes, ID_ATTRIBUTE);
        if (attr) {
            if (strcmp(attr->value, idName.constData()) == 0) {
                nodes.emplace_back(QGumboNode(node));
                return true;
            }
        }
        return false;
    };

    iterateTree(ptr_, functor);

    return nodes;
}
QGumboNodes QGumboNode::getElementsByTagName(HtmlTag tag) const
{
    Q_ASSERT(ptr_);

    GumboTag tag_ = static_cast<GumboTag>(tag);
    QGumboNodes nodes;

    auto functor = [&nodes, tag_](GumboNode* node) {
        if (node->v.element.tag == tag_) {
            nodes.emplace_back(QGumboNode(node));
        }
        return false;
    };

    iterateTree(ptr_, functor);

    return nodes;
}

QGumboNodes QGumboNode::getElementsByClassName(const QString& name) const
{
    Q_ASSERT(ptr_);

    if (name.isEmpty())
        throw std::invalid_argument("class name can't be empty string");

    const QByteArray className = name.toUtf8();
    QGumboNodes nodes;

    auto functor = [&nodes, &className] (GumboNode* node) {
        GumboAttribute* attr = gumbo_get_attribute(&node->v.element.attributes, CLASS_ATTRIBUTE);
        if (attr) {
            if (strstr(attr->value, className.constData()) != nullptr) {
                nodes.emplace_back(QGumboNode(node));
            }
        }
        return false;
    };

    iterateTree(ptr_, functor);

    return nodes;
}

QString QGumboNode::innerText() const
{
    Q_ASSERT(ptr_);

#ifdef QT_NO_DEBUG
    for (uint i = 0; i < ptr_->v.element.children.length; ++i) {
        GumboNode* node =
                static_cast<GumboNode*>(ptr_->v.element.children.data[i]);
        Q_ASSERT(node);
        if (node->type == GUMBO_NODE_TEXT) {
            return QString::fromUtf8(node->v.text.text);
        }
    }
#else
    QString data;
    for (uint i = 0; i < ptr_->v.element.children.length; ++i) {
        GumboNode* node =
                static_cast<GumboNode*>(ptr_->v.element.children.data[i]);
        Q_ASSERT(node);

        if (node->type == GUMBO_NODE_TEXT) {

            Q_ASSERT(data.isEmpty());
            data += QString::fromUtf8(node->v.text.text);
        }
    }
    return data;
#endif
}

HtmlTag QGumboNode::tag() const
{
    if (isElement())
        return HtmlTag(ptr_->v.element.tag);

    return HtmlTag::UNKNOWN;
}

QString QGumboNode::tagName() const
{
    Q_ASSERT(ptr_);
    GumboTag tag = ptr_->v.element.tag;
    return QString::fromUtf8(gumbo_normalized_tagname(tag));
}

QString QGumboNode::nodeName() const {
    return tagName();
}

QString QGumboNode::id() const
{
    GumboAttribute* attr = gumbo_get_attribute(&ptr_->v.element.attributes, ID_ATTRIBUTE);
    if (attr)
        return QString::fromUtf8(attr->value);

    return QString();
}

QStringList QGumboNode::classList() const
{
    GumboAttribute* attr = gumbo_get_attribute(&ptr_->v.element.attributes, CLASS_ATTRIBUTE);
    if (attr) {
        QString values = QString::fromUtf8(attr->value);
        return values.split(u8" ", QString::SkipEmptyParts, Qt::CaseInsensitive);
    }

    return QStringList();
}

bool QGumboNode::isElement() const
{
    return ptr_->type == GUMBO_NODE_ELEMENT;
}

bool QGumboNode::hasAttribute(const QString& name) const
{
    if (name.isEmpty())
        throw std::invalid_argument("attribute can't be empty string");

    GumboAttribute* attr = gumbo_get_attribute(&ptr_->v.element.attributes,
                                               name.toUtf8().constData());
    return attr != nullptr;
}

QString QGumboNode::getAttribute(const QString& attrName) const
{
    if (attrName.isEmpty())
        throw std::invalid_argument("attribute name can't be empty string");

    GumboAttribute* attr = gumbo_get_attribute(&ptr_->v.element.attributes,
                                               attrName.toUtf8().constData());
    if (attr)
        return QString::fromUtf8(attr->value);

    return QString();
}

QGumboAttributes QGumboNode::allAttributes() const
{
    Q_ASSERT(ptr_);

    QGumboAttributes attrs;

    for (uint i = 0, len = ptr_->v.element.attributes.length; i < len; ++i) {
        GumboAttribute* attr =
                static_cast<GumboAttribute*>(ptr_->v.element.attributes.data[i]);
        attrs.emplace_back(QGumboAttribute(attr->name, attr->value));
    }
    return attrs;
}


void QGumboNode::forEach(std::function<void(const QGumboNode&)> func) const
{
    Q_ASSERT(ptr_);

    auto functor = [&func](GumboNode* node) {
        func(QGumboNode(node));
        return false;
    };

    iterateTree(ptr_, functor);
}

QGumboNode::operator bool() const {
    return ptr_;
}
