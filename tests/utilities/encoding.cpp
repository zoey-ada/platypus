#include <list>

#include <catch2/catch.hpp>
#include <utilities/encoding.hpp>

std::list<std::pair<const char*, const char32_t*>> words {
	{"κόσμε", U"\U000003ba\U00001f79\U000003c3\U000003bc\U000003b5"},
	{"unique New York",
		U"\U00000075\U0000006e\U00000069\U00000071\U00000075\U00000065\U00000020\U0000004e"
		U"\U00000065\U00000077\U00000020\U00000059\U0000006f\U00000072\U0000006b"},
	{"ありがと", U"\U00003042\U0000308a\U0000304c\U00003068"},
	{"Пошли", U"\U0000041f\U0000043e\U00000448\U0000043b\U00000438"}};

SCENARIO("Can convert between utf-8 and utf-32", "[utilities, encoding]")
{
	for (const auto& data : words)
	{
		std::string label1("utf-8 string ");
		label1.append(data.first);
		GIVEN(label1.c_str())
		{
			std::string message(data.first);

			WHEN("attempting to convert to utf-32")
			{
				auto translated_message = Encoding::toUtf32(message);

				THEN("string is correctly converted")
				{
					REQUIRE(translated_message.compare(data.second) == 0);
				}
			}
		}

		std::string label2("utf-32 string ");
		label2.append(data.first);
		GIVEN(label2.c_str())
		{
			std::u32string message(data.second);

			WHEN("attempting to convert to utf-32")
			{
				auto translated_message = Encoding::toUtf8(message);

				THEN("string is correctly converted")
				{
					REQUIRE(translated_message.compare(data.first) == 0);
				}
			}
		}
	}
}

#ifdef _WIN32

SCENARIO("Can convert between utf-8 and Windows wide strings", "[utilities, encoding, windows]")
{
	GIVEN("wide string (utf-16)")
	{
		std::wstring message(L"words words words");

		WHEN("attempting to convert to utf-8")
		{
			auto translated_message = Encoding::fromWindowsString(message);

			THEN("string is correctly converted")
			{
				REQUIRE(translated_message.compare("words words words") == 0);
			}
		}
	}

	GIVEN("utf-8 string")
	{
		std::string message("words words words");

		WHEN("attempting to convert to wide string (utf-16)")
		{
			auto translated_message = Encoding::toWindowsString(message);

			THEN("string is correctly converted")
			{
				REQUIRE(translated_message.compare(L"words words words") == 0);
			}
		}
	}
}

#endif
