#ifndef HITTABLE_LIST_HPP_
#define HITTABLE_LIST_HPP_

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
        HitRecord tempRec;
        bool hitAnything = false;
        auto closestSoFar = rayT.max;

        for (const auto& object : objects) {
            if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }

        return hitAnything;
    }
};

#endif