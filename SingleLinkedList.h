#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

template <typename Type>
class SingleLinkedList
{
    struct Node
    {
        Node() = default;
        Node(const Type &val, Node *next)
            : value(val), next_node(next)
        {
        }
        Type value = Type();
        Node *next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator
    {
        friend class SingleLinkedList;
        explicit BasicIterator(Node *node)
            : node_(node)
        {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type> &other) noexcept
            : node_(other.node_)
        {
        }

        BasicIterator &operator=(const BasicIterator &rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept
        {
            return !operator==(rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept
        {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept
        {
            return !operator==(rhs);
        }

        BasicIterator &operator++() noexcept
        {
            assert(node_ != nullptr);
            if (node_)
            {
                node_ = node_->next_node;
            }
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            return &node_->value;
        }

    private:
        Node *node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList()
    {
    }

    SingleLinkedList(const std::initializer_list<Type> &values)
    {
        ListInitialization(values);
    }

    SingleLinkedList(const SingleLinkedList &other)
    {
        assert(size_ == 0 && head_.next_node == nullptr);
        ListInitialization(other);
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator{&head_};
    }

    Iterator InsertAfter(ConstIterator pos, const Type &value)
    {
        assert(pos != end());
        if (IsEmpty())
        {
            PushFront(value);
        }
        else
        {
            Node *new_Node = new Node(value, pos.node_->next_node);
            pos.node_->next_node = new_Node;
            ++size_;
        }
        return Iterator{pos.node_->next_node};
    }

    void PopFront() noexcept
    {
        assert(!IsEmpty());
        Node *temp = head_.next_node;
        head_.next_node = head_.next_node->next_node;
        delete temp;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        assert(pos != end());
        if (!IsEmpty())
        {
            Node *to_Delete = pos.node_->next_node;
            pos.node_->next_node = to_Delete->next_node;
            delete to_Delete;
            --size_;
        }
        return Iterator{pos.node_->next_node};
    }

    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return (head_.next_node == nullptr);
    }

    void PushFront(const Type &value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept
    {
        while (!IsEmpty())
        {
            PopFront();
        }
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs)
    {
        if (this != &rhs)
        {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList &other) noexcept
    {
        std::swap(this->head_.next_node, other.head_.next_node);
        std::swap(this->size_, other.size_);
    }

private:
    Node head_;
    size_t size_ = 0;

    template <typename Element>
    void ListInitialization(const Element &elements)
    {
        auto pos = cbefore_begin();
        for (const auto &element : elements)
        {
            pos = this->InsertAfter(pos, element);
        }
    }
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !operator==(lhs, rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !operator<(rhs, lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return operator<(rhs, lhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !operator>(rhs, lhs);
}