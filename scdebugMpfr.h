
#ifndef _SC_Debug_MPFR_
#define _SC_Debug_MPFR_
#include "./mpfr/gmp.h"
#include "./mpfr/mpfr.h"
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <hash_map>
#include <string>
#include <vector>
#include <stack>

using namespace __gnu_cxx; //hash_map need
typedef uint64_t Addr;
static long int callExp = 0, oldCallExp = 0;

enum ArgType
{
    FloatArg = 0,
    DoubleArg,
    LongDoubleArg
};
static int getTypeID(std::string typeStr)
{
    if (typeStr == "float")
        return 0;
    else if (typeStr == "double")
        return 1;
    else
        return 2;
}
static std::string getTypeStr(ArgType type)
{
    if (type == ArgType::FloatArg)
        return "float";
    else if (type == ArgType::DoubleArg)
    {
        return "double";
    }
    else
    {
        return "long double";
    }
}
class MPFRSlot;
class RContext;

class MPFRPool
{
private:
    MPFRSlot *free_stack;
    MPFRSlot *used_stack;
    RContext *context;
    long slot_count;

public:
    MPFRPool(RContext *c);
    ~MPFRPool();
    MPFRSlot *alloc();
    void free(MPFRSlot *value);
    long getSlotCount();
};
class RContext
{
protected:
    static RContext *default_r_context; // default precision of MPFR values
    long precision;                     // default rounding mode of MPFR
    mpfr_rnd_t round;                   // mpfr_value_pool
    MPFRPool mpfr_value_pool;

public:
    RContext(long p, mpfr_rnd_t r = MPFR_RNDN);
    ~RContext();
    inline long getPrecision();
    inline mpfr_rnd_t getRoundMode();
    inline MPFRSlot *alloc();
    inline void free(MPFRSlot *r);
    long getSlots();
    static RContext *getDefaultContext();
    static void setDefaultContext(RContext *c);
};

class MPFRSlot
{
public:
    MPFRSlot(RContext *context);
    ~MPFRSlot();
    mpfr_t value;
    friend class MPFRPool;

private:
    MPFRSlot *next;
    MPFRSlot *prev;
};

class Real
{
private:
    /* data */
public:
    Real(RContext *c = RContext::getDefaultContext());
    Real(const double d, RContext *c);
    ~Real();

public: //operations
    Real &operator=(const Real &r);
    Real &operator=(const double r);
    Real &operator=(const char *r);
    Real &operator=(const int *r);

    friend Real operator+(const Real &l, const Real &r);
    friend Real operator+(const double l, const Real &r);
    friend Real operator+(const Real &l, const double r);

    Real &operator+=(const Real &r);
    Real &operator+=(const double r);

    friend Real operator-(const Real &l, const Real &r);
    friend Real operator-(const double l, const Real &r);
    friend Real operator-(const Real &l, const double r);

    Real &operator-=(const Real &r);
    Real &operator-=(const double r);

    friend Real operator*(const Real &l, const Real &r);
    friend Real operator*(const double l, const Real &r);
    friend Real operator*(const Real &l, const double r);

    Real &operator*=(const Real &r);
    Real &operator*=(const double r);

    friend Real operator/(const Real &l, const Real &r);
    friend Real operator/(const double l, const Real &r);
    friend Real operator/(const Real &l, const double r);

    Real &operator/=(const Real &r);
    Real &operator/=(const double r);

    friend bool operator==(const Real &l, const Real &r);
    friend bool operator==(const double l, const Real &r);
    friend bool operator==(const Real &l, const double r);

    friend bool operator!=(const Real &l, const Real &r);
    friend bool operator!=(const double l, const Real &r);
    friend bool operator!=(const Real &l, const double r);

    friend bool operator>(const Real &l, const Real &r);
    friend bool operator>(const double l, const Real &r);
    friend bool operator>(const Real &l, const double r);

    friend bool operator<(const Real &l, const Real &r);
    friend bool operator<(const double l, const Real &r);
    friend bool operator<(const Real &l, const double r);

    friend bool operator>=(const Real &l, const Real &r);
    friend bool operator>=(const double l, const Real &r);
    friend bool operator>=(const Real &l, const double r);

    friend bool operator<=(const Real &l, const Real &r);
    friend bool operator<=(const double l, const Real &r);
    friend bool operator<=(const Real &l, const double r);

public:
    bool isNan();
    bool isInf();
    bool isZero();

    mpfr_t &getRawValue() const;
    double toDouble();

public:
    void setExponent(long exp);
    Real abs();
    Real pow(const Real &p);
    Real pow(const long p);
    Real sqrt();
    Real exp();
    Real exp2();
    Real exp10();
    Real log();
    Real log2();
    Real log10();

private:
    MPFRSlot *slot;
    RContext *context;
};
class Shadow
{
private:
    Addr argAddress;
    ArgType type;
    Real value;
    std::string lastop;

public:
    union
    {
        float floatValue;
        double doubleValue;
        long double longdoubleValue;
    } origin;
    Shadow();
    Shadow(Addr address, ArgType t);
    Shadow(const Shadow &sourceShadow);
    Shadow(Addr address, ArgType t, double newValue, std::string lop);
    Shadow(Addr address, ArgType t, Real newValue, std::string lop);
    bool updateInfo(Real newValue, std::string lop);
    bool updateInfo(double newValue, std::string lop);
    Real getValue();
    std::string getTypeString();
    ArgType getType();
    std::string getLastOp();
    ~Shadow();
};

MPFRSlot::MPFRSlot(RContext *context)
{
    mpfr_init2(this->value, context->getPrecision());
    next = NULL;
    prev = NULL;
}
MPFRSlot::~MPFRSlot()
{
    mpfr_clear(value);
}

/*
     * MPFRPool functions
     */
MPFRPool::MPFRPool(RContext *context) : context(context)
{
    free_stack = NULL;
    used_stack = NULL;
    slot_count = 0;
}

MPFRPool::~MPFRPool()
{
    while (free_stack != NULL)
    {
        MPFRSlot *s = free_stack;
        free_stack = free_stack->next;
        delete s;
    }
    while (used_stack != NULL)
    {
        MPFRSlot *s = used_stack;
        used_stack = used_stack->next;
        delete s;
    }
}
long MPFRPool::getSlotCount()
{
    return slot_count;
}
MPFRSlot *MPFRPool::alloc()
{
    MPFRSlot *slot;
    if (free_stack != NULL)
    {
        // remove free head
        slot = free_stack;
        free_stack = free_stack->next;

        if (slot->next != NULL)
            slot->next->prev = NULL;
        slot->prev = slot->next = NULL;
    }
    else
    {
        slot = new MPFRSlot(context);
        slot_count++;
    }
    // insert to used head
    slot->next = used_stack;
    if (used_stack != NULL)
        used_stack->prev = slot;
    used_stack = slot;
    return slot;
}

void MPFRPool::free(MPFRSlot *value)
{
    if (value == NULL)
        return;

    if (used_stack == value)
    {
        // move stack head
        used_stack = value->next;
    }

    // remove value from list
    if (value->prev != NULL)
    {
        value->prev->next = value->next;
    }
    if (value->next != NULL)
    {
        value->next->prev = value->prev;
    }
    value->next = value->prev = NULL;

    // insert into free stack
    value->next = free_stack;
    if (free_stack != NULL)
        free_stack->prev = value;
    free_stack = value;
}

/*
     * RContext functions
     */
RContext::RContext(long p, mpfr_rnd_t r) : mpfr_value_pool(this)
{
    RContext::precision = p;
    round = r;
}

RContext::~RContext()
{
    mpfr_value_pool.~MPFRPool();
}

inline MPFRSlot *RContext::alloc()
{
    return this->mpfr_value_pool.alloc();
}

inline void RContext::free(MPFRSlot *r)
{
    this->mpfr_value_pool.free(r);
}

inline long RContext::getPrecision()
{
    return RContext::precision;
}
inline mpfr_rnd_t RContext::getRoundMode()
{
    return round;
}

long RContext::getSlots()
{
    return mpfr_value_pool.getSlotCount();
}

RContext *RContext::default_r_context = NULL;
RContext *RContext::getDefaultContext()
{
    if (RContext::default_r_context == NULL)
    {
        RContext::default_r_context = new RContext(120, MPFR_RNDN);
    }
    return RContext::default_r_context;
}

void RContext::setDefaultContext(RContext *c)
{
    RContext::default_r_context = c;
}

/*
     * Real functions
     */
Real::Real(RContext *c) : context(c)
{
    slot = context->alloc();
}

Real::Real(const double d, RContext *c) : Real(c)
{
    mpfr_set_d(slot->value, d, c->getRoundMode());
}

Real::~Real()
{
    context->free(slot);
}

mpfr_t &Real::getRawValue() const
{
    return slot->value;
}

Real &Real::operator=(const Real &r)
{
    if (this != &r)
    {
        mpfr_set(slot->value, r.slot->value, context->getRoundMode());
    }
    return *this;
}

Real &Real::operator=(const double r)
{
    mpfr_set_d(slot->value, r, context->getRoundMode());
    return *this;
}

Real &Real::operator=(const char *r)
{
    mpfr_set_str(slot->value, r, 10, context->getRoundMode());
    return *this;
}
Real &Real::operator=(const int *r) //Handle method of bit operations
{
}

Real &Real::operator+=(const Real &r)
{
    mpfr_add(slot->value, slot->value, r.slot->value, context->getRoundMode());
    return *this;
}
Real &Real::operator+=(const double r)
{
    mpfr_add_d(slot->value, slot->value, r, context->getRoundMode());
    return *this;
}

Real &Real::operator-=(const Real &r)
{
    mpfr_sub(slot->value, slot->value, r.slot->value, context->getRoundMode());
    return *this;
}
Real &Real::operator-=(const double r)
{
    mpfr_sub_d(slot->value, slot->value, r, context->getRoundMode());
    return *this;
}

Real &Real::operator*=(const Real &r)
{
    mpfr_mul(slot->value, slot->value, r.slot->value, context->getRoundMode());
    return *this;
}
Real &Real::operator*=(const double r)
{
    mpfr_mul_d(slot->value, slot->value, r, context->getRoundMode());
    return *this;
}

Real &Real::operator/=(const Real &r)
{
    mpfr_div(slot->value, slot->value, r.slot->value, context->getRoundMode());
    return *this;
}
Real &Real::operator/=(const double r)
{
    mpfr_div_d(slot->value, slot->value, r, context->getRoundMode());
    return *this;
}

Real operator+(const Real &l, const Real &r)
{
    Real result(l.context);
    mpfr_add(result.slot->value, l.slot->value, r.slot->value, result.context->getRoundMode());
    return result;
}
Real operator+(const double l, const Real &r)
{
    return r + l;
}

Real operator+(const Real &l, const double r)
{
    Real result(l.context);
    mpfr_add_d(result.slot->value, l.slot->value, r, result.context->getRoundMode());
    return result;
}

Real operator-(const Real &l, const Real &r)
{
    Real result(l.context);
    mpfr_sub(result.slot->value, l.slot->value, r.slot->value, result.context->getRoundMode());
    return result;
}

Real operator-(const double l, const Real &r)
{
    Real result(r.context);
    mpfr_d_sub(result.slot->value, l, r.slot->value, result.context->getRoundMode());
    return result;
}

Real operator-(const Real &l, const double r)
{
    Real result(l.context);
    mpfr_sub_d(result.slot->value, l.slot->value, r, result.context->getRoundMode());
    return result;
}

Real operator*(const Real &l, const Real &r)
{
    Real result(l.context);
    mpfr_mul(result.slot->value, l.slot->value, r.slot->value, result.context->getRoundMode());
    return result;
}
Real operator*(const double l, const Real &r)
{
    return r * l;
}

Real operator*(const Real &l, const double r)
{
    Real result(l.context);
    mpfr_mul_d(result.slot->value, l.slot->value, r, result.context->getRoundMode());
    return result;
}

Real operator/(const Real &l, const Real &r)
{
    Real result(l.context);
    mpfr_div(result.slot->value, l.slot->value, r.slot->value, result.context->getRoundMode());
    return result;
}
Real operator/(const double l, const Real &r)
{
    Real result(r.context);
    mpfr_d_div(result.slot->value, l, r.slot->value, result.context->getRoundMode());
    return result;
}

Real operator/(const Real &l, const double r)
{
    Real result(l.context);
    mpfr_div_d(result.slot->value, l.slot->value, r, result.context->getRoundMode());
    return result;
}

bool operator==(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) == 0;
}

bool operator==(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) == 0;
}
bool operator==(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) != 0;
}

bool operator!=(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) != 0;
}

bool operator!=(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) != 0;
}
bool operator!=(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) != 0;
}

bool operator>(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) > 0;
}

bool operator>(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) < 0;
}
bool operator>(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) > 0;
}

bool operator<(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) < 0;
}

bool operator<(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) > 0;
}
bool operator<(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) < 0;
}

bool operator>=(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) >= 0;
}

bool operator>=(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) <= 0;
}
bool operator>=(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) >= 0;
}

bool operator<=(const Real &l, const Real &r)
{
    return mpfr_cmp(l.slot->value, r.slot->value) <= 0;
}

bool operator<=(const double l, const Real &r)
{
    return mpfr_cmp_d(r.slot->value, l) >= 0;
}
bool operator<=(const Real &l, const double r)
{
    return mpfr_cmp_d(l.slot->value, r) <= 0;
}

bool Real::isNan()
{
    return mpfr_nan_p(this->slot->value);
}

bool Real::isInf()
{
    return mpfr_inf_p(this->slot->value);
}

bool Real::isZero()
{
    return mpfr_zero_p(this->slot->value);
}

void Real::setExponent(long exp)
{
    mpfr_set_exp(this->slot->value, exp);
}

Real Real::abs()
{
    Real result(this->context);
    mpfr_abs(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::pow(const Real &p)
{
    Real result(this->context);
    mpfr_pow(result.slot->value, this->slot->value, p.slot->value, this->context->getRoundMode());
    return result;
}
Real Real::pow(const long p)
{
    Real result(this->context);
    mpfr_pow_si(result.slot->value, this->slot->value, p, this->context->getRoundMode());
    return result;
}
Real Real::exp()
{
    Real result(this->context);
    mpfr_exp(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::exp2()
{
    Real result(this->context);
    mpfr_exp2(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::exp10()
{
    Real result(this->context);
    mpfr_exp10(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::sqrt()
{
    Real result(this->context);
    mpfr_sqrt(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}

Real Real::log()
{
    Real result(this->context);
    mpfr_log(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::log2()
{
    Real result(this->context);
    mpfr_log2(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}
Real Real::log10()
{
    Real result(this->context);
    mpfr_log10(result.slot->value, this->slot->value, this->context->getRoundMode());
    return result;
}

double Real::toDouble()
{
    return mpfr_get_d(this->slot->value, this->context->getRoundMode());
}
Shadow::Shadow()
{
    argAddress = NULL;
}
Shadow::Shadow(Addr address, ArgType t)
{
    argAddress = address;
    type = t;
}
Shadow::Shadow(const Shadow &sourceShadow)
{
    argAddress = sourceShadow.argAddress;
    lastop = sourceShadow.lastop;
    type = sourceShadow.type;
    value = sourceShadow.value;
}
Shadow::Shadow(Addr address, ArgType t, double newValue, std::string lop)
{
    argAddress = address;
    lastop = lop;
    type = t;
    value = newValue;
}
Shadow::Shadow(Addr address, ArgType t, Real newValue, std::string lop)
{
    argAddress = address;
    type = t;
    value = newValue;
    lastop = lop;
}
bool Shadow::updateInfo(Real newValue, std::string lop)
{
    value = newValue;
    lastop = lop;
}
bool Shadow::updateInfo(double newValue, std::string lop)
{
    value = newValue;
    lastop = lop;
}
Real Shadow::getValue()
{
    return value;
}
std::string Shadow::getTypeString()
{
    return getTypeStr(type);
}
ArgType Shadow::getType()
{
    return type;
}
std::string Shadow::getLastOp()
{
    return lastop;
}
Shadow ::~Shadow()
{
}
static hash_map<Addr, Shadow> ShadowMap;
static std::stack<Real> callExpStack;
//
/*
Float map is used to store the float point nums address and its type;
type 0 is float 
type 1 is double 
type 2 is long double
*/
template <class FClass>
static Real getTop(FClass d)
{
    if (oldCallExp == callExp)
        return d;
    else
    {
        oldCallExp = callExp;
        Real top = callExpStack.top();
        callExpStack.pop();
        return top;
    }
}
static hash_map<Addr, int> FloatMap;

static bool shadowExist(Addr address)
{
    hash_map<Addr, Shadow>::iterator iter1;
    iter1 = ShadowMap.find(address);
    if (iter1 == ShadowMap.end())
        return false;
    else
        return true;
}
static void insert_Shadow(Addr addrss, Shadow *shadowvalue);
static void printVarInfo(std::string argStr, Addr addrss)
{
    if (!shadowExist(addrss))
    {
        std::cout << "\n==" << getpid() << "== ";
        std::cout << "sorry there  is no shadow of " << argStr << std::endl;
    }
    else
    {
        Shadow ShadowTmp = ShadowMap[addrss];
        std::cout << "\n==" << getpid() << "== ";
        std::cout << argStr << "`s type is " << ShadowTmp.getTypeString() << ",and it`s address is 0x";
        printf("%X\n", addrss);
        // std::cout << std::setbase(16) << addrss << std::endl;
    }
}
static void printShadowInfo(std::string argStr, Addr addrss)
{
    if (!shadowExist(addrss))
    {
        std::cout << "\n==" << getpid() << "== ";
        std::cout << "sorry there  is no shadow of " << argStr << std::endl;
    }
    else
    {
        Shadow ShadowTmp = ShadowMap[addrss];
        std::cout << "\n==" << getpid() << "== ";
        std::cout << argStr << "`s type is " << ShadowTmp.getTypeString() << ",and it`s address is 0x";
        printf("%X\n", addrss);
        // std::cout << std::setbase(16) << addrss << std::endl;
    }
}
static void getFloatOrigin(Real &originValue, Addr address, ArgType type)
{
    if (type == ArgType::FloatArg)
    {
        float *floatOrigin = (float *)(address);
        originValue = (*floatOrigin);
    }
    else if (type == ArgType::DoubleArg)
    {
        double *doubleOrigin = (double *)(address);
        originValue = (*doubleOrigin);
    }
    else
    {
        long double *longDoubleOrigin = (long double *)(address);
        originValue = (*longDoubleOrigin);
    }
}
static Real computeAbsoluteEror(std::string argStr, Addr address)
{
    Shadow shadowValue = ShadowMap[address];
    Real originReal;
    getFloatOrigin(originReal, address, shadowValue.getType());
    Real absoluteError = shadowValue.getValue() - originReal;
    return absoluteError;
}
static Real computeRelatedError(std::string argStr, Addr address)
{

    Shadow shadowValue = ShadowMap[address];
    Real absoluteError = computeAbsoluteEror(argStr, address);
    //printf
    Real relatedError = (absoluteError / shadowValue.getValue());
    relatedError.abs();
    return relatedError;
}
static Real getReal(std::string argStr, Addr address)
{
    Real desReal;
    if (!shadowExist(address))
    {
        std::cout << "sorry there  is no shadow of " << argStr << std::endl;
    }
    else
    {
        Shadow shadowValue = ShadowMap[address];
        desReal = shadowValue.getValue();
    }
    return desReal;
}
static void printErrorInfo(std::string argStr, Addr address)
{
    using namespace std;
    if (!shadowExist(address))
    {
        std::cout << "sorry there  is no shadow of " << argStr << std::endl;
    }
    else
    {
        std::cout << "\n==" << getpid() << "== The Error Info of " << argStr << endl;
        printVarInfo(argStr, address);
        printShadowInfo(argStr, address);
        Real shadowValue, originReal;
        originReal = ShadowMap[address].getValue();
        getFloatOrigin(originReal, address, ShadowMap[address].getType());
        std::cout << "\n==" << getpid() << "== ";
        mpfr_printf("Shadow Value is %.40Re\n", shadowValue.getRawValue());
        std::cout << "\n==" << getpid() << "== ";
        mpfr_printf("Origin Value is %.40Re\n", originReal.getRawValue());

        Real absoluteError, relatedError;
        absoluteError = (shadowValue - originReal);
        std::cout << "\n==" << getpid() << "== ";
        mpfr_printf("Abusolute Error is %.40Re\n", absoluteError.getRawValue());
        relatedError = (absoluteError / shadowValue).abs();
        std::cout << "\n==" << getpid() << "== ";
        mpfr_printf("Related Error is %.40Re\n", relatedError.getRawValue());
        std::cout << "\n==" << getpid() << "== ";
        cout << "The last operation of " << argStr << " is in " << ShadowMap[address].getLastOp() << endl;
        mpfr_printf("\n_______________________________\n");
    }
}

/**/
static void computeBinary(Real &result, Addr desAddress, std::string desType, Real arg1Real, Real arg2Real, std::string op)
{
    using namespace std;
    if (op == "+")
        result = arg1Real + arg2Real;
    else if (op == "-")
        result = arg1Real - arg2Real;
    else if (op == "*")
        result = arg1Real * arg2Real;
    else if (op == "/")
        result = arg1Real / arg2Real;
    else
    {
        // cout << "There isn`t the method of processing of " << op << endl;
        Real originReal;
        getFloatOrigin(originReal, desAddress, (ArgType)getTypeID(desType));

        result = originReal;
    }
    // mpfr_printf("the result is %.40Re\n", result.getRawValue());
}
/*
set desResult to *arg1Address op *arg2Address 
*/
static void compute(Real &desResult, Addr desAddress, std::string desType, Addr arg1Address, std::string arg1Type, Addr arg2Address, std::string arg2Type, std::string op)
{
    using namespace std;
    Real arg1Real, arg2Real;
    Real origin1Real, origin2Real;
    if (shadowExist(arg1Address))
    {
        arg1Real = ShadowMap[arg1Address].getValue();
    }
    else
    {
        getFloatOrigin(origin1Real, arg1Address, (ArgType)getTypeID(arg1Type));
        arg1Real = origin1Real;
    }

    if (shadowExist(arg2Address))
    {
        arg2Real = ShadowMap[arg2Address].getValue();
    }
    else
    {
        /* code */
        getFloatOrigin(origin2Real, arg2Address, (ArgType)getTypeID(arg2Type));
        arg2Real = origin2Real;
    }
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);

    //mpfr_printf("the origin is %le and the result is %.40Re\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)),desResult->getRawValue());
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/* *arg1Address op real2 */
static void compute(Real &desResult, Addr desAddress, std::string desType, Addr arg1Address, std::string arg1Type, Real arg2Real, std::string op)
{
    using namespace std;
    Real arg1Real;
    Real origin1Real;
    if (shadowExist(arg1Address))
    {
        arg1Real = ShadowMap[arg1Address].getValue();
    }
    else
    {
        getFloatOrigin(origin1Real, arg1Address, (ArgType)getTypeID(arg1Type));
        arg1Real = origin1Real;
    }
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/* real1 op *arg2Address */
static void compute(Real &desResult, Addr desAddress, std::string desType, Real arg1Real, Addr arg2Address, std::string arg2Type, std::string op)
{
    using namespace std;
    Real arg2Real;
    Real origin2Real;
    if (shadowExist(arg2Address))
    {
        arg2Real = ShadowMap[arg2Address].getValue();
    }
    else
    {
        getFloatOrigin(origin2Real, arg2Address, (ArgType)getTypeID(arg2Type));
        arg2Real = origin2Real;
    }
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/*real1 op real2 */
static void compute(Real &desResult, Addr desAddress, std::string desType, Real arg1Real, Real arg2Real, std::string op)
{
    using namespace std;
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/* *arg1Address op *arg2Address */
static void compute(Real &desResult, Addr desAddress, std::string desType, double arg1Double, double arg2Double, std::string op)
{
    using namespace std;
    Real arg1Real, arg2Real;
    arg1Real = arg1Double;
    arg2Real = arg2Double;
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    //    mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/*(double)a op *arg2Address*/
static void compute(Real &desResult, Addr desAddress, std::string desType, double arg1Double, Addr arg2Address, std::string arg2Type, std::string op)
{
    using namespace std;
    Real arg1Real, arg2Real;
    arg1Real = arg1Double;
    Real origin2Real;
    if (shadowExist(arg2Address))
    {
        arg2Real = ShadowMap[arg2Address].getValue();
    }
    else
    {
        getFloatOrigin(origin2Real, arg2Address, (ArgType)getTypeID(arg2Type));
        arg2Real = origin2Real;
    }
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/* *arg1Address op (double)b */
static void compute(Real &desResult, Addr desAddress, std::string desType, Addr arg1Address, std::string arg1Type, double arg2Double, std::string op)
{
    using namespace std;
    Real arg1Real, arg2Real;
    arg2Real = arg2Double;
    Real origin1Real;
    if (shadowExist(arg1Address))
    {
        arg1Real = ShadowMap[arg1Address].getValue();
    }
    else
    {
        getFloatOrigin(origin1Real, arg1Address, (ArgType)getTypeID(arg1Type));
        arg1Real = origin1Real;
    }
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/**/
static void compute(Real &desResult, Addr desAddress, std::string desType, Real arg1Real, double arg2Double, std::string op)
{
    using namespace std;
    Real arg2Real;
    arg2Real = arg2Double;
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}
/**/
static void compute(Real &desResult, Addr desAddress, std::string desType, double arg1Double, Real arg2Real, std::string op)
{
    using namespace std;
    Real arg1Real;
    arg1Real = arg1Double;
    computeBinary(desResult, desAddress, desType, arg1Real, arg2Real, op);
    // mpfr_printf("the origin is %le\n",getFloatOrigin(desAddress,(ArgType)getTypeID(desType)));
    //mpfr_printf(" and Shadow is %.40Re",(&desResult).getRawValue());
}

static void setOrigin(Addr address, std::string type)
{
    if (shadowExist(address))
    {
        if (ShadowMap[address].getType() == ArgType::FloatArg)
        {
            float *floatValue = (float *)(address);
            ShadowMap[address].origin.floatValue = *floatValue;
        }
        else if (ShadowMap[address].getType() == ArgType::DoubleArg)
        {
            double *doubleValue = (double *)(address);
            ShadowMap[address].origin.doubleValue = *doubleValue;
        }
        else
        {
            long double *longDoubleValue = (long double *)(address);
            ShadowMap[address].origin.longdoubleValue = *longDoubleValue;
        }
    }
}

static void mapShadow(Addr address, std::string type, double source, std::string lop)
{
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(source, lop);
    }
    else
    {
        Shadow shadowValue(address, (ArgType)getTypeID(type), source, lop);
        ShadowMap.insert(hash_map<Addr, Shadow>::value_type(address, shadowValue));
    }
    // mpfr_printf("Test509,%.40Re\n",ShadowMap[address].getValue().getRawValue());
    //setOrigin(address,type);
}
static void mapShadow(Addr address, std::string type, Real newValue, std::string lop)
{
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(newValue, lop);
    }
    else
    {
        Shadow shadowValue(address, (ArgType)getTypeID(type), newValue, lop);
        ShadowMap.insert(hash_map<Addr, Shadow>::value_type(address, shadowValue));
    }
    // mpfr_printf("the result is %.40Re\n", ShadowMap[address].getValue().getRawValue());
    // mpfr_printf("Test523,value %.40Re , newvalue %.40Re\n",ShadowMap[address].getValue().getRawValue(),newValue.getRawValue());
    //setOrigin(address,type);
}
static void mapShadow(Addr address, std::string type, Addr source, std::string lop)
{

    if (!shadowExist(source))
    {
        double *doublevalue = (double *)source;
        Shadow shadowValue(source, (ArgType)getTypeID(type), *doublevalue, lop);
        ShadowMap.insert(hash_map<Addr, Shadow>::value_type(source, shadowValue));
    }
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(ShadowMap[source].getValue(), lop);
    }
    else
    {
        Shadow shadowValue(address, (ArgType)getTypeID(type), ShadowMap[source].getValue(), lop);
        ShadowMap.insert(hash_map<Addr, Shadow>::value_type(address, shadowValue));
    }
    // mpfr_printf("Test544,%.40Re\n",ShadowMap[address].getValue().getRawValue());
    //setOrigin(address,type);
}
#endif