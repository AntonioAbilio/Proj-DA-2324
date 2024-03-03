//
// Created by leona on 03/03/2024.
//

#ifndef DA_WATERELEMENT_H
#define DA_WATERELEMENT_H

#include <string>

class WaterElement {

private:
    std::string code;
    int id;

public:

    WaterElement(const std::string &code, int id) : code(code), id(id) {}

    const std::string &getCode() const {
        return code;
    }

    void setCode(const std::string &code) {
        WaterElement::code = code;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        WaterElement::id = id;
    }

};

#endif //DA_WATERELEMENT_H
