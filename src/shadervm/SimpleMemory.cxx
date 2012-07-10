#include "SimpleMemory.hxx"
#include <memory.h>

SimpleMemory::SimpleMemory()
{
	m_size = m_maxsize = 0;
	m_mem = 0;
}

SimpleMemory::~SimpleMemory()
{
	delete [] m_mem;
}

void SimpleMemory::ReserveMore(unsigned int size)
{
	if (m_size+size<m_maxsize)
		return;


	while (m_size+size>=m_maxsize) {
		if (m_maxsize==0)
			m_maxsize = 16;
		else
			m_maxsize *= 2;
	}

	char *newmem = new char[m_maxsize];
	if (m_mem!=0) {
		memcpy(newmem, m_mem, m_size);
		delete [] m_mem;
	}
	m_mem = newmem;
}

void SimpleMemory::Reserve(unsigned int size)
{
	if (m_maxsize>=size)
		return;

	char *newmem = new char[size];
	if (m_mem!=0) {
		memcpy(newmem, m_mem, m_size);
		delete [] m_mem;
	}
	m_mem = newmem;
	m_maxsize = size;
}
