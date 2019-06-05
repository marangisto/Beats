#pragma once

template<typename T>
struct list_node
{
    list_node(): next(this), prev(this) {}
    list_node(const T& _value): value(_value), next(0), prev(0) {}

    T           value;
    list_node   *next;
    list_node   *prev;
};

template<typename T>
struct list_iterator
{
    list_iterator(): p(0) {}
    list_iterator(list_node<T> *_p): p(_p) {}

    list_iterator& operator++()
    {
        p = p->next;
        return *this;
    }

    T& operator*() const
    {
        return p->value;
    }

    friend bool operator==(const list_iterator& x, const list_iterator& y)
    {
        return x.p == y.p;
    }

    friend bool operator!=(const list_iterator& x, const list_iterator& y)
    {
        return !(x == y);
    }

    list_node<T> *p;
};

template<typename T>
class list
{
public:
    list()
    {
        s = new list_node<T>();
    }

    list(int n, const T& value)
    {
        s = new list_node<T>();
        for (int i = 0; i < n; ++i)
            push_back(value);
    }

    ~list()
    {
        for (list_iterator<T> it = begin(); it != end(); ++it)
            delete it.p;
        delete s;
    }

    list_iterator<T> begin() const
    {
        return list_iterator<T>(s->next);
    }

    list_iterator<T> end() const
    {
        return list_iterator<T>(s);
    }

    void push_front(const T& value)
    {
        list_node<T> *node = new list_node<T>(value);
        
        s->next->prev = node;
        node->next = s->next;
        s->next = node;
        node->prev = s;
    }

    void push_back(const T& value)
    {
        list_node<T> *node = new list_node<T>(value);
        
        s->prev->next = node;
        node->prev = s->prev;
        s->prev = node;
        node->next = s;
    }

    void splice(const list_iterator<T>& p, const list& c)
    {
        for (list_iterator<T> it = c.begin(); it != c.end(); ++it)
            push_back(*it); // FIXME: supposed to be at p!
    }

private:
    list_node<T> *s;
};

