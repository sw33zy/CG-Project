#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <ostream>
#include <map>
#include "utils/point.hpp"

using std::pair;
using std::map;
using std::vector;

typedef unsigned int StripIndex;

class IndexStrip {
private:
    std::vector<int> indexes;
public:
    IndexStrip();

    pair<int, int> getStart();

    pair<int, int> getEnding();

    void pushIndex(int index);

    void append(IndexStrip &strip);

    friend std::ostream &operator<<(std::ostream &os, const IndexStrip &strip);

    void prepend(IndexStrip &strip);

    int operator[](int i) const;

    int &operator[](int i);

    // IndexStrip recycle(int stride, int count);


};


class Primitive {
public:
    Primitive();

    void pushPoint(Point);

    int indexIfAbsent(Point p);

    void writeStringTo(std::ostream &);

    /*
    Strip recycling is still work in progress

    void injectStart(const vector<int> &strip, int stride, int count);

    void newRecycledStrip(int stride, int count);

    void inject(const IndexStrip& strip, int stride, int count);

     */

    void pushIndex(int index);

    void autoPushPoint(Point);

    void autoPushSegment(Point p1, Point p2);

    void autoSnapStrip();

    int indexPoint(Point input);

    void pushStrip(const IndexStrip &strip);

    void pushIndexSegment(int p1, int p2);

    void newStrip();

private:
    StripIndex current;
    std::vector<Point> points;
    std::vector<IndexStrip> strips;


};

#endif // MESH_HPP
