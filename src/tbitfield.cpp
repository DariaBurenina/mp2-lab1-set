// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen=len;
	if (len<1)
		throw "Error";
	else
	{	
	  MemLen = (BitLen + (sizeof(TELEM))*8-1)/(sizeof(TELEM)*8);	
	  pMem = new TELEM[MemLen];
		for(int i=0; i < MemLen; i++)
			 pMem[i]=0;	
	}
}



TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem = new TELEM [MemLen];
	if(pMem!=NULL)
		for(int i=0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
}


TBitField::~TBitField()
{
	delete []pMem;
}


int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n/(sizeof(TELEM)*8));
}


TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n ????
{
	int pos = n % (sizeof(TELEM)*8);
	return 1<<pos;
}


// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}


void TBitField::SetBit(const int n) // установить бит
{
	if ((n<0) || (n>BitLen))
		throw "Error";
	else
		pMem[GetMemIndex(n)] |= GetMemMask(n);
}


void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n<0) || (n>BitLen))
		throw "Error";
	else
	{
		int index =GetMemIndex(n);
		TELEM mask = ~GetMemMask(n);
		pMem[index] &= mask;
	}
}


int TBitField::GetBit(const int n) const // получить значение бита ????
{
	if((n<0)||(n>BitLen))
		throw "Error";
	else
	{
	TELEM temp = GetMemMask(n);
	if((pMem[GetMemIndex(n)]&temp)==0)
		return 0;
	else
		return 1;
	}
}


// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание ?????
{
	if(pMem != bf.pMem)
	{
		if (BitLen != bf.BitLen)
		{
			delete[]pMem;
			BitLen = bf.BitLen;
			MemLen = bf.MemLen;
			pMem = new TELEM[MemLen];
		}
		for( int i = 0; i < MemLen; i ++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}




int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res=1;
	if((BitLen != bf.BitLen)||(MemLen != bf.MemLen))
		res=0;
	else
	{
		for(int i=0;i < MemLen;i++)
			if(pMem[i] != bf.pMem[i])
			{
				res=0;
				break;
			}
  return res;
	}
}


int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = bf.BitLen;
	if (BitLen <= len)
	{
		TBitField temp(bf);
	    for (int i=0;i < MemLen; i++)
			temp.pMem[i] |= pMem[i];
		return temp;
	}
	else
	{
		TBitField temp(*this);
	    for (int i=0;i < bf.MemLen; i++)
			temp.pMem[i] |= bf.pMem[i];
		return temp;
	}
}


TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if ( bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	return temp;
}



TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for(int i = 0; i < BitLen; i++)
	{
		if(this->GetBit(i) == 0)
			temp.SetBit(i);
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{ 
	int i;
	char c;
	do
	{
		istr>>c;
	}
	while (c != ')');
	while (1)
	{
		istr>>c;
		if (c == '0')
			bf.ClrBit(i++);
		else
			if (c == '1')
				bf.SetBit(i++);
			else 
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i=0; i < len; i++)
		//ostr << bf.GetBit(i);
		if ( bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0'; 
		return ostr;
}
