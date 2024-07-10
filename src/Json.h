#pragma once
#include "Alastor.h"

#ifdef _DEBUG
#define TRACK(DESC) //do {std::cout << "--> " << DESC << std::endl;} while (0);
#else
#define TRACK(DESC)
#endif

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>
#include <map>

namespace Alastor
{
	class ALASTOR_API Json
	{
	public:
		/**
		 * 类型枚举 http://www.json.org/
		 */
		enum Kind
		{
			kNumber,	///< 数字,   2013, 12.45, 3.3e+12, -12E-5
			kString,	///< 字符串, "hello world"
			kBool,		///< 布尔值, true or false
			kNull,		///< 空值
			kObject,	///< 对象. {"id": 1931, "name": "Ggicci"}
			kArray		///< 数组. [ 1, 2, null, {"love": true} ]
		};

		/**
		 *  解析Json字符串到Json对象.
		 *
		 * 当解析失败时会产生 std::exception::what() 消息
		 * 告诉你失败的原因
		 * @param  json 字符串
		 * @return             一个 Json 类接口
		 *
		 * \code{.cpp}
		 * try
		 * {
		 *		Json json = Json::Parse("{ \"year\": 2013, month: 8 }"); // cause exception
		 * }
		 * catch (exception& e)
		 * {
		 * 	cout << e.what() << endl;
		 * 	// output:
		 * 	// SyntaxError: Unexpected token m at pos 16
		 * }
		 * \endcode
		 */
		static Json Parse(const char* json_string);
		Json();

		/// <summary>
		/// 从int构建Json对象
		/// </summary>
		explicit Json(int num);

		/// <summary>
		/// 从double构造Json对象
		/// </summary>
		explicit Json(double num);

		/// <summary>
		/// 从string构造Json对象,该string将不会被解析
		/// </summary>
		explicit Json(const std::string& str);

		/// <summary>
		/// 从char*构造Json对象,该char*将不会被解析
		/// </summary>
		/// <param name="str"></param>
		explicit Json(const char* str);


		/// <summary>
		/// 从bool构造Json对象
		/// </summary>
		explicit Json(bool boo);


		/// <summary>
		/// 深拷贝构造函数
		/// </summary>
		Json(const Json& rhs);

		Json& operator = (const Json& rhs);

		virtual ~Json();

		/**
		 * \brief Get the enum value of this Json object, which indicates
		 * the type of the data it holds.
		 * @return an enum value
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3, 4]");
		 * json.IsArray(); // true
		 * json.DataKind() == Json::kArray; // true
		 * \endcode
		 */
		Kind DataKind() const { return kind_; }

		/**
		 * \brief Test whether this Json object represents a number.
		 * @return true means it's a number
		 */
		bool IsNumber() const { return kind_ == kNumber; }

		/**
		 * \brief Test whether this Json object represents a string.
		 * @return true means it's a string
		 */
		bool IsString() const { return kind_ == kString; }

		/**
		 * \brief Test whether this Json object represents a bool.
		 * @return true means it's a bool (its value may be true or false)
		 */
		bool IsBool() const { return kind_ == kBool; }

		/**
		 * \brief Test whether this Json object represents null.
		 * \note Distinguish with IsEmpty()
		 * \see Json()
		 * @return true means it's null
		 */
		bool IsNull() const { return !data_; }

		/**
		 * \brief Test whether this Json object represents an array.
		 * @return true means it's an array
		 */
		bool IsArray() const { return kind_ == kArray; }

		/**
		 * \brief Test whether this Json object represents an object.
		 * @return true means it's an object
		 */
		bool IsObject() const { return kind_ == kObject; }

		/**
		 * \brief Test whether an object or array holds values.
		 * @return true if no values in an object or array
		 *
		 * \code{.cpp}
		 * Json empty_object = Json::Parse("{}");
		 * Json empty_array = Json::Parse("[]");
		 * empty_object.IsEmpty(); // true
		 * empty_array.IsEmpty(); // true
		 * empty_object.IsNull; // false
		 * \endcode
		 */
		bool IsEmpty() const;

		/**
		 * \brief Test if an object Json object has a KVP(key-value pair) named \em key.
		 *
		 * An object Json object means this Json object represents an object "{...}".
		 * \note if this Json object does not represent an object (IsObject() == false),
		 * 		 it always returns false.
		 *
		 * @param  key the key(name) of a KVP
		 * @return \b true if IsObject() == true and it contains that KVP, otherwise \b false
		 *
		 * \code {.cpp}
		 * Json object = Json::Parse("{\"name\": \"Ggicci\", \"age\": 21}");
		 * object.Contains("name"); // true
		 * object.Contains("sex"); // false
		 * object = 3; // will delete the data of object and
		 *             // allocate new memory for object to
		 *             // hold value (3). and now, object is
		 *             // a number
		 * object.Contains("hello"); // false
		 * \endcode
		 */
		bool Contains(const char* key) const;

		/**
		 * Indicate the size of the array.
		 * @return size of the array, or 1 if it is not an array.
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3, 4]");
		 * json.Size(); // 4
		 * json.Push(5).Push(6).Size(); // 6
		 * json = 0; // 1
		 * (json = "Ggicci").Size(); // 1
		 * \endcode
		 */
		size_t Size() const;

		/**
		 * \brief Get all the keys of the KVPs the object Json object holds.
		 *
		 * If this Json object represents an object (parsed from "{...}"), it
		 * will have some KVPs (or maybe none), then use this function you can
		 * retrieve all the keys (names) in a std::vector.
		 * \note An empty vector will be returned if it's not an object Json object.
		 * @return a std::vector contains all the keys
		 *
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"id\":1234, \"name\": \"Ggicci\", \"birthday\": [1991, 11, 10]}");
		 * vector<string> keys = json.Keys();
		 * for (vector<string>::const_iterator cit = keys.begin(); cit != keys.end(); ++cit)
		 * {
		 * 	cout << *cit << ": " << json[cit->c_str()].ToString() << endl;
		 * }
		 * \endcode
		 * \b Output:
		 * \code{.txt}
		 * birthday: [ 1991, 11, 10 ]
		 * id: 1234
		 * name: "Ggicci"
		 * \endcode
		 */
		std::vector<std::string> Keys() const;

		/**
		 * \brief Push a Json object to the current Json object (finally an array).
		 *
		 * If current Json object is a number, string, bool, null or object, it will be
		 * converted to an array first and then push back \em rhs to this array.
		 * If current Json object is an array, just simply push back \em rhs.
		 * Otherwise, an exception will be thrown.
		 * \note An exception may be thrown to indicate that it's a bad operation.
		 * @param  rhs the Json object to push
		 * @return     The Json object finally got
		 *
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json(10);
		 * cout << json.ToString() << endl; // 10
		 * json.Push("fuck").Push("{Here will not be parsed}").Push(true);
		 * cout << json.ToString() << endl;
		 * // [ 10, "fuck", "{Here will not be parsed}", true ]
		 * \endcode
		 */
		Json& Push(const Json& rhs);

		/**
		 * \brief AddProperty a Json object to the current Json object (finally an object).
		 *
		 * If this Json object doesn't represent an object (IsObject() == false), an exception
		 * will be thrown. Otherwise, a KVP(key-value pair) will be insert into this object
		 * Json object, which consists of \em key and \em val. The function returns the
		 * referece to this object, so you can call AddProperty function in a cascade way.
		 * \note An exception may be thrown to indicate that it's a bad operation.
		 * @param  key the key(name) of the KVP
		 * @param  val the value of the KVP
		 * @return      the Json object finally got
		 *
		 * <b>Example 1:</b>
		 * \code{.cpp}
		 * Json json = Json::Parse("{}");
		 * json.AddProperty("name", Json("Ggicci"));
		 * json.AddProperty("characteristics", Json::Parse("[\"optimitic\", \"sympathetic\"]"));
		 * cout << json.ToString() << endl;
		 * // output:
		 * // { "characteristics": [ "optimitic", "sympathetic" ], "name": "Ggicci" }
		 * \endcode
		 *
		 */
		Json& AddProperty(const std::string& key, const Json& value);

		/**
		 * \brief Remove a specefied KVP(key-value pair) from an object Json object by key(name).
		 *
		 * Find the KVP named \em key and remove it from this current Json object. Simultaneously,
		 * the memory will be deallocated. And nothing will happen if not found.
		 * \note An exception will be thrown when you try to operate on non-object Json object.
		 * @param  key the key(name) of the KVP
		 * @return     the Json object after removing the specified KVP
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"color\": \"green\", \"weight\": 12.4}");
		 * cout << "json = " << json.ToString() << endl;
		 * // output:
		 * // json = { "color": "green", "weight": 12.4 }
		 * cout << "json(after) = " << json.Remove("color").Remove("not found").ToString() << endl;
		 * // output:
		 * // json(after) = { "weight": 12.4 }
		 * \endcode
		 */
		Json& Remove(const std::string& key);

		/**
		 * \brief Remove an item from an array by the specified index
		 *
		 * If the index out of range [0, Size()-1], the specified item of this array
		 * will be removed and deleted in memory.
		 * \note This function will not return the Json object.
		 * @param index the index of the array
		 *
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, true, \"Hello World\"]");
		 * json.Remove(1);
		 * cout << json.ToString() << endl;
		 * // output:
		 * // [ 1, true, "Hello World" ]
		 * \endcode
		 */
		void Remove(size_t index);

		/**
		 * \brief Extract the data from number Json object and return it as \b int.
		 * \note Exception when Json object is not a number.
		 */
		int AsInt() const;

		/**
		 * \brief Extract the data from number Json object and return it as \b double.
		 * \note Exception when Json object is not a number.
		 */
		double AsDouble() const;

		/**
		 * \brief Extract the data from bool Json object and return it as \b bool.
		 * \note Exception when Json object is not a bool.
		 */
		bool AsBool() const;

		/**
		 * \brief Extract the data from string Json object and return it as \b std::string.
		 * \note Exception when Json object is not a string.
		 */
		std::string AsString() const;

		/**
		 * \brief Extract the item data from an array. Return the reference.
		 * \note Exception when Json object is not an array.
		 */
		const Json& operator[] (int index) const;

		/**
		 * \brief Extract the item data from an array. Return the reference.
		 * \note Exception when Json object is not an array.
		 * \code{.cpp}
		 * Json json = Json::Parse("[1, 2, 3 ,4]");
		 * json[1] = "hello"; // [1, "hello", 3, 4]
		 * \endcode
		 */
		Json& operator[] (int index);

		/**
		 * \brief Extract the item data from an object by specified a key(name).
		 * Return the reference.
		 * \note Exception when Json object is not an object.
		 */
		const Json& operator[] (const char* key) const;

		/**
		 * \brief Extract the item data from an object by specified a key(name).
		 * Return the reference.
		 * \note Exception when Json object is not an object.
		 * \code{.cpp}
		 * Json json = Json::Parse("{\"author\": \"Ggicci\", \"sex\": 0}");
		 * json["sex"] = 1; // { "author": "Ggicci", "sex": 1 }
		 * \endcode
		 */
		Json& operator[] (const char* key);

		/**
		 * \brief Assignment from \b int, finally become a number.
		 */
		Json& operator = (int num);

		/**
		 * \brief Assignment from \b double, finally become a number.
		 */
		Json& operator = (double num);

		/**
		 * \brief Assignment from \b std::string, finally become a string.
		 * \note \en str will not be parsed.
		 */
		Json& operator = (const std::string& str);

		/**
		 * \brief Assignment from \b const char*, finally become a string.
		 * \note \em str will not be parsed.
		 */
		Json& operator = (const char* str);

		/**
		 * \brief Assignment from \b bool, finally become a bool.
		 */
		Json& operator = (bool boo);

		/**
		 * \brief Get the json structural string of this Json object.
		 *
		 * For example, if this Json object represents an object, and it has
		 * a KVP named "id" and its value is 194024, then you will get a json
		 * string like this: <b>{ "id": 194024 }</b>.
		 * @return the json structural string
		 */
		std::string ToString() const;

	private:
#define CAST_JSON_OBJ(DATA) (static_cast<ObjectData*>(DATA))
#define CAST_JSON_ARR(DATA) (static_cast<ArrayData*>(DATA))

		typedef std::vector<Json*> ArrayData;
		typedef std::map<std::string, Json*> ObjectData;
		typedef std::pair<std::string, Json*> Pair;

		/**
		 * 执行真正解析工作的嵌套结构.
		 *
		 * Firstly, I want to use static functions for doing
		 * the same job. However, it is annoying to pass arguments
		 * between different functions in order to keep the library
		 * \b thread-safe.
		 */
		struct Parser
		{
			const char* source;		///< the json structural string need to be parsed
			int			pos;		///< current position(index) of the character in \em source
			unsigned char character;///< current character scanned at
			std::string	token;		///< appear as a word, finally it will be parsed to correspoding
			///< data structure
			///< ~~~
			///< +---------+---+----------+---+---------+---+
			///< | source  | [ | 112.4e+3 | , | "hello" | ] |
			///< +---------+---+----------+---+---------+---+
			///< | tokens  |   |  token   |   |  token  |   |
			///< +---------+---+----------+---+---------+---+
			///< ~~~

/**
 * Constructor
 */
			Parser(const char* json_string);

			/**
			 * \brief Parse a \b Value.
			 *
			 * Value is a general term. A value maybe a string, number, object, array, bool, null.
			 * ![value](value.gif "value")
			 * \note All the consume functions will throw an exception when syntax error occurs
			 * 		 in the \en source string.
			 * @param  section whether global, it will check the end of the input when global
			 * @return         the Json object parsed from \em source (maybe from substring)
			 */
			Json* ConsumeValue(bool section = true);

			/**
			 * \brief Parse a \b number.
			 *
			 * ![number](number.gif "number")
			 * @return the number Json object parsed
			 */
			Json* ConsumeNumber();

			/**
			 * \brief Parse a \b string.
			 *
			 * ![string](string.gif "string")
			 * @return the string Json object parsed
			 */
			Json* ConsumeString();

			/**
			 * \brief Parse a \b bool(true or false).
			 * @return the bool Json object parsed
			 */
			Json* ConsumeBool();

			/**
			 * \brief Parse a \b null.
			 * @return the null Json object parsed
			 */
			Json* ConsumeNull();

			/**
			 * \brief Parse an \b object.
			 *
			 * ![object](object.gif "object")
			 * @return the object Json object parsed
			 */
			Json* ConsumeObject();

			/**
			 * \brief Parse an \b array.
			 *
			 * ![array](array.gif "array")
			 * @return the array Json object parsed
			 */
			Json* ConsumeArray();

			/**
			 * \brief Parse a \b pair of an \b object.
			 *
			 * Object of a Json consists of zero or more pairs. Ref :
			 * <a href="www.json.org">json.org</a>
			 * ![object](object.gif "object")
			 * @return the pair parsed
			 */
			Json::Pair ConsumePair();

			/**
			 * \brief Consume a specified string in the \em source.
			 * \note If the string not found in \em source, it will throw an exception
			 * 		 to indicate a parse error.
			 * @param str the spcified string to be cosumed in \em source
			 */
			void ConsumeSpecific(const char* str);

			/**
			 * \brief Detect the whole \em source string represents what, a string or a number or something else.
			 * @return the Kind enum value indicates the data type
			 */
			Json::Kind KindDetect();

			/**
			 * \brief Concat the current \em character to \em token.
			 */
			void Concat() { token += character; }

			/**
			 * \brief Scan backward a step(one character distance).
			 */
			void Retract() { pos--; character = ' '; }

			/**
			 * \brief Whether it is the end-of-line.
			 * @return true if EOL.
			 */
			bool EOL() const { return '\0' == character; }

			/**
			 * \brief Scan forward a step(one character distance).
			 * @return the character after current \em position, i.e. the character after steping forward
			 */
			char NextCharacter()
			{
				if (pos >= 0 && '\0' == *(source + pos)) { UnexpectedToken(); }
				character = *(source + (++pos));
				return character;
			}

			/**
			 * \brief Skip all the white spcaes(' ', \t, \r, \n) from \em position.
			 */
			void SkipWhitespaces();

			/**
			 * \brief Throw UnexpectedTokenException to indicate an unexpected token in \em source.
			 */
			void UnexpectedToken();
		};

		/**
		 * \brief Exception indicates syntax error of \em source.
		 */
		struct UnexpectedTokenException : std::exception
		{
		public:
			UnexpectedTokenException(char ch, int pos);
			virtual ~UnexpectedTokenException() throw();
			const char* what() const throw();
		private:
			char 		ch_; 	///< which character cause syntax error
			int  		pos_;	///< where this character locates
			std::string msg_;	///< error message
		};

		/**
		 * \brief Exception indicates there is a bad conversion.
		 *
		 * \code{.cpp}
		 * Json a(12.3);
		 * a.AsString(); 	// bad conversion
		 * a.AsBool();   	// bad conversion
		 * a.AsInt();		// fine
		 * a.AsDouble();	// fine
		 * Json b("hello");
		 * b.AsString(); 	// fine
		 * b.AsInt();		// bad conversion
		 * b.AsBool(); 		// bad conversion
		 * \endcode
		 */
		struct BadConversionException : std::exception
		{
		public:
			const char* what() const throw() { return "a bad conversion"; }
		};

		/**
		 * 从数组(vector)中删除Json对象
		 */
		static void DestroyArrayData(ArrayData& arr);

		/**
		 * 从映射(map)中删除Json对象
		 */
		static void DestroyObjectData(ObjectData& obj);

		/**
		 * 从一个右值对象指针构造
		 */
		explicit Json(Json* rhs);

		/**
		 * 从map构造
		 */
		explicit Json(ObjectData* obj);

		/**
		 * 从vector构造
		 */
		explicit Json(ArrayData* arr);

		/**
		 * 深拷贝
		 */
		void DoDeepCopy(const Json& rhs);

		/**
		 * 释放Json对象所使用的内存
		 */
		void Release();

		/**
		 * 将数据转换为指定类型 返回引用
		 */
		template <typename ToType>
		ToType& Data()
		{
			return const_cast<ToType&>(static_cast<const Json&>(*this).Data<ToType>());
		}

		/**
		 * 将数据转换为指定类型 返回引用
		 */
		template <typename ToType>
		const ToType& Data() const
		{
			bool okay;
			switch (kind_)
			{
			case kNumber: okay = (typeid(double) == typeid(ToType)); break;
			case kString: okay = (typeid(std::string) == typeid(ToType)); break;
			case kBool: okay = (typeid(bool) == typeid(ToType)); break;
			case kArray: okay = (typeid(ArrayData) == typeid(ToType)); break;
			case kObject: okay = (typeid(ObjectData) == typeid(ToType)); break;
			default: okay = false;
			}
			if (!okay)
			{
				throw BadConversionException();
			}
			return *static_cast<ToType*>(data_);
		}

		Kind kind_;		// 数据类型
		void* data_;	// Json对象所保存的真实数据
	};

}