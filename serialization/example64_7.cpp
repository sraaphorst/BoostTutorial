/**
 * example64_7.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Backward compatibility with version numbers.
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using namespace boost::archive;

std::stringstream ss;

class animal {
private:
    animal() = default;

public:
    animal(int legs, std::string name)
        : legs_{legs}, name_{std::move(name)} {}

    int legs() const {
        return legs_;
    }

    const std::string &name() const {
        return name_;
    }

private:
    friend class boost::serialization::access;

    template<typename Archive>
    friend void serialize(Archive &ar, animal &a, const unsigned int version);

    int legs_;
    std::string name_;

public:
    static void save() {
        text_oarchive oa{ss};
        animal a{4, "cat"};
        oa << a;
    }

    static void load() {
        text_iarchive ia{ss};
        animal a;
        ia >> a;
        std::cout << a.name() << " has " << a.legs() << " legs" << std::endl;
    }
};

template<typename Archive>
void serialize(Archive &ar, animal &a, const unsigned int version) {
    ar & a.legs_;
    if (version > 0)
        ar & a.name_;
}

BOOST_CLASS_VERSION(animal, 1)


int main() {
    animal::save();
    animal::load();
}