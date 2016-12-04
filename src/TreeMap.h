#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

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

        friend class ConstIterator;
        friend class Node;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

        TreeMap(): root(NULL), count(0) {}

        TreeMap(std::initializer_list<value_type> list):TreeMap() {
            for(auto i : list) insert(i.first, i.second);
        }

        TreeMap(const TreeMap &other):TreeMap() {
            for(auto i: other)insert(i.first, i.second);
        }

        TreeMap(TreeMap &&other):TreeMap() {
            root = other.root;
            other.root = NULL;
            count = other.count;
            other.count = 0;
        }

        TreeMap &operator=(const TreeMap &other) {
            if(*this == other) return *this;
            clean(root);
            root = NULL;
            count = 0;
            for(auto i: other)insert(i.first, i.second);
            return *this;
        }

        TreeMap &operator=(TreeMap &&other) {
            if(*this == other) return *this;
            clean(root);
            root = other.root;
            other.root = NULL;
            count = other.count;
            other.count = 0;
            return *this;
        }

        bool isEmpty() const {
            return !count;
        }

        mapped_type &operator[](const key_type &key) {
            Node *current = findNode(key);
            if( !current ){
                ValueType i;
                insert(key, i);
                current = findNode(key);
                return current->NodePair.second;
            }
            return current->NodePair.second;
        }

        const mapped_type &valueOf(const key_type &key) const {
            Node *t = findNode(key);
            if( !t ) throw std::out_of_range("There is no element of this key");
            return t->NodePair.second;
        }

        mapped_type &valueOf(const key_type &key) {
            Node *t = findNode(key);
            if( !t ) throw std::out_of_range("There is no element of this key");
            return t->NodePair.second;
        }

        const_iterator find(const key_type &key) const {
            return ConstIterator(this,findNode(key));
        }

        iterator find(const key_type &key) {
            return Iterator(this,findNode(key));
        }

        void insert(KeyType tkey, ValueType tvalue){
                root = insertToNode(root, tkey, tvalue);
                count++;
                return;
        }

        void remove(const key_type &key) {
            Node *temp = findNode(key);
            if( !temp ) throw std::out_of_range("Obiekt o podanym kluczu nie istnieje.");
            temp = removeNode(root, key);
            root = temp;
            count--;
        }

        void remove(const const_iterator &it) {
            if(it == end())throw std::out_of_range("Trying to erase end()");
            remove(it.currentNode->NodePair.first);
        }

        size_type getSize() const {
            return count;
        }

        bool operator==(const TreeMap &other) const {
            return (root == other.root);
        }

        bool operator!=(const TreeMap &other) const {
            return !(*this == other);
        }

        iterator begin() {
            if(count == 0) return Iterator(this, NULL);
            return Iterator(this, findFirst(root));
        }

        iterator end() {
            return Iterator(this, NULL);
        }

        const_iterator cbegin() const {
            if(count == 0) return ConstIterator(this, NULL);
            return ConstIterator(this, findFirst(root));
        }

        const_iterator cend() const {
            return ConstIterator(this, NULL);
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

        Node *insertToNode(Node *pnode, const KeyType tkey, const ValueType tvalue){
            if (!pnode)
                return new Node(tkey, tvalue);
            if(tkey < pnode->NodePair.first)
                pnode->left = insertToNode(pnode->left, tkey, tvalue);
            else
                pnode->right = insertToNode(pnode->right, tkey, tvalue);
            return balance(pnode);
        }

        Node *findMinNode(Node *pnode)const{
            return pnode->left ? findMinNode(pnode->left) : pnode;
        }

        Node *removeMinNode(Node *pnode){
            if( pnode->left == NULL)
                return pnode->right;
            pnode->left = removeMinNode(pnode->left);
            return balance(pnode);
        }

        Node *removeNode(Node *pnode, KeyType tkey){
            if ( !pnode ) return NULL;
            if( tkey < pnode->NodePair.first )
                pnode->left = removeNode(pnode->left, tkey);
            else if(tkey > pnode->NodePair.first )
                pnode->right = removeNode(pnode->right, tkey);
            else{
                Node *qnode = pnode->left;
                Node *rnode = pnode->right;
                delete pnode;
                if( !rnode ) return qnode;
                Node *min = findMinNode(rnode);
                min->right = removeMinNode(rnode);
                min->left = qnode;
                return balance(min);
            }
            return balance(pnode);
        }

        Node *findNodeAt(Node *pnode, KeyType tkey)const{
            if( !pnode ) return NULL;
            if(tkey < pnode->NodePair.first ) pnode = findNodeAt(pnode->left, tkey);
            else if(tkey > pnode->NodePair.first ) pnode = findNodeAt(pnode->right, tkey);
            return pnode;
        }

        Node *findNode(KeyType tkey)const{
            return findNodeAt(root, tkey);
        }

        //Zakładamy przechodzenie po drzewie in-order(Left->Parent->Right)
        Node *findLast() const{
            Node *last = NULL;
            TraverseInOrder(root, last);
            return last;
        }
        Node *findFirst(Node *pnode) const{
            return pnode->left ? findFirst(pnode->left) : pnode;
        }

        //Funkcja przechodząca po drzewie in-order(Left->Parent->Right)
        void TraverseInOrder(Node *p, Node* &t)const{
            if ( !p ) return;
            TraverseInOrder(p->left, t);
            t = p;
            TraverseInOrder(p->right, t);
        }

        void clean(Node *p){
            if ( !p ) return;
            clean(p->left);
            clean(p->right);
            delete p;
        }
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::ConstIterator {
    public:
        friend class TreeMap;
        using reference = typename TreeMap::const_reference;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename TreeMap::value_type;
        using pointer = const typename TreeMap::value_type *;

        explicit ConstIterator(const TreeMap *tta, Node *cnode){
            ttree = tta;
            currentNode = cnode;
        }

        ConstIterator(const ConstIterator &other):ConstIterator(other.ttree, other.currentNode) {}

        ConstIterator &operator++() {
            if( !currentNode ) throw std::out_of_range("Trying to increment end() iterator.");
            else{
                currentNode = inOrderSuccessor(currentNode);
                return *this;
            }
        }

        ConstIterator operator++(int) {
            if( !currentNode ) throw std::out_of_range("Trying to increment end() iterator.");
            else{
                ConstIterator result(this->ttree, this->currentNode);
                currentNode = inOrderSuccessor(currentNode);
                return result;
            }
        }

        ConstIterator &operator--() {
            Node *temp;
            temp = inOrderPredeccessor(currentNode);
            if( !temp ) throw std::out_of_range("Trying to decrement begin() operator.");
            currentNode = temp;
            return *this;
        }

        ConstIterator operator--(int) {
            Node *temp;
            ConstIterator result(this->ttree, this->currentNode);
            temp = inOrderPredeccessor(currentNode);
            if( !temp ) throw std::out_of_range("Trying to decrement begin() operator.");
            currentNode = temp;
            return result;
        }

        reference operator*() const {
            if(!currentNode) throw std::out_of_range("Trying to dereference end() iterator.");
            return (currentNode->NodePair);
        }

        pointer operator->() const {
            return &this->operator*();
        }

        bool operator==(const ConstIterator &other) const {
            return ((this->ttree == other.ttree)&&(this->currentNode == other.currentNode));
        }

        bool operator!=(const ConstIterator &other) const {
            return !(*this == other);
        }
    protected:
        const TreeMap *ttree;
        Node *currentNode;
        Node *findMinNode(Node *pnode){
            return pnode->left ? findMinNode(pnode->left) : pnode;
        }
        Node *inOrderSuccessor(Node *n)
        {
            Node *root = ttree->root;
            // Prawa gałąź nie jest pusta
            if( n->right != NULL )
                return findMinNode(n->right);
            Node *succ = NULL;

            // W innym przypadku
            while (root != NULL)
            {
                if (n->NodePair.first < root->NodePair.first)
                {
                    succ = root;
                    root = root->left;
                }
                else if (n->NodePair.first > root->NodePair.first)
                    root = root->right;
                else
                    break;
            }
            return succ;
        }
        Node *inOrderPredeccessor(Node *n) {
            Node *root = ttree->root;
            //Drzewo jest puste
            if( !root ) return NULL;
            //Dekrementujemy end()
            if( !n ){
                while(root->right != NULL) root = root->right;
                return root;
            }
            //Lewa gałąź nie jest pusta
            if( n->left != NULL )
                return n->left;
            Node *succ = NULL;

            // W innym przypadku
            while (root != NULL)
            {
                if (n->NodePair.first < root->NodePair.first)
                {
                    root = root->left;
                }
                else if (n->NodePair.first > root->NodePair.first){
                    succ = root;
                    root = root->right;
                }
                else
                    break;
            }
            return succ;
        }
    };

    template<typename KeyType, typename ValueType>
    class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator {
    public:
        using reference = typename TreeMap::reference;
        using pointer = typename TreeMap::value_type *;

        explicit Iterator(const TreeMap *tt, Node* cnode):ConstIterator(tt,cnode) {}

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
        friend class TreeMap;
        Node(KeyType tkey, ValueType tvalue):NodePair(tkey, tvalue) {
            height = 1;
            right = NULL;
            left = NULL;
            NodePair.second = tvalue;
        }

        Node *right;
        Node *left;
        unsigned char height;
        std::pair<const KeyType, ValueType> NodePair;
    };

}

#endif /* AISDI_MAPS_MAP_H */
