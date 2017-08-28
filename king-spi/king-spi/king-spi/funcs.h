#pragma once

#define TO_UTF8(x)	(kg::windows::utf::to_utf8(x))
#define UTF8_TO_CHAR(x)	(kg::windows::utf::wchar_to_char(kg::windows::utf::to_utf16(x)))
