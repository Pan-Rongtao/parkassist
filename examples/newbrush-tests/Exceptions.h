#include <exception>
#include <string>
#include "spdlog/fmt/fmt.h"

namespace nb {

class Exception : public std::exception
{
public:
	Exception() = default;
	Exception(const std::string &msg)
		: m_msg(msg)
	{
	}

protected:
	virtual char const* what() const
	{
		return m_msg.data();
	}

	std::string m_msg;
};

class FileNotExistsException : public Exception
{
public:
	FileNotExistsException(const std::string &path)
	{
		m_msg = "can't find [" + path + "]";
	}

};

class JsonParsingException : public Exception
{
public:
	JsonParsingException(const std::string &filePath, const std::string &errorMsg)
	{
		m_msg = fmt::format("[{}] syntax error.\n\t{}", filePath, errorMsg);
	}
};

class InvalidArraySizeException : public Exception
{
public:
	InvalidArraySizeException()
	{
	}
};

class InvalidArrayValueException : public Exception
{
public:
	InvalidArrayValueException(int invalidIndex)
		: InvalidIndex(invalidIndex)
	{
		m_msg = fmt::format("invalidIndex({})", invalidIndex);
	}

	int InvalidIndex;

};

}

