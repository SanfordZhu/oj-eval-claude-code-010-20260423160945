#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <functional>

namespace sjtu {

template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &value, node *p = nullptr, node *n = nullptr)
            : data(new T(value)), prev(p), next(n) {}
        ~node() {
            delete data;
        }
    };

protected:
    node *head;
    node *tail;
    size_t list_size;

    node *insert(node *pos, node *cur) {
        cur->next = pos;
        cur->prev = pos->prev;
        pos->prev->next = cur;
        pos->prev = cur;
        list_size++;
        return cur;
    }

    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;
        list *container;

    public:
        iterator(node *p = nullptr, list *c = nullptr) : ptr(p), container(c) {}
        iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        node* get_ptr() const { return ptr; }
        list* get_container() const { return container; }

        iterator operator++(int) {
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        iterator operator--(int) {
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        T & operator *() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return *(ptr->data);
        }
        T * operator ->() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return ptr->data;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class const_iterator;
    };

    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const const_iterator &other) : ptr(other.ptr), container(other.container) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const node* get_ptr() const { return ptr; }
        const list* get_container() const { return container; }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        const_iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        const_iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        const T & operator *() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return *(ptr->data);
        }
        const T * operator ->() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return ptr->data;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class iterator;
    };

    list() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        list_size = 0;
    }
    list(const list &other) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        list_size = 0;

        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }

    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }

    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->next->data);
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(tail->prev->data);
    }

    iterator begin() {
        return iterator(head->next, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    iterator end() {
        return iterator(tail, this);
    }
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    virtual bool empty() const {
        return list_size == 0;
    }
    virtual size_t size() const {
        return list_size;
    }

    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    virtual iterator insert(iterator pos, const T &value) {
        if (pos.get_ptr() == nullptr || pos.get_container() != this) throw invalid_iterator();
        node *new_node = new node(value);
        insert(pos.get_ptr(), new_node);
        return iterator(new_node, this);
    }

    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (pos.get_ptr() == nullptr || pos.get_ptr() == head || pos.get_ptr() == tail || pos.get_container() != this)
            throw invalid_iterator();
        node *to_delete = pos.get_ptr();
        iterator result(to_delete->next, this);
        erase(to_delete);
        delete to_delete;
        return result;
    }

    void push_back(const T &value) {
        insert(tail, new node(value));
    }
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *to_delete = tail->prev;
        erase(to_delete);
        delete to_delete;
    }
    void push_front(const T &value) {
        insert(head->next, new node(value));
    }
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *to_delete = head->next;
        erase(to_delete);
        delete to_delete;
    }

    void sort() {
        if (size() <= 1) return;

        bool swapped;
        do {
            swapped = false;
            node *cur = head->next;
            while (cur != tail && cur->next != tail) {
                node *next_node = cur->next;
                if (*(next_node->data) < *(cur->data)) {
                    T *temp = cur->data;
                    cur->data = next_node->data;
                    next_node->data = temp;
                    swapped = true;
                }
                cur = next_node;
            }
        } while (swapped);
    }

    void merge(list &other) {
        if (this == &other) return;

        node *cur1 = head->next;
        node *cur2 = other.head->next;

        while (cur1 != tail && cur2 != other.tail) {
            if (*(cur2->data) < *(cur1->data)) {
                node *next2 = cur2->next;
                cur2->prev = cur1->prev;
                cur2->next = cur1;
                cur1->prev->next = cur2;
                cur1->prev = cur2;
                cur2 = next2;
            } else {
                cur1 = cur1->next;
            }
        }

        if (cur2 != other.tail) {
            node *last_other = other.tail->prev;
            node *before_tail = tail->prev;

            before_tail->next = cur2;
            cur2->prev = before_tail;

            last_other->next = tail;
            tail->prev = last_other;
        }

        other.head->next = other.tail;
        other.tail->prev = other.head;

        list_size += other.list_size;
        other.list_size = 0;
    }

    void reverse() {
        if (size() <= 1) return;

        node *cur = head;
        while (cur != nullptr) {
            node *temp = cur->next;
            cur->next = cur->prev;
            cur->prev = temp;
            cur = temp;
        }

        node *temp = head;
        head = tail;
        tail = temp;
    }

    void unique() {
        if (size() <= 1) return;

        node *cur = head->next;
        while (cur != tail && cur->next != tail) {
            node *next_node = cur->next;
            if (*(cur->data) == *(next_node->data)) {
                cur->next = next_node->next;
                next_node->next->prev = cur;
                delete next_node;
                list_size--;
            } else {
                cur = cur->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP