﻿// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
// Верхнетреугольная матрица - реализация на основе шаблона вектора

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>
#include <functional>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
    ValType *pVector;
    int Size; // размер вектора

public:
    typedef function<ValType(size_t)> AllocatorFunc;
    TVector(int s = 1, AllocatorFunc fnAlloc = nullptr);
    TVector(const TVector &v); // конструктор копирования
    ~TVector();
    int GetSize() { return Size; } // размер вектора
    ValType& operator[](int pos); // доступ
    bool operator==(const TVector &v) const noexcept; // сравнение
    bool operator!=(const TVector &v) const noexcept; // сравнение
    TVector& operator=(const TVector &v); // присваивание

    // скалярные операции
    TVector  operator+(const ValType &val);   // прибавить скаляр
    TVector  operator-(const ValType &val);   // вычесть скаляр
    TVector  operator*(const ValType &val);   // умножить на скаляр

    // векторные операции
    TVector  operator+(const TVector &v);     // сложение
    TVector  operator-(const TVector &v);     // вычитание
    ValType  operator*(const TVector &v);     // скалярное произведение

    // ввод-вывод
    friend istream& operator>>(istream &in, TVector &v)
    {
    for (int i = 0; i < v.Size; i++)
        in >> v.pVector[i];
    return in;
    }
    friend ostream& operator<<(ostream &out, const TVector &v)
    {
    for (int i = 0; i < v.Size; i++)
        out << v.pVector[i] << ' ';
    return out;
    }
};

template <class ValType>
TVector<ValType>::TVector(int s = 1, AllocatorFunc fnAlloc = nullptr) : Size(s)
{
    if (s < 0 || s > MAX_VECTOR_SIZE) { throw exception("Invalid size"); }

    pVector = fnAlloc ? new ValType[Size] : new ValType[Size]();
    if (fnAlloc)
    {
        for (size_t i = 0; i < Size; i++)
        {
            pVector[i] = fnAlloc(i);
        }
    }
} /*-------------------------------------------------------------------------*/

template <class ValType> //конструктор копирования
TVector<ValType>::TVector(const TVector<ValType> &v): Size(v.Size)
{
    pVector = new ValType[Size]();
    for (int i = 0; i < Size; i++) { pVector[i] = v.pVector[i]; }
} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>::~TVector()
{
    delete[] pVector;
} /*-------------------------------------------------------------------------*/

template <class ValType> // доступ
ValType& TVector<ValType>::operator[](int pos)
{
    if (pos < 0 || pos > Size) { throw exception("Invalid position"); }

    return pVector[pos];
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator==(const TVector &v) const noexcept
{
    if (Size != v.Size) { return false; }

    for (int i = 0; i < Size; i++)
    {
        if (pVector[i] != v.pVector[i]) { return false; }
    }

    return true;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator!=(const TVector &v) const noexcept
{
    return !(*this == v);
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TVector<ValType>& TVector<ValType>::operator=(const TVector &v)
{
    if (this != &v)
    {
        if (Size != v.Size)
        {
            Size = v.Size;
            delete[] pVector;
            pVector = new ValType[Size];
        }
        copy(v.pVector, v.pVector + Size, pVector);
    }
    return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // прибавить скаляр
TVector<ValType> TVector<ValType>::operator+(const ValType &val)
{
    TVector<ValType> temp(Size);

    for (int i = 0; i < Size; i++) { temp[i] = pVector[i] + val; }
    return temp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычесть скаляр
TVector<ValType> TVector<ValType>::operator-(const ValType &val)
{
    TVector<ValType> temp(Size);

    for (int i = 0; i < Size; i++) { temp[i] = pVector[i] - val; }
    return temp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // умножить на скаляр
TVector<ValType> TVector<ValType>::operator*(const ValType &val)
{
    TVector<ValType> temp(Size);

    for (int i = 0; i < Size; i++) { temp[i] = pVector[i] * val; }
    return temp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType> &v)
{
    if (Size != v.Size) { throw exception("Invalid size"); }
    
    TVector<ValType> temp(Size);

    for (int i = 0; i < Size; i++) { temp.pVector[i] = pVector[i] + v.pVector[i]; }
    return temp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType> &v)
{
    if (Size != v.Size) { throw exception("Invalid size"); }

    TVector<ValType> temp(Size);

    for (int i = 0; i < Size; i++) { temp.pVector[i] = pVector[i] - v.pVector[i]; }
    return temp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // скалярное произведение
ValType TVector<ValType>::operator*(const TVector<ValType> &v)
{
    if (Size != v.Size) { throw exception("Invalid size"); }
    
    int result = 0;
    for (int i = 0; i < Size; i++) { result += pVector[i] * v.pVector[i]; }
    
    return result;
} /*-------------------------------------------------------------------------*/


// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType>>
{
public:
    TMatrix(int s = 10);                           
    TMatrix(const TMatrix &mt);                    // копирование
    TMatrix(const TVector<TVector<ValType> > &mt); // преобразование типа
    bool operator==(const TMatrix &mt) const;      // сравнение
    bool operator!=(const TMatrix &mt) const;      // сравнение
    TMatrix& operator= (const TMatrix &mt);        // присваивание
    TMatrix  operator+ (const TMatrix &mt);        // сложение
    TMatrix  operator- (const TMatrix &mt);        // вычитание

    // ввод / вывод
    friend istream& operator>>(istream &in, TMatrix &mt)
    {
    for (int i = 0; i < mt.Size; i++)
        in >> mt.pVector[i];
    return in;
    }
    friend ostream & operator<<( ostream &out, const TMatrix &mt)
    {
    for (int i = 0; i < mt.Size; i++)
        out << mt.pVector[i] << endl;
    return out;
    }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType> >(s, [s](size_t i) { return TVector<ValType>(s); })
{
    if (s < 0 || s > MAX_MATRIX_SIZE) { throw exception("Invalid size"); }
} /*-------------------------------------------------------------------------*/

template <class ValType> // конструктор копирования
TMatrix<ValType>::TMatrix(const TMatrix<ValType> &mt):
  TVector<TVector<ValType> >(mt) {}

template <class ValType> // конструктор преобразования типа
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType> > &mt):
  TVector<TVector<ValType> >(mt) {}

template <class ValType> // сравнение
bool TMatrix<ValType>::operator==(const TMatrix<ValType> &mt) const
{
    if (Size != mt.Size) { return false; }

    for (int i = 0; i < Size; i++)
    {
        if (pVector[i] != mt.pVector[i]) { return false; } 
    }
    return true;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TMatrix<ValType>::operator!=(const TMatrix<ValType> &mt) const
{
    return !(*this == mt);
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType> &mt)
{
    if (this != &mt) { TVector<TVector<ValType>>::operator=(mt); }

    return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType> &mt)
{
    if (Size != mt.Size) { throw exception("Invalid size"); }

    TMatrix<ValType> result(Size);

    for (int i = 0; i < Size; i++) { result.pVector[i] = pVector[i] + mt.pVector[i]; }
    return result;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType> &mt)
{
    if (Size != mt.Size) { throw exception("Invalid size"); }

    TMatrix<ValType> result(Size);

    for (int i = 0; i < Size; i++) { result.pVector[i] = pVector[i] - mt.pVector[i]; }
    return result;
}

// TVector О3 Л2 П4 С6
// TMatrix О2 Л2 П3 С3
#endif
