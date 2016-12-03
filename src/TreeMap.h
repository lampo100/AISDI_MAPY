#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi {

    template<typename KeyType, typename ValueType>
    class TreeMap {
    public:
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;


        class ConstIterator;

        struct Node;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        TreeMap() {}

        TreeMap(std::initializer_list<value_type> list) {
            (void) list; // disables "unused argument" warning, can be removed when method is implemented.
            throw std::runtime_error("TODO");
        }

        TreeMap(const TreeMap &other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        TreeMap(TreeMap &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        TreeMap &operator=(const TreeMap &other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        TreeMap &operator=(TreeMap &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool isEmpty() const {
            throw std::runtime_error("TODO");
        }

        mapped_type &operator[](const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        const mapped_type &valueOf(const key_type &key) const {
            (void) key;
            throw std::runtime_error("TODO");
        }

        mapped_type &valueOf(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        const_iterator find(const key_type &key) const {
            (void) key;
            throw std::runtime_error("TODO");
        }

        iterator find(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }
        void insert(const KeyType tkey, const ValueType tvalue){
            if (!root){
                root = new Node(tkey,tvalue);
                if()
            }
        }

        void remove(const key_type &key) {
            (void) key;
            throw std::runtime_error("TODO");
        }

        void remove(const const_iterator &it) {
            (void) it;
            throw std::runtime_error("TODO");
        }

        size_type getSize() const {
            throw std::runtime_error("TODO");
        }

        bool operator==(const TreeMap &other) const {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool operator!=(const TreeMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            throw std::runtime_error("TODO");
        }

        iterator end() {
            throw std::runtime_error("TODO");
        }

        const_iterator cbegin() const {
            throw std::runtime_error("TODO");
        }

        const_iterator cend() const {
            throw std::runtime_error("TODO");
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    protected:
        Node *root;
        unsigned int count;

        //Funkcje pomocnicze do drzewa AVL
        unsigned char height(Node *pnode) {
            return pnode ? pnode->height : (unsigned char) 0;
        }
        int bfactor(Node *pnode){
            return height(pnode->right) - height(pnode->left);
        }
        void fixheight(Node *pnode){
            unsigned char hl = height(pnode->left);
            unsigned char hr = height(pnode->right);
            pnode->height = ((hl>hr ? hl : hr) + (unsigned char) 1);
        }
        Node *rotateRight(Node *pnode){
            Node *qnode = pnode->left;
            pnode->left = qnode->right;
            qnode->right = pnode;
            fixheight(pnode);
            fixheight(qnode);
            return qnode;
        }
        Node *rotateLeft(Node *qnode){
            Node *pnode = qnode->right;
            qnode->right = pnode->left;
            pnode->left = qnode;
            fixheight(qnode);
            fixheight(pnode);
            return pnode;
        }
        Node *balance(Node *pnode){
            fixheight(pnode);
            if(bfactor(pnode) == 2){
                if(bfactor(pnode->right) < 0)
                    pnode->right = rotateRight(pnode->right);
                return rotateLeft(pnode);
            }
            if(bfactor(pnode) == -2){
                if(bfactor(pnode->left) > 0)
                    pnode->left = rotateLeft(pnode->left);
                return rotateRight(pnode);
            }
            return pnode;
        }
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::ConstIterator {
    public:
        friend <KeyType, ValueType> TreeMap;
        using reference = typename TreeMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename TreeMap::value_type;
        using pointer = const typename TreeMap::value_type *;

        explicit ConstIterator() {}

        ConstIterator(const ConstIterator &other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        ConstIterator &operator++() {
            throw std::runtime_error("TODO");
        }

        ConstIterator operator++(int) {
            throw std::runtime_error("TODO");
        }

        ConstIterator &operator--() {
            throw std::runtime_error("TODO");
        }

        ConstIterator operator--(int) {
            throw std::runtime_error("TODO");
        }

        reference operator*() const {
            throw std::runtime_error("TODO");
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename TreeMap::reference;
        using pointer = typename TreeMap::value_type *;

        explicit Iterator() {}

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
    struct TreeMap<KeyType, ValueType>::Node {
        Node(KeyType tkey, ValueType tvalue) {
            NodeKey = tkey;
            NodeValue = tvalue;
            height = 1;
            right = NULL;
            left = NULL;
        }

        KeyType NodeKey;
        ValueType NodeValue;
        Node *right;
        Node *left;
        unsigned char height;
    };

}

#endif /* AISDI_MAPS_MAP_H */
