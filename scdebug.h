
#ifndef _SC_Debug_
#define _SC_Debug_
#include "./mpfr/gmp.h"
#include "./mpfr/mpfr.h"
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <hash_map>
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
using namespace std;
#define DOUBLEFACTOR 134217729
#define ULP 1.0e-30
using namespace __gnu_cxx; //hash_map need
typedef uint64_t Addr;
static long int callExp = 0, oldCallExp = 0;


class Real
{
public:
    double z;
    double zz;
    Real()
    {
        z = 0.0;
        zz = 0.0;
    }
    Real(double zvalue)
    {
        z = zvalue;
        zz = 0.0;
    }
    Real(double zvalue, double zzvalue)
    {
        z = zvalue;
        zz = zzvalue;
    }
};

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
class Shadow
{
private:
    Addr argAddress;
    ArgType type;
    std::string lastop;

public:
    Real value;
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
    double getFloatValue()
    {
        return value.z + value.zz;
    }
    ~Shadow();
};
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


struct valueCache
{
    Addr address;
    Shadow shadowValue;
    // valueCache(){
    //     address = 0x0;
    //     shadowValue = 0x0;
    // }
};

static valueCache shadowCache[256];
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
    else if (callExpStack.size() > 0)
    {
        oldCallExp = callExp;
        Real top = callExpStack.top();
        callExpStack.pop();
        return top;
    }
    else
    {
        return d;
    }
}

template <class valueTye>
inline valueTye getValuebyAddr(Addr address)
{
    valueTye *addrValue = (valueTye *)address;
    // std::cout << *addrValue << std::endl;
    return (*addrValue);
}
//("temp_var_for_ext_0", (Addr)&temp_var_for_ext_0)

static hash_map<Addr, int> FloatMap;

inline bool shadowExist(Addr address)
{
    if (ShadowMap.find(address) == ShadowMap.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}
inline bool shadowExist(Addr address, Shadow &shadowValue)
{
    int mask = address & 0xff;
    valueCache &valueInCache = shadowCache[mask];
    if (address == valueInCache.address)
    {
        shadowValue = (valueInCache.shadowValue);
        return true;
    }
    else
    {
        hash_map<Addr, Shadow>::iterator findIt = ShadowMap.find(address);
        if (findIt == ShadowMap.end())
        {
            return false;
        }
        else
        {
            shadowValue = findIt->second;
            valueInCache.address = address;
            valueInCache.shadowValue = shadowValue;
            return true;
        }
    }
}

inline Real getReal(std::string varName, Addr address)
{
    Real desReal;
    Shadow shadowValue;
    if (shadowExist(address, shadowValue))
    {
        desReal = shadowValue.getValue();
    }
    else
    {
        // std::cout << "sorry there  is no shadow of " << varName << std::endl;
    }
    return desReal;
}
template <class Type>
Type fabs(Type value)
{
    return value > 0.0 ? value : -value;
}

template <class Type>
Type max(Type value1, Type value2)
{
    return value1 > value2 ? value1 : value2;
}

inline double getAbsError(Addr address)
{
    Shadow ShadowTmp = ShadowMap[address];
    double origin = getValuebyAddr<double>(address);
    double reminder = fabs((ShadowTmp.value.z - origin) + ShadowTmp.value.zz) / fabs(ShadowTmp.value.z);
    return reminder;
}
inline void printVarInfo(std::string argStr, Addr address)
{
    Shadow ShadowTmp;
    if (!shadowExist(address, ShadowTmp))
    {
        std::cout << "\n==" << getpid() << "== ";
        std::cout << "sorry there  is no shadow of " << argStr << std::endl;
    }
    else
    {
        std::cout << "\n==" << getpid() << "== ";
        std::cout << argStr << "`s type is " << ShadowTmp.getTypeString() << ",and it`s address is 0x";
        printf("%X\n", address);
        // std::cout << "Origin Value:" << getValuebyAddr<double>(address) << std::endl;
        double origin = getValuebyAddr<double>(address);
        printf("Origin Value:%.20le\n", origin);
        printf("ShadowValue: higher=%.20le remainder=%.20le\n", ShadowTmp.value.z, ShadowTmp.value.zz);
        double reminder = fabs((ShadowTmp.value.z - origin) + ShadowTmp.value.zz);
        printf("Absolutely Error:%.20le\n", reminder);
        printf("Related Error:%.20le\n", reminder / fabs(ShadowTmp.value.z));
        // std::cout<<ShadowTmp.getLastOp()<<endl;
        printf("LastOp:%s\n", ShadowTmp.getLastOp().c_str());
    }
}

template <class twoSumType>
inline Real TwoSum(twoSumType a, twoSumType b)
{
    Real fpp;
    fpp.z = a + b;
    double z = fpp.z - a;
    fpp.zz = a - (fpp.z - z) + (b - z);
    return fpp;
}

template <class subType>
inline Real Sub2(vector<Addr> &p) // cost 7 times
{
    int len = p.size();
    if (len == 0)
    {
        return 0;
    }
    double s, e, y;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(p[0], shadowValueLeft))
    {
        s = shadowValueLeft.value.z;
        e = shadowValueLeft.value.zz;
    }
    else
    {
        s = getValuebyAddr<subType>(p[0]);
        e = 0.0;
    }
    if (!shadowExist(p[1], shadowValueRight))
    {
        Real fpp = TwoSum<subType>(s, -getValuebyAddr<subType>(p[1]));
        s = fpp.z;
        e += fpp.zz;
    }
    else
    {
        Real fpp = TwoSum<subType>(s, -shadowValueRight.value.z);
        s = fpp.z;
        e += fpp.zz - shadowValueRight.value.zz;
    }
    return Real(s, e);
}

template <class divType>
inline Real Div2(vector<Addr> &p) // cost 7 times
{
    Real fpp;
    int len = p.size();
    if (len == 0)
    {
        return 0;
    }
    double s, e, y;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(p[1]))
    {
        s = shadowValueRight.value.z;
        e = shadowValueRight.value.zz;
    }
    else
    {
        s = getValuebyAddr<divType>(p[1]);
        e = 0.0;
    }
    if (s > -ULP && s < ULP)
        s = ULP;
    if (!shadowExist(p[0]))
    {
        fpp.z = getValuebyAddr<divType>(p[0]) / (s + e);
    }
    else
    {
        fpp.z = shadowValueLeft.value.z / (s + e);
        fpp.zz = shadowValueLeft.value.zz / (s + e);
    }
    return fpp;
}

template <class sumType>
inline Real Sum2(vector<Addr> &p) // cost 7 times
{
    int len = p.size();
    if (len == 0)
    {
        return 0;
    }
    double s, e, y;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(p[0], shadowValueLeft))
    {
        s = shadowValueLeft.value.z;
        e = shadowValueLeft.value.zz;
    }
    else
    {
        s = getValuebyAddr<sumType>(p[0]);
        e = 0.0;
    }
    for (int i = 1; i < len; i++)
    {
        if (!shadowExist(p[i], shadowValueRight))
        {
            Real fpp = TwoSum<sumType>(s, getValuebyAddr<sumType>(p[i]));
            s = fpp.z;
            e += fpp.zz;
        }
        else
        {
            Real fpp = TwoSum<sumType>(s, shadowValueRight.value.z);
            s = fpp.z;
            e += fpp.zz + shadowValueRight.value.zz;
        }
    }
    return Real(s, e);
}
inline Real split(double x)
{
    double c = DOUBLEFACTOR * x;
    Real fpp;
    fpp.z = c - (c - x);
    fpp.zz = x - fpp.z;
    return fpp;
}
inline Real TwoProduct(Real fp1, Real fp2)
{
    Real fpp;
    double p, q;
    p = fp1.z * fp2.z;
    q = fp1.z * fp2.zz + fp2.z * fp1.zz;
    fpp.z = p + q;
    // zz = ((p - z) + q) + tx * ty;
    fpp.zz = ((p - fpp.z) + q) + fp1.zz * fp2.zz;
    return fpp;
}
inline Real TwoProduct(Real fp1, double para2)
{
    Real fpp, fp2;
    double p, q;
    fp2 = split(para2);
    p = fp1.z * fp2.z;
    q = fp1.z * fp2.zz + fp2.z * fp1.zz;
    fpp.z = p + q;
    // zz = ((p - z) + q) + tx * ty;
    fpp.zz = ((p - fpp.z) + q) + fp1.zz * fp2.zz;
    return fpp;
}
inline Real TwoProduct(double para1, Real fp2)
{
    Real fpp, fp1;
    double p, q;
    fp1 = split(para1);
    p = fp1.z * fp2.z;
    q = fp1.z * fp2.zz + fp2.z * fp1.zz;
    fpp.z = p + q;
    // zz = ((p - z) + q) + tx * ty;
    fpp.zz = ((p - fpp.z) + q) + fp1.zz * fp2.zz;
    return fpp;
}
inline Real TwoProduct(double para1, double para2)
{
    Real fpp, fp1, fp2;
    double p, q;
    fp1 = split(para1);
    fp2 = split(para2);
    p = fp1.z * fp2.z;
    q = fp1.z * fp2.zz + fp2.z * fp1.zz;
    fpp.z = p + q;
    // zz = ((p - z) + q) + tx * ty;
    fpp.zz = ((p - fpp.z) + q) + fp1.zz * fp2.zz;
    return fpp;
}
inline Real prod_dd_dd(Real fp1, Real fp2)
{
    Real t = TwoProduct(fp1.z, fp2.z);
    double t3;
    if ((fp1.zz != 0.0) && (fp2.zz != 0.0))
        t3 = fp1.z * fp2.zz + fp1.zz * fp2.z + t.zz;
    else if (fp2.zz != 0.0)
    {
        t3 = fp1.z * fp2.zz + t.zz;
    }
    else if (fp1.zz != 0.0)
    {
        t3 = fp2.z * fp1.zz + t.zz;
    }
    else
    {
        return t;
    }
    Real r = TwoSum(t.z, t3);
    return r;
}
template <class valueType>
inline Real Mul2(vector<Addr> &p)
{
    int len = p.size();
    Real fpp(1.0), temp(0.0);
    Shadow shadowValueRight;
    for (int i = 0; i < len; i++)
    {
        if (shadowExist(p[i], shadowValueRight))
            temp = shadowValueRight.value;
        else
            temp = Real(getValuebyAddr<valueType>(p[i]));
        fpp = prod_dd_dd(fpp, temp);
    }
    return fpp;
}

static void insert_Shadow(Addr addrss, Shadow *shadowvalue);

static inline void computeDAdd(Addr address, vector<Addr> paras, std::string s)
{
    Real realValue = Sum2<double>(paras);
    // std::cout << "higher:" << realValue.z << "remainder:" << realValue.zz << std::endl;
    Shadow shadowValueOld;
    if (shadowExist(address, shadowValueOld))
    {
        shadowValueOld.updateInfo(realValue, s);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, s);
    }
}
static inline void computeDAdd2(Addr address, Addr paraLeft, Addr paraRight, std::string posiStr) // bind para nums
{
    double s, e, y;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(paraLeft, shadowValueLeft))
    {
        s = shadowValueLeft.value.z;
        e = shadowValueLeft.value.zz;
    }
    else
    {
        s = getValuebyAddr<double>(paraLeft);
        e = 0.0;
    }
    if (!shadowExist(paraRight, shadowValueRight))
    {
        double valuePara1 = getValuebyAddr<double>(paraRight);
        Real fpp;
        // = TwoSum<double>(s, -getValuebyAddr<double>(p[1]));
        fpp.z = s - valuePara1;
        double z = fpp.z - s;
        fpp.zz = s - (fpp.z - z) + (valuePara1 - z);
        s = fpp.z;
        e += fpp.zz;
    }
    else
    {
        //Real fpp = TwoSum<double>(s, -shadowValueRight.value.z);
        double valuePara1 = getValuebyAddr<double>(paraRight);
        Real fpp;
        // = TwoSum<double>(s, -getValuebyAddr<double>(p[1]));
        fpp.z = s - valuePara1;
        double z = fpp.z - s;
        fpp.zz = s - (fpp.z - z) + (valuePara1 - z);
        s = fpp.z;
        e += fpp.zz + shadowValueRight.value.zz;
    }
    Real realValue(s, e);
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, posiStr);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, posiStr);
    }
}
static inline void computeDSub2(Addr address, Addr paraLeft, Addr paraRight, std::string posiStr) // bind para nums
{
    double s, e, y;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(paraLeft, shadowValueLeft))
    {
        s = shadowValueLeft.value.z;
        e = shadowValueLeft.value.zz;
    }
    else
    {
        s = getValuebyAddr<double>(paraLeft);
        e = 0.0;
    }
    if (!shadowExist(paraRight, shadowValueRight))
    {
        double valuePara1 = getValuebyAddr<double>(paraRight);
        Real fpp;
        // = TwoSum<double>(s, -getValuebyAddr<double>(p[1]));
        fpp.z = s - valuePara1;
        double z = fpp.z - s;
        fpp.zz = s - (fpp.z - z) - (valuePara1 + z);
        s = fpp.z;
        e += fpp.zz;
    }
    else
    {
        //Real fpp = TwoSum<double>(s, -shadowValueRight.value.z);
        double valuePara1 = getValuebyAddr<double>(paraRight);
        Real fpp;
        // = TwoSum<double>(s, -getValuebyAddr<double>(p[1]));
        fpp.z = s - valuePara1;
        double z = fpp.z - s;
        fpp.zz = s - (fpp.z - z) - (valuePara1 + z);
        s = fpp.z;
        e += fpp.zz - shadowValueRight.value.zz;
    }
    Real realValue(s, e);
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, posiStr);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, posiStr);
    }
}
static inline void computeDSub(Addr address, vector<Addr> paras, std::string s)
{
    Real realValue = Sub2<double>(paras);
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, s);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, s);
    }
    // std::cout << s << std::endl;
}

static inline void computeDMul2(Addr address, Addr paraLeft, Addr paraRight, std::string s)
{
    Real realValue;
    Shadow shadowValueLeft, shadowValueRight;
    if (shadowExist(paraLeft, shadowValueLeft) && shadowExist(paraRight, shadowValueRight))
    {
        // realValue = TwoProduct(ShadowMap[paraLeft].value, ShadowMap[paraRight].value);
        Real fp1, fp2;
        fp1 = shadowValueLeft.value;
        fp2 = shadowValueRight.value;
        double p, q;
        p = fp1.z * fp2.z;
        q = fp1.z * fp2.zz + fp2.z * fp1.zz;
        realValue.z = p + q;
        // zz = ((p - z) + q) + tx * ty;
        realValue.zz = ((p - realValue.z) + q) + fp1.zz * fp2.zz;
    }
    else if (shadowExist(paraLeft, shadowValueLeft))
    {
        // realValue = TwoProduct(ShadowMap[paraLeft].value, getValuebyAddr<double>(paraRight));
        double para2;
        para2 = getValuebyAddr<double>(paraRight);
        Real fp1, fp2;
        fp1 = shadowValueLeft.value;
        double p, q;
        double c = DOUBLEFACTOR * para2;
        fp2.z = c - (c - para2);
        fp2.zz = para2 - fp2.z;
        // fp2 = split(para2);
        p = fp1.z * fp2.z;
        q = fp1.z * fp2.zz + fp2.z * fp1.zz;
        realValue.z = p + q;
        realValue.zz = ((p - realValue.z) + q) + fp1.zz * fp2.zz;
    }
    else if (shadowExist(paraRight, shadowValueRight))
    {
        // realValue = TwoProduct(getValuebyAddr<double>(paraLeft), ShadowMap[paraRight].value);
        double para2;
        para2 = getValuebyAddr<double>(paraLeft);
        Real fp1, fp2;
        fp1 = shadowValueRight.value;
        double p, q;
        double c = DOUBLEFACTOR * para2;
        fp2.z = c - (c - para2);
        fp2.zz = para2 - fp2.z;
        // fp2 = split(para2);

        p = fp1.z * fp2.z;
        q = fp1.z * fp2.zz + fp2.z * fp1.zz;
        realValue.z = p + q;
        realValue.zz = ((p - realValue.z) + q) + fp1.zz * fp2.zz;
    }
    else
    {
        // realValue = TwoProduct(getValuebyAddr<double>(paraLeft), getValuebyAddr<double>(paraRight));

        Real fp1, fp2;
        double para1, para2;
        para1 = getValuebyAddr<double>(paraLeft);
        para2 = getValuebyAddr<double>(paraRight);

        double p, q;

        // fp1 = split(para1);

        double c = DOUBLEFACTOR * para1;
        fp1.z = c - (c - para1);
        fp1.zz = para1 - fp1.z;

        // fp2 = split(para2);

        c = DOUBLEFACTOR * para2;
        fp2.z = c - (c - para2);
        fp2.zz = para2 - fp2.z;

        p = fp1.z * fp2.z;
        q = fp1.z * fp2.zz + fp2.z * fp1.zz;
        realValue.z = p + q;
        // zz = ((p - z) + q) + tx * ty;
        realValue.zz = ((p - realValue.z) + q) + fp1.zz * fp2.zz;
    }
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, s);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, s);
    }
}
static inline void computeDMul3(Addr address, vector<Addr> paras, std::string s)
{
}
static inline void computeDMul(Addr address, vector<Addr> paras, std::string s)
{
    Real realValue;
    if (paras.size() > 2)
        realValue = Mul2<double>(paras);
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, s);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, s);
    }
    // getValuebyAddr<double>(p[0]);
    // std::cout << s << std::endl;
}

static inline void computeDDiv2(Addr address, Addr paraLeft, Addr paraRight, std::string posiStr)
{
    // Real realValue = Div2<double>(paras);
    Real realValue;
    Shadow shadowValueLeft, shadowValueRight;
    double s, e, y;
    if (shadowExist(paraRight, shadowValueRight))
    {
        s = shadowValueRight.value.z;
        e = shadowValueRight.value.zz;
    }
    else
    {
        s = getValuebyAddr<double>(paraRight);
        e = 0.0;
    }
    if (s > -ULP && s < ULP)
        s = ULP;
    if (!shadowExist(paraLeft, shadowValueLeft))
    {
        realValue.z = getValuebyAddr<double>(paraLeft) / (s + e);
    }
    else
    {
        realValue.z = shadowValueLeft.value.z / (s + e);
        realValue.zz = shadowValueLeft.value.zz / (s + e);
    }

    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, posiStr);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, posiStr);
    }
    // std::cout << s << std::endl;
}
static inline void computeDDiv(Addr address, vector<Addr> paras, std::string s)
{
    Real realValue = Div2<double>(paras);
    if (shadowExist(address))
    {
        ShadowMap[address].updateInfo(realValue, s);
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, realValue, s);
    }
    // std::cout << s << std::endl;
}

static void computeFAdd(Addr address, vector<Addr> paras, std::string s)
{

    // std::cout << s << std::endl;
}
static void computeFSub(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeFMul(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeFDiv(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeLdAdd(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeLdSub(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeLdMul(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

static void computeLdDiv(Addr address, vector<Addr> paras, std::string s)
{
    // std::cout << s << std::endl;
}

/**/

//statment.append("computeadd(&").append(varName)
//.append(",{").append(paraStr).append("},").append(toString(biExpr->getExprLoc())).append(");");

//  Assignment({(Addr)&g, (Addr)&f, (Addr)&e, (Addr)&b, (Addr)&d}, (Addr)&h,
//              "/home/shijia/Public/testprogram/testVar.c_e.c:28:7");
static void AssignD(vector<Addr> des, Addr sour, std::string s)
{
    if (shadowExist(sour))
    {
        for (int i = 0; i < des.size(); i++)
        {
            if (shadowExist(des[i]))
                ShadowMap[des[i]].updateInfo(ShadowMap[sour].value, s);
            else
            {
                ShadowMap[des[i]] = Shadow(des[i], ArgType::DoubleArg, ShadowMap[sour].value, s);
            }
        }
    }
    else
    {
        for (int i = 0; i < des.size(); i++)
        {
            if (shadowExist(des[i]))
                ShadowMap[des[i]].updateInfo(getValuebyAddr<double>(sour), s);
            else
            {
                ShadowMap[des[i]] = Shadow(des[i], ArgType::DoubleArg, getValuebyAddr<double>(sour), s);
            }
        }
    }
}
static void computeDCom(Addr address, vector<Addr> paras, std::string s)
{
    if (shadowExist(paras[1]))
    {
        if (shadowExist(address))
            ShadowMap[address].updateInfo(getValuebyAddr<double>(paras[1]), s);
        else
        {
            ShadowMap[address] = ShadowMap[paras[1]];
        }
    }
}
static void CallStackPop(Addr address, Real arg)
{
    if (shadowExist(address))
        ShadowMap[address].updateInfo(arg, "");
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, arg, "");
    }
}
template <class T>
static inline T getFVbyShadow(Addr address)
{
    if (shadowExist(address))
    {
        return ShadowMap[address].getFloatValue();
    }
    else
    {
        // std::cout << "Sorry,there is no shadow of the ";
        // printf("%X\n", address);
        return getValuebyAddr<T>(address);
    }
}

template <class T>
inline T SC_DEBUG_POSTINC(Addr address, T &value)
{
    T tempValue = value;
    if (shadowExist(address))
    {
        ShadowMap[address].value.z += 1;
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, value + 1, "");
    }
    value = value + 1;
    return tempValue;
}

template <class T>
inline T SC_DEBUG_POSTFIX(Addr address, T &value)
{
    T tempValue = value;
    if (shadowExist(address))
    {
        ShadowMap[address].value.z -= 1;
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, value - 1, "");
    }
    value = value - 1;
    return tempValue;
}

template <class T>
inline T SC_DEBUG_PREINC(Addr address, T &value)
{
    if (shadowExist(address))
    {
        ShadowMap[address].value.z += 1;
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, value + 1, "");
    }
    value = value + 1;
    return value;
}

template <class T>
inline T SC_DEBUG_PREFIX(Addr address, T &value)
{
    if (shadowExist(address))
    {
        ShadowMap[address].value.z -= 1;
    }
    else
    {
        ShadowMap[address] = Shadow(address, ArgType::DoubleArg, value - 1, "");
    }
    value = value - 1;
    return value;
}

static void ReMapD(Addr address)
{
    ShadowMap[address].value = Real(getValuebyAddr<double>(address));
}
static void ReMapF(Addr address)
{
}
static void ReMapLf(Addr address)
{
}
#endif