#include "Storage.hpp"

WS::Storage::Storage():
        m_storage(new WS::Byte[100]),
        m_storedSize(0),
        m_storageSize(100)
{
}

WS::Storage::~Storage()
{
  delete[] (m_storage);
}

size_t WS::Storage::find(WS::Byte needle) const
{
  if (this->empty())
    return (WS::Storage::NOT_FOUND);
  return (static_cast<WS::Byte*>(memchr(m_storage, needle, m_storedSize)) - this->m_storage);
}

static bool isMatched(const WS::Byte* bytes, const std::string& needle, const size_t byteSize)
{
  if (needle.size() < byteSize)
    return (false);
  for (size_t idx = 0; (idx < needle.size() && idx < byteSize); ++idx)
  {
    if (bytes[idx] != needle[idx])
      return (false);
  }
  return (true);
}

size_t WS::Storage::find(const std::string& needle) const
{
  if (this->empty())
    return (WS::Storage::NOT_FOUND);
  for (size_t idx = 0; idx < m_storedSize; ++idx)
  {
    if (isMatched(&m_storage[idx], needle, (m_storedSize - idx)))
      return (idx);
  }
  return (WS::Storage::NOT_FOUND);
}

size_t WS::Storage::rfind(const std::string& needle) const
{
  if (this->empty())
    return (WS::Storage::NOT_FOUND);
  for (size_t idx = m_storedSize - 1; ; --idx)
  {
    if (isMatched(&m_storage[idx], needle, (m_storedSize - idx)))
      return (idx);
    if (idx == 0)
      return (WS::Storage::NOT_FOUND);
  }
}

size_t WS::Storage::rfind(WS::Byte needle) const
{
  if (this->empty())
    return (WS::Storage::NOT_FOUND);
  for (size_t idx = m_storedSize - 1; ; --idx)
  {
    if (m_storage[idx] == needle)
    {
      return (idx);
    }
    if (idx == 0)
      return (WS::Storage::NOT_FOUND);
  }
}

size_t WS::Storage::size() const
{
  return (m_storedSize);
}

bool WS::Storage::empty() const
{
  return (m_storedSize == 0);
}

void WS::Storage::clear()
{
  m_storedSize = 0;
}

void WS::Storage::append(WS::Byte* buf, size_t size)
{
  if (buf == nullptr || size == 0)
    return ;
  const size_t NEW_STORED_SIZE = m_storedSize + size;
  const size_t NEW_STORAGE_SIZE = m_storageSize + size;

  reserve(NEW_STORAGE_SIZE);
  memcpy(&m_storage[m_storedSize], buf, size);
  m_storedSize = NEW_STORED_SIZE;
}

WS::Storage WS::Storage::operator+(const std::string& str)
{
  WS::Storage newStore = *this;

  newStore.append((WS::Byte*) str.c_str(), str.size());
  return (newStore);
}

WS::Storage WS::Storage::operator+(WS::CString str)
{
  if (str == nullptr)
    return (*this);
  WS::Storage newStore = *this;

  newStore.append((WS::Byte*)str, strlen(str) + 1);
  return (newStore);
}

WS::Storage WS::Storage::operator+(const WS::Storage& str)
{
  WS::Storage newStore = *this;

  newStore.append(str.m_storage, str.m_storedSize);
  return (newStore);
}

WS::Storage WS::Storage::operator+(ssize_t n)
{
  return (*this + std::to_string(n));
}

WS::Storage WS::Storage::operator+(size_t n)
{
  return (*this + std::to_string(n));
}

WS::Storage WS::Storage::operator+(double n)
{
  return (*this + std::to_string(n));
}

WS::Storage& WS::Storage::operator+=(const std::string& str)
{
  *this = *this + str;
  return (*this);
}

WS::Storage& WS::Storage::operator+=(WS::CString str)
{
  *this = *this + str;
  return (*this);
}

WS::Storage& WS::Storage::operator+=(const WS::Storage& str)
{
  *this = *this + str;
  return (*this);
}

WS::Storage& WS::Storage::operator+=(ssize_t n)
{
  *this = *this + n;
  return (*this);
}

WS::Storage& WS::Storage::operator+=(size_t n)
{
  *this = *this + n;
  return (*this);
}

WS::Storage& WS::Storage::operator+=(double n)
{
  *this = *this + n;
  return (*this);
}

WS::Storage& WS::Storage::operator=(const WS::Storage& rhs)
{
  if (&rhs == this)
    return (*this);

  reserve(rhs.m_storageSize);
  if (!rhs.empty())
    memcpy(this->m_storage, rhs.m_storage, rhs.m_storedSize);
  this->m_storedSize = rhs.m_storedSize;
  return (*this);
}

WS::Storage& WS::Storage::operator=(const std::string& rhs)
{
  clear();
  append((Byte*) rhs.c_str(), rhs.size());
  return (*this);
}

WS::Storage& WS::Storage::operator=(WS::CString rhs)
{
  clear();
  append((Byte*) rhs, strlen(rhs) + 1);
  return (*this);
}

WS::Byte& WS::Storage::operator[](const size_t pos) const
{
  if (pos > m_storedSize)
    throw (std::runtime_error("Storage overflow\n"));
  return (m_storage[pos]);
}

WS::Storage WS::Storage::subStorage(size_t start, size_t len) const
{
  if (len == 0)
    return (WS::Storage());
  if (start == 0 && len >= m_storedSize)
  {
    WS::Storage newStorage;
    newStorage = *this;
    return (newStorage);
  }
  else
  {
    WS::Storage newStorage;
    newStorage.reserve(len);
    memcpy(newStorage.m_storage, &m_storage[start], len);
    newStorage.m_storedSize = len;
    return (newStorage);
  }
}

void WS::Storage::pop(size_t len)
{
  if (m_storedSize == 0 || m_storageSize == 0)
    return ;
  if (m_storedSize <= len)
  {
    m_storedSize = 0;
  }
  else
  {
    memmove(m_storage, &m_storage[len], (m_storageSize - len));
    m_storedSize = m_storageSize - len;
  }
}

void WS::Storage::reserve(size_t size)
{
  if (m_storageSize > size || size == 0)
    return ;
  else
  {
    auto newStorage = new WS::Byte[size];
    size_t temp = m_storedSize;

    memcpy(newStorage, m_storage, m_storedSize);
    delete[] (m_storage);
    m_storage = newStorage;
    m_storedSize = temp;
    m_storageSize = size;
  }
}

const WS::Byte* WS::Storage::data() const
{
  return (m_storage);
}

std::ostream& operator<<(std::ostream& stream, const WS::Storage& buf)
{
  const size_t BUF_SIZE = buf.size();

  for (size_t idx = 0; idx < BUF_SIZE; ++idx)
  {
    stream << buf[idx];
  }
  return (stream);
}