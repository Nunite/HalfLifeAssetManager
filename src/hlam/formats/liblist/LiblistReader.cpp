#include <fstream>
#include <streambuf>

#include "LiblistReader.hpp"
#include "utility/IOUtils.hpp"
#include "utility/Tokenizer.hpp"

std::optional<std::unordered_map<std::string, std::string>> LiblistReader::Read(const std::string& fileName)
{
	// liblist.gam files have comments and a list of keyvalue pairs in the format "key \"value\""

	std::unique_ptr<FILE, decltype(::fclose)*> cFile{utf8_fopen(fileName.c_str(), "rb"), &::fclose};

	if (!cFile)
	{
		return {};
	}

	std::string contents;

	fseek(cFile.get(), 0, SEEK_END);
	contents.resize(ftell(cFile.get()));
	fseek(cFile.get(), 0, SEEK_SET);

	if (fread(contents.data(), 1, contents.size(), cFile.get()) != contents.size())
	{
		return {};
	}

	std::unordered_map<std::string, std::string> keyvalues;

	Tokenizer tokenizer{contents};

	while (true)
	{
		const auto key = tokenizer.Next();
		const auto value = tokenizer.Next();

		if (!key || !value)
		{
			break;
		}

		keyvalues.insert_or_assign(std::string{*key}, *value);
	}

	return keyvalues;
}
