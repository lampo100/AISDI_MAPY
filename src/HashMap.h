#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include "TreeMap.h"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>
#include <array>
#include <iterator>

#define ARRAY_SIZE 1024

namespace aisdi {

    template<typename KeyType, typename ValueType>
    class HashMap {
    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;

        class ConstIterator;

        class Iterator;

        class List;

        struct BaseNode;
        struct Node;

        friend class List;

        friend class ConstIterator;

        friend class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        HashMap() : count(0) {}

        HashMap(std::initializer_list<value_type> list) : HashMap() {
            for (auto i: list) {
                operator[](i.first) = i.second;
            }
        }

        HashMap(const HashMap &other) : HashMap() {
            for (auto i: other) {
                operator[](i.first) = i.second;
            }
        }

        HashMap(HashMap &&other) : HashMap() {
            for (auto i: other) {
                operator[](i.first) = i.second;
            }
            clean(other);
        }

        HashMap &operator=(const HashMap &other) {
            if (*this == other) return *this;
            clean(*this);
            if (!other.isEmpty())
                for (auto i: other) {
                    operator[](i.first) = i.second;
                }
            return *this;
        }

        HashMap &operator=(HashMap &&other) {
            if (*this == other) return *this;
            clean(*this);
            if (!other.isEmpty())
                for (auto i: other) {
                    operator[](i.first) = i.second;
                }
            clean(other);
            return *this;
        }

        bool isEmpty() const {
            return count == 0;
        }

        mapped_type &operator[](const key_type &key) {
            size_t index = get_index(key);
            Node *found;
            found = array[index].find(key);
            if (!found) {
                array[index].append(key, ValueType());
                ++count;
                found = array[index].find(key);
                return found->item.second;
            } else {
                return found->item.second;
            }
        }

        const mapped_type &valueOf(const key_type &key) const {
            size_t index = get_index(key);
            BaseNode *t = array[index].find(key);
            if( !t ) throw std::out_of_range("Trying to fin nonexisting key.");
            return static_cast<Node *>(t)->item.second;

        }

        mapped_type &valueOf(const key_type &key) {
            size_t index = get_index(key);
            BaseNode *t = array[index].find(key);
            if( !t ) throw std::out_of_range("Trying to fin nonexisting key.");
            return static_cast<Node *>(t)->item.second;
        }

        const_iterator find(const key_type &key) const {
            std::hash<KeyType> h;
            size_t index = h(key) % ARRAY_SIZE;
            Node *result = array[index].find(key);
            if (!result) return cend();
            return ConstIterator(this, index, result);
        }

        iterator find(const key_type &key) {
            std::hash<KeyType> h;
            size_t index = h(key) % ARRAY_SIZE;
            Node *result = array[index].find(key);
            if (!result) return end();
            return Iterator(this, index, result);
        }

        void remove(const key_type &key) {
            std::hash<KeyType> h;
            size_t index = h(key) % ARRAY_SIZE;
            BaseNode *result = array[index].find(key);
            if( !result ) throw std::out_of_range("Trying to erase nonexisting element.");
            result->next->previous = result->previous;
            result->previous->next = result->next;
            delete result;
            array[index].size--;
            count--;
        }

        void remove(const const_iterator &it) {
            if(it == end()) throw std::out_of_range("Trying to erase end().");
            BaseNode *result = it.node;
            result->next->previous = result->previous;
            result->previous->next = result->next;
            delete result;
            array[it.index].size--;
            count--;
        }

        size_type getSize() const {
            return count;
        }

        bool operator==(const HashMap &other) const {
            if((this->getSize() == 0) && (other.getSize() == 0)) return true;
            if (isEmpty() != other.isEmpty()) return false;
            std::hash<KeyType> h;
            KeyType key;
            ValueType val;
            size_t index;
            for (auto i: *this) {
                key = i.first;
                val = i.second;
                index = h(key)%1024;
                if(!other.array[index].find(key)) return false;
                if (other.array[index].find(key)->item.second != val) return false;
            }
            return true;
        }

        bool operator!=(const HashMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            BaseNode *result = NULL;
            size_t first_used_index = 0;
            for (size_t i = 0; i < ARRAY_SIZE; i++) {
                if (array[i].size != 0) {
                    first_used_index = i;
                    break;
                }
            }
            result = array[first_used_index].head->next;
            return Iterator(this, first_used_index, result);
        }

        iterator end() {
            BaseNode *result = NULL;
            size_t last_used_index = 0;
            for (size_t i = 0; i < ARRAY_SIZE; i++) {
                if (array[i].size != 0) last_used_index = i;
            }
            result = array[last_used_index].tail;
            return Iterator(this, last_used_index, result);
        }

        const_iterator cbegin() const {
            BaseNode *result = NULL;
            size_t first_used_index = 0;
            for (size_t i = 0; i < ARRAY_SIZE; i++) {
                if (array[i].size != 0) {
                    first_used_index = i;
                    break;
                }
            }
            result = array[first_used_index].head->next;
            return ConstIterator(this, first_used_index, result);
        }

        const_iterator cend() const {
            BaseNode *result = NULL;
            size_t last_used_index = 0;
            for (size_t i = 0; i < ARRAY_SIZE; i++) {
                if (array[i].size != 0) last_used_index = i;
            }
            result = array[last_used_index].tail;
            return ConstIterator(this, last_used_index, result);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    protected:
        List array[ARRAY_SIZE];
        size_t count;

        size_t get_index(const key_type &key) const{
            std::hash<KeyType> h;
            return (h(key) % ARRAY_SIZE);
        }

        void clean(HashMap &target) {
            for (size_t i = 0; i < ARRAY_SIZE; i++) {
                if(target.array[i].size != 0){
                    BaseNode *x = target.array[i].head->next;
                    BaseNode *to_remove;
                    while(x != target.array[i].tail){
                        to_remove = x;
                        x = to_remove->next;
                        delete to_remove;
                    }
                }
                target.array[i].head->next = target.array[i].tail;
                target.array[i].tail->previous = target.array[i].head;
            }
            target.count = 0;
        }

    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename HashMap::value_type;
        using pointer = const typename HashMap::value_type *;

        friend class HashMap;

        explicit ConstIterator(const HashMap *mmap, size_t tindex, BaseNode *tnode) :
                map(mmap), index(tindex), node(tnode) {}

        ConstIterator(const ConstIterator &other) : ConstIterator(other.map, other.index, other.node) {}

        ConstIterator &operator++() {
            if (node == map->end().node)throw std::out_of_range("Trying to increment end()");
            if (node->next == map->array[index].tail) {
                size_t next_index = index;
                bool is_next = false;
                if (index != ARRAY_SIZE - 1) {
                    next_index++;
                    for (; next_index < ARRAY_SIZE; ++next_index) {
                        if (map->array[next_index].size != 0) {
                            is_next = true;
                            break;
                        }
                    }
                }

                if (is_next == true) {
                    index = next_index;
                    node = map->array[index].head->next;
                } else {
                    node = node->next; //Powinno przejść na end().node
                }
            } else {
                node = node->next;
            }
            return *this;
        }

        ConstIterator operator++(int) {
            if (node == map->end().node)throw std::out_of_range("Trying to increment end()");
            ConstIterator result(*this);
            if (node->next == map->array[index].tail) {
                size_t next_index = index;
                bool is_next = false;
                if (index != ARRAY_SIZE - 1) {
                    next_index++;
                    for (; next_index < ARRAY_SIZE; ++next_index) {
                        if (map->array[next_index].size != 0) {
                            is_next = true;
                            break;
                        }
                    }
                }

                if (is_next) {
                    index = next_index;
                    node = map->array[index].head->next;
                } else {
                    node = node->next; //Powinno przejść na end().node
                }
            } else {
                node = node->next;
            }
            return result;
        }

        ConstIterator &operator--() {
            if (node == map->begin().node)throw std::out_of_range("Trying to decrement begin()");
            if (node->previous == map->array[index].head) {
                size_t next_index = index;
                bool is_next = false;
                if (index != 0) {
                    --next_index;
                    for (; next_index >= 0; --next_index) {
                        if (map->array[next_index].size != 0) {
                            is_next = true;
                            break;
                        }
                    }
                }

                if (is_next) {
                    index = next_index;
                    node = map->array[index].head->next;
                } else {
                    node = node->previous; //Powinno przejść na begin().node
                }
            } else {
                node = node->previous;
            }
            return *this;
        }

        ConstIterator operator--(int) {
            if (node == map->begin().node)throw std::out_of_range("Trying to decrement begin()");
            ConstIterator result(*this);
            if (node->previous == map->array[index].head) {
                size_t next_index = index;
                bool is_next = false;
                if (index != 0) {
                    --next_index;
                    for (; next_index >= 0; --next_index) {
                        if (map->array[next_index].size != 0) {
                            is_next = true;
                            break;
                        }
                    }
                }

                if (is_next) {
                    index = next_index;
                    node = map->array[index].head->next;
                } else {
                    node = node->previous; //Powinno przejść na begin().node
                }
            } else {
                node = node->previous;
            }
            return result;
        }

        reference operator*() const {
            if (node == map->array[index].head || node == map->array[index].tail)
                throw std::out_of_range("Trying to dereference outside of scope.");
            else return (static_cast<Node *>(node))->item;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            return ((this->map == other.map) && (this->index == other.index) && (this->node == other.node));
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }

    protected:
        const HashMap *map;
        size_t index;
        BaseNode *node;
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename HashMap::reference;
        using pointer = typename HashMap::value_type *;

        friend class HashMap;

        explicit Iterator(const HashMap *mmap, size_t tindex, BaseNode *node) : ConstIterator(mmap, tindex, node) {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        pointer operator->() const {
            return &this->operator*();
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

    template<typename KeyType, typename ValueType>
    class HashMap<KeyType, ValueType>::List {
        friend class HashMap;

    public:
        List() {
            head = new BaseNode(nullptr, nullptr);
            tail = new BaseNode(nullptr, nullptr);
            head->next = tail;
            tail->previous = head;
            size = 0;
        }

        Node *find(const KeyType key) const {//Szuka node'a o podanym kluczu
            BaseNode *result;
            result = head;
            while (result != tail) {
                if (static_cast<Node *>(result)->item.first == key) {
                    return static_cast<Node *>(result);
                };
                result = result->next;
            }
            return NULL;
        }

        void append(const KeyType &key, const ValueType &item) {
            BaseNode *newNode = new Node(tail->previous, tail, key, item);
            tail->previous->next = newNode;
            tail->previous = newNode;
            size++;
        }

    protected:
        BaseNode *head;
        BaseNode *tail;
        size_type size;
    };

    template<typename KeyType, typename ValueType>
    struct HashMap<KeyType, ValueType>::BaseNode {
        BaseNode *next;
        BaseNode *previous;

        BaseNode() {
            next = nullptr;
            previous = nullptr;
        }

        BaseNode(BaseNode *prev, BaseNode *nxt) {
            previous = prev;
            next = nxt;
        }

        virtual ~BaseNode() {
            previous = nullptr;
            next = nullptr;
        }
    };

    template<typename KeyType, typename ValueType>
    struct HashMap<KeyType, ValueType>::Node : public HashMap<KeyType, ValueType>::BaseNode {
        std::pair<const KeyType, ValueType> item;

        Node() : BaseNode() {
            item.first = nullptr;
            item.second = nullptr;
        }

        Node(BaseNode *prev, BaseNode *nxt, const KeyType &key, const ValueType &value) : BaseNode(prev, nxt),
                                                                                          item(key, value) {
        }

        Node(BaseNode *prev, BaseNode *nxt, KeyType &&key, ValueType &&value) : BaseNode(prev, nxt) {
            item.first = std::move(key);
            item = std::move(value);
        }

        ~Node() {}
    };
}

#endif /* AISDI_MAPS_HASHMAP_H */
