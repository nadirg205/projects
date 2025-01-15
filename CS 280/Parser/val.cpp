#include "val.h"
using namespace std;

bool isNumeric(const ValType& val) {
    return val == VINT || val == VREAL || val == VCHAR;
}

Value Value::operator+(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL || type2 == VBOOL || type1 == VERR || type2 == VERR) {
        return result;
    }
    if (type1 == type2) {
        if (type1 == VINT) {
            result.SetInt(GetInt() + op.GetInt());
        }
        else if (type1 == VREAL) {
            result.SetReal(GetReal() + op.GetReal());
        }
        else if (type1 == VCHAR) {
            if (GetChar() + op.GetChar() > 127 || GetChar() + op.GetChar() < 0) {
                result.SetInt(GetChar() + op.GetChar());
                result.SetType(VINT);
                return result;
            }
            result.SetChar(GetChar() + op.GetChar());
        }
        else if (type1 == VSTRING) {
            result.SetString(GetString() + op.GetString());
        }
        result.SetType(type1);
    }
    else if (type1 == VINT && type2 == VCHAR) {
        result.SetInt(GetInt() + op.GetChar());
        result.SetType(VINT);
    }
    else if (type1 == VCHAR && type2 == VINT) {
        result.SetInt(GetChar() + op.GetInt());
        result.SetType(VINT);
    }
    else if (type1 == VSTRING && type2 == VCHAR) {
        result.SetString(GetString() + op.GetChar());
        result.SetType(VSTRING);
    }
    else if (type1 == VCHAR && type2 == VSTRING) {
        result.SetString(GetChar() + op.GetString());
        result.SetType(VSTRING);
    }
    else if ((type1 == VSTRING && type2 != VSTRING) || (type1 != VSTRING && type2 == VSTRING)) {
        return result;
    }
    else {
        result.SetType(VREAL);
        double r = 0;
        if (type1 == VINT) {
            r += GetInt();
        }
        else if (type1 == VREAL) {
            r += GetReal();
        }
        else if (type1 == VCHAR) {
            r += GetChar();
        }
        if (type2 == VINT) {
            r += op.GetInt();
        }
        else if (type2 == VREAL) {
            r += op.GetReal();
        }
        else if (type2 == VCHAR) {
            r += op.GetChar();
        }
        result.SetReal(r);
    }
    return result;
}

Value Value::operator-(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL || type2 == VBOOL || type1 == VERR || type2 == VERR || type1 == VSTRING || type2 == VSTRING) {
        return result;
    }
    if (type1 == type2) {
        if (type1 == VINT) {
            result.SetInt(GetInt() - op.GetInt());
        }
        else if (type1 == VREAL) {
            result.SetReal(GetReal() - op.GetReal());
        }
        else if (type1 == VCHAR) {
            if (GetChar() - op.GetChar() > 127 || GetChar() - op.GetChar() < 0) {
                result.SetInt(GetChar() - op.GetChar());
                result.SetType(VINT);
                return result;
            }
            result.SetChar(GetChar() - op.GetChar());
        }
        result.SetType(type1);
    }
    else if (type1 == VINT && type2 == VCHAR) {
        result.SetInt(GetInt() - op.GetChar());
        result.SetType(VINT);
    }
    else if (type1 == VCHAR && type2 == VINT) {
        result.SetInt(GetChar() - op.GetInt());
        result.SetType(VINT);
    }
    else {
        result.SetType(VREAL);
        double r = 0;
        if (type1 == VINT) {
            r += GetInt();
        }
        else if (type1 == VREAL) {
            r += GetReal();
        }
        else if (type1 == VCHAR) {
            r += GetChar();
        }
        if (type2 == VINT) {
            r -= op.GetInt();
        }
        else if (type2 == VREAL) {
            r -= op.GetReal();
        }
        else if (type2 == VCHAR) {
            r -= op.GetChar();
        }
        result.SetReal(r);
    }
    return result;
}

Value Value::operator*(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL || type2 == VBOOL || type1 == VERR || type2 == VERR || type1 == VSTRING || type2 == VSTRING) {
        return result;
    }
    if (type1 == type2) {
        if (type1 == VINT) {
            result.SetInt(GetInt() * op.GetInt());
        }
        else if (type1 == VREAL) {
            result.SetReal(GetReal() * op.GetReal());
        }
        else if (type1 == VCHAR) {
            if (GetChar() * op.GetChar() > 127 || GetChar() * op.GetChar() < 0) {
                result.SetInt(GetChar() * op.GetChar());
                result.SetType(VINT);
                return result;
            }
            result.SetChar(GetChar() * op.GetChar());
        }
        result.SetType(type1);
    }
    else if (type1 == VINT && type2 == VCHAR) {
        result.SetInt(GetInt() * op.GetChar());
        result.SetType(VINT);
    }
    else if (type1 == VCHAR && type2 == VINT) {
        result.SetInt(GetChar() * op.GetInt());
        result.SetType(VINT);
    }
    else {
        result.SetType(VREAL);
        double r = 0;
        if (type1 == VINT) {
            r += GetInt();
        }
        else if (type1 == VREAL) {
            r += GetReal();
        }
        else if (type1 == VCHAR) {
            r += GetChar();
        }
        if (type2 == VINT) {
            r *= op.GetInt();
        }
        else if (type2 == VREAL) {
            r *= op.GetReal();
        }
        else if (type2 == VCHAR) {
            r *= op.GetChar();
        }
        result.SetReal(r);
    }
    return result;
}

Value Value::operator/(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL || type2 == VBOOL || type1 == VERR || type2 == VERR || type1 == VSTRING || type2 == VSTRING) {
        return result;
    }
    if (type1 == type2) {
        if (type1 == VINT) {
            result.SetInt(GetInt() / op.GetInt());
        }
        else if (type1 == VREAL) {
            result.SetReal(GetReal() / op.GetReal());
        }
        else if (type1 == VCHAR) {
            if (GetChar() / op.GetChar() > 127 || GetChar() / op.GetChar() < 0) {
                result.SetInt(GetChar() / op.GetChar());
                result.SetType(VINT);
                return result;
            }
            result.SetChar(GetChar() / op.GetChar());
        }
        result.SetType(type1);
    }
    else if (type1 == VINT && type2 == VCHAR) {
        result.SetInt(GetInt() / op.GetChar());
        result.SetType(VINT);
    }
    else if (type1 == VCHAR && type2 == VINT) {
        result.SetInt(GetChar() / op.GetInt());
        result.SetType(VINT);
    }
    else {
        result.SetType(VREAL);
        double r = 0;
        if (type1 == VINT) {
            r += GetInt();
        }
        else if (type1 == VREAL) {
            r += GetReal();
        }
        else if (type1 == VCHAR) {
            r += GetChar();
        }
        if (type2 == VINT) {
            r /= op.GetInt();
        }
        else if (type2 == VREAL) {
            r /= op.GetReal();
        }
        else if (type2 == VCHAR) {
            r /= op.GetChar();
        }
        result.SetReal(r);
    }
    return result;
}

Value Value::operator%(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VREAL || type2 == VREAL) {
        return result;
    }
    if (type1 == type2) {
        if (type1 == VINT) {
            result.SetInt(GetInt() % op.GetInt());
        }
        else if (type1 == VCHAR) {
            if (GetChar() % op.GetChar() > 127 || GetChar() % op.GetChar() < 0) {
                result.SetInt(GetChar() % op.GetChar());
                result.SetType(VINT);
                return result;
            }
            result.SetChar(GetChar() % op.GetChar());
        }
        result.SetType(type1);
    }
    else if (type1 == VINT && type2 == VCHAR) {
        result.SetInt(GetInt() % op.GetChar());
        result.SetType(VINT);
    }
    else if (type1 == VCHAR && type2 == VINT) {
        result.SetInt(GetChar() % op.GetInt());
        result.SetType(VINT);
    }
    return result;
}

Value Value::operator>(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (isNumeric(type1) && isNumeric(type2)) {
        if (type1 == VINT) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() > op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() > op.GetReal());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() > op.GetChar());
            }
        }
        else if (type1 == VREAL) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() > op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() > op.GetReal());
            }
        }
        else if (type1 == VCHAR) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() > op.GetInt());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() > op.GetChar());
            }
        }
    }
    else if (type1 == type2) {
        result.SetType(VBOOL);
        switch (type1)
        {
        case VINT:
            result.SetBool(GetInt() > op.GetInt());
            break;
        case VREAL:
            result.SetBool(GetReal() > op.GetReal());
            break;
        case VCHAR:
            result.SetBool(GetChar() > op.GetChar());
            break;
        case VBOOL:
            result.SetBool(GetBool() > op.GetBool());
            break;
        case VSTRING:
            result.SetBool(GetString() > op.GetString());
            break;
        default:
            break;
        }
    }
    return result;
}

Value Value::operator<(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (isNumeric(type1) && isNumeric(type2)) {
        if (type1 == VINT) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() < op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() < op.GetReal());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() < op.GetChar());
            }
        }
        else if (type1 == VREAL) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() < op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() < op.GetReal());
            }
        }
        else if (type1 == VCHAR) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() < op.GetInt());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() < op.GetChar());
            }
        }
    }
    else if (type1 == type2) {
        result.SetType(VBOOL);
        switch (type1)
        {
        case VINT:
            result.SetBool(GetInt() < op.GetInt());
            break;
        case VREAL:
            result.SetBool(GetReal() < op.GetReal());
            break;
        case VCHAR:
            result.SetBool(GetChar() < op.GetChar());
            break;
        case VBOOL:
            result.SetBool(GetBool() < op.GetBool());
            break;
        case VSTRING:
            result.SetBool(GetString() < op.GetString());
            break;
        default:
            break;
        }
    }
    return result;
}

Value Value::operator==(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (isNumeric(type1) && isNumeric(type2)) {
        if (type1 == VINT) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() == op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() == op.GetReal());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() == op.GetChar());
            }
        }
        else if (type1 == VREAL) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() == op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() == op.GetReal());
            }
        }
        else if (type1 == VCHAR) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() == op.GetInt());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() == op.GetChar());
            }
        }
    }
    else if (type1 == type2) {
        result.SetType(VBOOL);
        switch (type1)
        {
        case VINT:
            result.SetBool(GetInt() == op.GetInt());
            break;
        case VREAL:
            result.SetBool(GetReal() == op.GetReal());
            break;
        case VCHAR:
            result.SetBool(GetChar() == op.GetChar());
            break;
        case VBOOL:
            result.SetBool(GetBool() == op.GetBool());
            break;
        case VSTRING:
            result.SetBool(GetString() == op.GetString());
            break;
        default:
            break;
        }
    }
    return result;
}

Value Value::operator!=(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (isNumeric(type1) && isNumeric(type2)) {
        if (type1 == VINT) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() != op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() != op.GetReal());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetInt() != op.GetChar());
            }
        }
        else if (type1 == VREAL) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() != op.GetInt());
            }
            else if (type2 == VREAL) {
                result.SetType(VBOOL);
                result.SetBool(GetReal() != op.GetReal());
            }
        }
        else if (type1 == VCHAR) {
            if (type2 == VINT) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() != op.GetInt());
            }
            else if (type2 == VCHAR) {
                result.SetType(VBOOL);
                result.SetBool(GetChar() != op.GetChar());
            }
        }
    }
    else if (type1 == type2) {
        result.SetType(VBOOL);
        switch (type1)
        {
        case VINT:
            result.SetBool(GetInt() != op.GetInt());
            break;
        case VREAL:
            result.SetBool(GetReal() != op.GetReal());
            break;
        case VCHAR:
            result.SetBool(GetChar() != op.GetChar());
            break;
        case VBOOL:
            result.SetBool(GetBool() != op.GetBool());
            break;
        case VSTRING:
            result.SetBool(GetString() != op.GetString());
            break;
        default:
            break;
        }
    }
    return result;
}


Value Value::operator&&(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL && type1 == type2) {
        result.SetType(VBOOL);
        result.SetBool(GetBool() && op.GetBool());
    }
    return result;
}

Value Value::operator||(const Value& op) const {
    Value result;
    ValType type1 = GetType();
    ValType type2 = op.GetType();
    if (type1 == VBOOL && type1 == type2) {
        result.SetType(VBOOL);
        result.SetBool(GetBool() || op.GetBool());
    }
    return result;
}

Value Value::operator!(void) const {
    Value result;
    result.SetType(VBOOL);
    result.SetBool(!GetBool());
    return result;
}