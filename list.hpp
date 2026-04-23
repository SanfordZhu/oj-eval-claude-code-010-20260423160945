#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
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
    /**
     * add data members for linked list as protected members
     */
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->next = pos;
        cur->prev = pos->prev;
        pos->prev->next = cur;
        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
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
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *ptr;
        list *container;

    public:
        iterator(node *p = nullptr, list *c = nullptr) : ptr(p), container(c) {}
        iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        node* get_ptr() const { return ptr; }
        list* get_container() const { return container; }

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return *(ptr->data);
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr == container->head || ptr == container->tail)
                throw invalid_iterator();
            return ptr->data;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class const_iterator;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
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
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
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
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->next->data);
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(tail->prev->data);
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }
    const_iterator cend() const {
        return const_iterator(tail, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.get_ptr() == nullptr || pos.get_container() != this) throw invalid_iterator();
        node *new_node = new node(value);
        insert(pos.get_ptr(), new_node);
        return iterator(new_node, this);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
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
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        insert(tail, new node(value));
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *to_delete = tail->prev;
        erase(to_delete);
        delete to_delete;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        insert(head->next, new node(value));
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *to_delete = head->next;
        erase(to_delete);
        delete to_delete;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (size() <= 1) return;

        // Use bubble sort to avoid default constructor issues
        bool swapped;
        do {
            swapped = false;
            node *cur = head->next;
            while (cur != tail && cur->next != tail) {
                node *next_node = cur->next;
                if (*(next_node->data) < *(cur->data)) {
                    // Swap the data
                    T *temp = cur->data;
                    cur->data = next_node->data;
                    next_node->data = temp;
                    swapped = true;
                }
                cur = next_node;
            }
        } while (swapped);
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;

        node *cur1 = head->next;
        node *cur2 = other.head->next;

        while (cur1 != tail && cur2 != other.tail) {
            if (*(cur2->data) < *(cur1->data)) {
                // Insert cur2 before cur1
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

        // Append remaining elements from other list
        if (cur2 != other.tail) {
            node *last_other = other.tail->prev;
            node *before_tail = tail->prev;

            before_tail->next = cur2;
            cur2->prev = before_tail;

            last_other->next = tail;
            tail->prev = last_other;
        }

        // Reset other list
        other.head->next = other.tail;
        other.tail->prev = other.head;

        list_size += other.list_size;
        other.list_size = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (size() <= 1) return;

        node *cur = head;
        while (cur != nullptr) {
            node *temp = cur->next;
            cur->next = cur->prev;
            cur->prev = temp;
            cur = temp;
        }

        // Swap head and tail
        node *temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (size() <= 1) return;

        node *cur = head->next;
        while (cur != tail && cur->next != tail) {
            node *next_node = cur->next;
            if (*(cur->data) == *(next_node->data)) {
                // Remove next_node
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
