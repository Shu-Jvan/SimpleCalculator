#ifndef CALCULATOR_H
#define CALCULATOR_H



#include <iostream>
#include <string>
#include <stack>
#include <limits>
#include <type_traits>
#include <cmath>
using namespace std;



/*======================================== 类及其函数定义 ========================================*/
template <class T> // 使用模板，以便计算不同类型的数据
class Calculator {
// 成员变量
private:
    string expr;

// 运算符处理
private:
    int precedence(char op);                // 运算符优先级
    bool isOperator(char c);                // 判断是否是运算符
    bool isShiftOperator(char a, char b);   // 判断是否是左移/右移运算符

// 运算符操作
private:
    T add(T a, T b);            // 加
    T subtract(T a, T b);       // 减
    T multiply(T a, T b);       // 乘
    T divide(T a, T b);         // 除
    T modulo(T a, T b);         // 取模
    T bitwise_AND(T a, T b);    // 与
    T bitwise_OR(T a, T b);     // 或
    T bitwise_NOT(T a);         // 非
    T bitwise_XOR(T a, T b);    // 异或
    T left_shift(T a, T bits);	// 左移
	T right_shift(T a, T bits);	// 右移

// 辅助成员函数
private:
	bool validateExpression();			// 初步判断用户输入的多项式是否合法
    T turn_string_into_digit(int &i);	// 将字符串转化为数字

public:
    Calculator();                     // 构造函数
    ~Calculator();                    // 析构函数
    T calculate();                    // 计算
};


/*======================================== 类方法实现 ========================================*/
/*==================== 预处理部分 ====================*/
/*----- 构造函数 -----*/
template <class T>
Calculator<T>::Calculator()
{
	cout << "=========================================== Welcome to use XLv's calcultor! ============================================\n\n\n";
	cout << "Please input the expression you what to calculate: ";

	cin >> this->expr;

	bool isLegal = Calculator<T>::validateExpression();
	if (isLegal == false)
	{
		cout << "\nYou input an illegal expression!" << endl;
		exit(1);
	}
}

/*----- 析构函数 -----*/
template <class T>
Calculator<T>::~Calculator()
{
	cout << "\n\n============================================ Thanks for using, best wishes! ============================================" << endl;
}

/*----- 初步判断用户输入的多项式是否合法 -----*/
template <class T>
bool Calculator<T>::validateExpression()
{
	// 主要就是看一下几种情况：
	// 1、有非法字符
	// 2、输入字符数目少于两个（因为‘~’为单目运算符）
	// 3、只有运算符或只有数据
	// 4、运算符在最后面或者在最前面且不为~、-~、+~、且-为减号而不是负号、+为加号而不是正号
	// 5、有连续的，不带~的运算符（~+、~-、+~、-~、~~都合法）
	const int N = (int)this->expr.size();

	// 情况1
	for (int i = 0; i < N; i++)
	{
		bool a = isdigit(this->expr[i]);
		bool b = this->expr[i] == '.';
		bool c = Calculator<T>::isOperator(this->expr[i]);
		bool d = (i < N-1 && this->expr[i] == '<' && this->expr[i+1] == '<') || (i > 0 && this->expr[i] == '<' && this->expr[i-1] == '<');
		bool e = (i < N-1 && this->expr[i] == '>' && this->expr[i+1] == '>') || (i > 0 && this->expr[i] == '>' && this->expr[i-1] == '>');
		bool f = expr[i] == '(' || expr[i] == ')';
		bool g = expr[i] == ' ';

		if (!a && !b && !c && !d && !e && !f) return false;
	}

	// 情况2
	if (N < 2 || (N == 2 && this->expr[0] != '~')) return false;

	// 情况3
	bool hasOpreator = false, hasOperand = false;
	for (int i = 0; i < N; i++)
	{
		if (isdigit(this->expr[i])) hasOperand = true;
		if (Calculator<T>::isOperator(this->expr[i]) || (i < N-1 && Calculator<T>::isShiftOperator(this->expr[i], this->expr[i+1]))) hasOpreator = true;
	}
	if (!hasOpreator || !hasOperand) return false;

	// 情况4
	if (!isdigit(this->expr[N-1]) && this->expr[N-1] != ')' && this->expr[N-1] != ' ') return false;
	if (this->isOperator(this->expr[0]) && this->expr[0] != '~' && this->expr[0] != '+' && this->expr[0] != '-' && this->expr[0] != ' ' && this->expr[0] != '(') return false;

	// 情况5
	for (int i = 1; i < N; i++)
	{
		bool a = Calculator<T>::isOperator(this->expr[i]);
		bool b = this->expr[i] != '~';

		if (a && b) // 遇见一个运算符且该运算符不是‘~’
		{
			if (i > 0 && this->expr[i-1] != '~' && Calculator<T>::isOperator(this->expr[i-1])) return false;
		}
	}

	return true;
}


/*==================== 运算符处理部分 ====================*/
/*----- 运算符优先级 -----*/
template <class T>
int Calculator<T>::precedence(char op)
{
    if (op == '|') return 1;
	else if (op == '^') return 2;
	else if (op == '&') return 3;
	else if (op == '<' || op == '>') return 4;
	else if (op == '+' || op == '-') return 5;
	else if (op == '*' || op == '/' || op == '%') return 6;
	else if (op == '~' || op == '=') return 7;
	return 0;
}

/*----- 判断是否是运算符（不包括左移、右移） -----*/
template <class T>
bool Calculator<T>::isOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&' || c == '|' || c == '~' || c == '^';
}

/*----- 判断是否是左移/右移运算符 -----*/
template <class T>
bool Calculator<T>::isShiftOperator(char a, char b)
{
	if ((a == '>' && b == '>') || (a == '<' && b == '<')) return true;
	return false;
}


/*==================== 运算符函数 ====================*/
/*----- 加 -----*/
template <class T>
T Calculator<T>::add(T a, T b)
{
	return a + b;
}

/*----- 减 -----*/
template <class T>
T Calculator<T>::subtract(T a, T b)
{
	return a - b;
}

/*----- 乘 -----*/
template <class T>
T Calculator<T>::multiply(T a, T b)
{
	return a * b;
}

/*----- 除 -----*/
template <class T>
T Calculator<T>::divide(T a, T b)
{
	if (b == 0) throw runtime_error("\nDivision by zero!"); // 除0，报错
	return a / b;
}

/*----- 取模 -----*/
template <class T>
T Calculator<T>::modulo(T a, T b)
{
    if (b == 0) throw runtime_error("\nModulo by zero!"); // 对0取模，报错
	if constexpr (is_integral<T>::value)
	{
		return a % b;
	}
	else
	{
		// For floating-point template, require integer-valued operands for modulo
		if (std::floor(a) != a || std::floor(b) != b) throw runtime_error("\nModulo on non-integer value is not allowed!");
		long long ai = static_cast<long long>(a);
		long long bi = static_cast<long long>(b);
		return static_cast<T>(ai % bi);
	}
}

/*----- 与 -----*/
template <class T>
T Calculator<T>::bitwise_AND(T a, T b)
{
    if constexpr (is_integral<T>::value)
	{
		return a & b;
	}
	else
	{
		if (std::floor(a) != a || std::floor(b) != b) throw runtime_error("\nBitwise operation on non-integer value is not allowed!");
		long long ai = static_cast<long long>(a);
		long long bi = static_cast<long long>(b);
		return static_cast<T>(ai & bi);
	}
}

/*----- 或 -----*/
template <class T>
T Calculator<T>::bitwise_OR(T a, T b)
{
    if constexpr (is_integral<T>::value)
	{
		return a | b;
	}
	else
	{
		if (std::floor(a) != a || std::floor(b) != b) throw runtime_error("\nBitwise operation on non-integer value is not allowed!");
		long long ai = static_cast<long long>(a);
		long long bi = static_cast<long long>(b);
		return static_cast<T>(ai | bi);
	}
}

/*----- 非 -----*/
template <class T>
T Calculator<T>::bitwise_NOT(T a)
{
    if constexpr (is_integral<T>::value)
	{
		return ~a;
	}
	else
	{
		if (std::floor(a) != a) throw runtime_error("\nBitwise operation on non-integer value is not allowed!");
		long long ai = static_cast<long long>(a);
		return static_cast<T>(~ai);
	}
}

/*----- 异或 -----*/
template <class T>
T Calculator<T>::bitwise_XOR(T a, T b)
{
    if constexpr (is_integral<T>::value)
	{
		return a ^ b;
	}
	else
	{
		if (floor(a) != a || floor(b) != b) throw runtime_error("\nBitwise operation on non-integer value is not allowed!");
		long long ai = static_cast<long long>(a);
		long long bi = static_cast<long long>(b);
		return static_cast<T>(ai ^ bi);
	}
}

/*----- 左移 -----*/
template <class T>
T Calculator<T>::left_shift(T a, T bits)
{
	// 检测a和bits是不是整数，不是的话就报错，因为左移不能用在浮点数
	if (is_floating_point<T>::value)
	{
		if (floor(a) != a || floor(bits) != bits) throw runtime_error("\nBitwise/shift operation on non-integer value is not allowed!");
	}

	// 将操作数转换为有符号类型的long long，即
	long long ai = static_cast<long long>(a);
	long long bi = static_cast<long long>(bits);
	// bits为负数，报错
	if (bi < 0) throw runtime_error("\nNegative shift count is not allowed!");

	if (bi >= static_cast<long long>(numeric_limits<long long>::digits)) throw overflow_error("\nLeft shift would result in overflow!");

	unsigned long long uai = static_cast<unsigned long long>(ai < 0 ? -ai : ai);
	unsigned long long resu = uai << bi;

	long long res = (ai < 0) ? -static_cast<long long>(resu) : static_cast<long long>(resu);

	if (is_integral<T>::value)
	{
		if (res > static_cast<long long>(numeric_limits<T>::max()) || res < static_cast<long long>(numeric_limits<T>::min())) throw overflow_error("\nLeft shift would result in overflow!");
	}

	return static_cast<T>(res);
}

/*----- 右移 -----*/
template <class T>
T Calculator<T>::right_shift(T a, T bits)
{
	if (is_floating_point<T>::value)
	{
		if (floor(a) != a || floor(bits) != bits) throw runtime_error("\nBitwise/shift operation on non-integer value is not allowed!");
	}

	long long ai = static_cast<long long>(a);
	long long bi = static_cast<long long>(bits);

	if (bi < 0) throw runtime_error("\nNegative shift count is not allowed!");

	if (bi >= static_cast<long long>(numeric_limits<long long>::digits)) return static_cast<T>(0);

	long long res = ai >> bi;

	if (is_integral<T>::value)
	{
		if (res > static_cast<long long>(numeric_limits<T>::max()) || res < static_cast<long long>(numeric_limits<T>::min())) throw overflow_error("\nRight shift would result in overflow!");
	}

	return static_cast<T>(res);
}


/*==================== 计算函数 ====================*/
/*----- 转换字符串为数字（只有正数，负数主函数内部处理） -----*/
template <class T>
T Calculator<T>::turn_string_into_digit(int &i)
{
	const int N = (int)this->expr.size();
	T num = 0;
    while (i < N && (isdigit(this->expr[i]) || this->expr[i] == '.'))
	{
		if (this->expr[i] == '.')
		{
			T decimal = 0.1;
			i++;
			while (i < N && isdigit(this->expr[i]))
			{
				num += (T)(this->expr[i] - '0') * decimal;
				decimal /= 10.0;
				i++;
			}
			continue;
		}
		else
		{
			num = num * 10 + (T)(this->expr[i] - '0');
			i++;
		}
	}
	i--;
	return num;
}

/*----- 主函数 -----*/
template <class T>
T Calculator<T>::calculate()
{
	stack<T> operandS;		// 存储操作数的栈
	stack<char> operatorS;	// 存储操作符的栈
	
	const int N = (int)this->expr.size();
	int i = 0;

	// 主计算过程
	while (i < N)
	{
		// 空格
		if (this->expr[i] == ' ')
		{
			i++;
		}
		// 数字
		else if (isdigit(this->expr[i]))
		{
			T num = this->turn_string_into_digit(i);
			operandS.push(num);
		}
		// 左括号
		else if (this->expr[i] == '(')
		{
			operatorS.push('(');
		}
		// 右括号
		else if (this->expr[i] == ')')
		{
			while (!operatorS.empty() && operatorS.top() != '(')
			{
				// 双目运算符
				if (operatorS.top() != '~' && operatorS.top() != '=')
				{
					// 从operandS中弹出两个数字来进行计算
					T x = operandS.top(); operandS.pop();
					T y = operandS.top(); operandS.pop();
					// 弹出一个运算符
					char op = operatorS.top(); operatorS.pop();
					// 进行运算，并且把运算结果压入栈中
					switch (op)
					{
						case '+': operandS.push(add(y, x));			break;
						case '-': operandS.push(subtract(y, x));	break;
						case '*': operandS.push(multiply(y, x));	break;
						case '/': operandS.push(divide(y, x));		break;
						case '%': operandS.push(modulo(y, x));		break;
						case '&': operandS.push(bitwise_AND(y, x));	break;
						case '|': operandS.push(bitwise_OR(y, x));	break;
						case '^': operandS.push(bitwise_XOR(y, x));	break;
						case '<': operandS.push(left_shift(y, x));	break;
						case '>': operandS.push(right_shift(y, x));	break;
					}
				}
				// 单目运算符~
                else if (operatorS.top() == '~')
				{
					T x = operandS.top(); operandS.pop(); operatorS.pop();
					operandS.push(bitwise_NOT(x));
				}
				// 负号‘=’
				else
				{
					T x = operandS.top(); operandS.pop(); operatorS.pop();
					operandS.push(-x);
				}
			}
			// 弹出左括号
			if (operatorS.top() == '(') operatorS.pop();
			else throw runtime_error("\nYou input an illegal expression!");
		}
		// 运算符（~、+、-要特殊处理，因为有~~、~+、~-、+~、-~）
		else if (Calculator<T>::isOperator(this->expr[i]) || (i < N-1 && Calculator<T>::isShiftOperator(this->expr[i], this->expr[i+1])))
		{
			char op = this->expr[i]; // 给下面是否是>>/<<而要不要i++用

			bool isNot = false, isNegative = false, isPositive = false;
			// 非
			if (this->expr[i] == '~')
			{
				operatorS.push('~'); // 因为~是在数字前面的，所以不能弹出一个数字来运算
				isNot = true;
			}
			// 负号（不作为减号）
			else if (this->expr[i] == '-')
			{
				// -~情况
				if (this->expr[i+1] == '~')
				{
					isNegative = true;
					operatorS.push('=');
				}
				// 多项式第一项为负数、左括号后一个数为负数、~-情况
				else if (i == 0 || this->expr[i-1] == '(' || this->expr[i-1] == '~')
				{
					isNegative = true;
					// 负号后面就是数字
					if (isdigit(this->expr[i+1]))
					{
						i++;
						T num = this->turn_string_into_digit(i);
						operandS.push(-num);
					}
					// 后面不是数字，那只能是‘(’或者‘~’，把负号改成‘=’
					else
					{
						operatorS.push('=');
					}
				}
			}
			// 正号（不作为加号）
			else if (this->expr[i] == '+')
			{
				if (i == 0 || this->expr[i-1] == '(' || this->expr[i-1] == '~')
				{
					isPositive = true;
				}
			}

			// 其他/是减号而不是负号/是加号而不是正号
			if (!isNot && !isNegative && !isPositive)
			{
				while (!operatorS.empty() && precedence(operatorS.top()) >= precedence(this->expr[i]))
				{
					// 双目运算符
					if (operatorS.top() != '~' && operatorS.top() != '=')
					{
						// 从operandS中弹出两个数字来进行计算
						T x = operandS.top(); operandS.pop();
						T y = operandS.top(); operandS.pop();
						// 弹出一个运算符
						char op = operatorS.top(); operatorS.pop();
						// 进行运算，并且把运算结果压入栈中
						switch (op)
						{
							case '+': operandS.push(add(y, x));			break;
							case '-': operandS.push(subtract(y, x));	break;
							case '*': operandS.push(multiply(y, x));	break;
							case '/': operandS.push(divide(y, x));		break;
							case '%': operandS.push(modulo(y, x));		break;
							case '&': operandS.push(bitwise_AND(y, x));	break;
							case '|': operandS.push(bitwise_OR(y, x));	break;
							case '^': operandS.push(bitwise_XOR(y, x));	break;
							case '<': operandS.push(left_shift(y, x));	break;
							case '>': operandS.push(right_shift(y, x));	break;
						}
					}
					// 单目运算符~
                    else if (operatorS.top() == '~')
					{
						T x = operandS.top(); operandS.pop(); operatorS.pop();
						operandS.push(bitwise_NOT(x));
					}
					// 负号‘=’
					else
					{
						T x = operandS.top(); operandS.pop(); operatorS.pop();
						operandS.push(-x);
					}
				}
			}
			// 跳过左移/右移的下一个</>
			if (op == '<' || op == '>') i++;
			// 把运算符压入运算符栈中
			if (!isNot && !isNegative && !isPositive) operatorS.push(this->expr[i]);
		}
		// 其他，报错
		else
		{
			throw runtime_error("\nInvaild character in expression!");
		}

		i++;
	}

	// 副计算过程：处理剩余操作数和操作符
	while (!operatorS.empty())
	{
		// 双目运算符
		if (operatorS.top() != '~' && operatorS.top() != '=')
		{
			// 从operandS中弹出两个数字来进行计算
			T x = operandS.top(); operandS.pop();
			T y = operandS.top(); operandS.pop();
			// 弹出一个运算符
			char op = operatorS.top(); operatorS.pop();
			// 进行运算，并且把运算结果压入栈中
			switch (op)
			{
				case '+': operandS.push(add(y, x));			break;
				case '-': operandS.push(subtract(y, x));	break;
				case '*': operandS.push(multiply(y, x));	break;
				case '/': operandS.push(divide(y, x));		break;
				case '%': operandS.push(modulo(y, x));		break;
				case '&': operandS.push(bitwise_AND(y, x));	break;
				case '|': operandS.push(bitwise_OR(y, x));	break;
				case '^': operandS.push(bitwise_XOR(y, x));	break;
				case '<': operandS.push(left_shift(y, x));	break;
				case '>': operandS.push(right_shift(y, x));	break;
			}
		}
        // 单目运算符~
		else if (operatorS.top() == '~')
		{
			T x = operandS.top(); operandS.pop(); operatorS.pop();
			operandS.push(bitwise_NOT(x));
		}
		// 负号‘=’
		else
		{
			T x = operandS.top(); operandS.pop(); operatorS.pop();
			operandS.push(-x);
		}
	}

	return operandS.top(); // 终于结束了🎉🎉
}



#endif
