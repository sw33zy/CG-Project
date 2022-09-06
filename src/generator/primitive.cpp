#include <iostream>
#include "primitive.hpp"

using std::endl;
using std::ofstream;

// Primitive -----------------------------------------------------------------------------------------------------------


Primitive::Primitive()
        : points(), strips(), current() { this->current = -1; }


void Primitive::pushPoint(Point input) {
    points.emplace_back(input);
}

int Primitive::indexPoint(Point input) {
    int res =(int) points.size();
    points.emplace_back(input);
    return res;
}


void Primitive::autoPushPoint(Point p1) {
    pushIndex(indexIfAbsent(p1));
}


void Primitive::pushIndex(int index) {
    strips[current].pushIndex(index);
}

void Primitive::writeStringTo(std::ostream &destination) {
    destination << points.size() << endl << strips.size() << endl;
    for (Point pt : points)
        destination << pt.getString(" ") << endl;
    for (auto &i : strips)
        destination << i << endl;
}

int Primitive::indexIfAbsent(Point p) {
    int res;
    auto it = find(points.begin(), points.end(), p);

    if (it != points.end())
        res = it - points.begin();
    else {
        points.emplace_back(p);
        res = (int) points.size() - 1;
    }
    return res;
}

void Primitive::autoPushSegment(Point p1, Point p2) {
    pushIndexSegment(indexIfAbsent(p1), indexIfAbsent(p2));
}


void Primitive::pushIndexSegment(int p1, int p2) {
    strips[current].pushIndex(p1);
    strips[current].pushIndex(p2);
}


void Primitive::autoSnapStrip() {
    IndexStrip *actual = &strips[current];
    pair<int, int> start = actual->getStart();
    pair<int, int> end = actual->getEnding();

    //If this takes to long (which i don't think it will),
    //a map can be used to map the start and ending of each strip
    for (int i = 0; i < strips.size(); i++) {
        if (i != current) {
            IndexStrip *c = &strips[i];
            if (start == c->getEnding()) {
                c->append(*actual);
                if (i < current)
                    current--;
                strips.pop_back();
                break;
            }

            if (end == c->getStart()) {
                c->prepend(*c);
                strips.pop_back();
                current = i;
                break;
            }
        }
    }
}

/*
void Primitive::newRecycledStrip(int stride, int count) {
    if (current < 0) {
        current = 0;
        if (strips.empty())
            newStrip();
    } else {
        StripIndex old = current;
        inject(strips[old], stride, count);
    }
}

void Primitive::inject(const IndexStrip &strip, int stride, int count) {
    IndexStrip *old = &strips[current];



}


void Primitive::injectStart(const vector<int> &values, int stride, int count) {
    int i, j, c, max = (int) values.size();
    IndexStrip res;

    for (i = 0, c = 0; c < max;) {
        for (j = 0; j < stride; j++)
            res.pushIndex(-1);
        i += j;
        for (j = 0; j < count && c < max; j++, c++)
            res.pushIndex(values[c]);
        i += j;
    }

    inject(res, stride, count);
}
*/

void Primitive::newStrip() {
    current = strips.size();
    strips.emplace_back(IndexStrip());
}

void Primitive::pushStrip(const IndexStrip &strip) {
    current = strips.size();
    strips.emplace_back(strip);
}


// IndexStrip ----------------------------------------------------------------------------------------------------------
IndexStrip::IndexStrip() : indexes() {
}

pair<int, int> IndexStrip::getStart() {
    if (indexes.size() < 2)
        return {-1, -1};
    return {indexes[0], indexes[1]};
}

pair<int, int> IndexStrip::getEnding() {
    int size = (int) indexes.size();
    if (size < 2)
        return {-1, -1};
    return {indexes[size - 2], indexes[size - 1]};
}

void IndexStrip::pushIndex(int index) {
    indexes.emplace_back(index);
}

std::ostream &operator<<(std::ostream &os, const IndexStrip &strip) {
    unsigned int size = strip.indexes.size();

    if(size<1){
        os<< "[Empty_Strip]";
        return os;
    }

    for (int i = 0; i < size - 1; i++)
        os << strip.indexes[i] << ' ';
    os << strip.indexes[size - 1];
    return os;
}

void IndexStrip::append(IndexStrip &strip) {
    for (int i = 2; i < strip.indexes.size(); i++)
        indexes.emplace_back(strip.indexes[i]);
}

void IndexStrip::prepend(IndexStrip &strip) {
    for (int i = 0; i < strip.indexes.size() - 2; i++)
        indexes.emplace(indexes.begin(), strip.indexes[i]);
}

/*
IndexStrip IndexStrip::recycle(int stride, int count){
    IndexStrip res;
    for (int i=stride; i<indexes.size();i+=stride){

    }


}
*/

//Operators
int IndexStrip::operator[](int i) const {
    return indexes[i];
}

int &IndexStrip::operator[](int i) {
    return indexes[i];
}


