#include "calculator.h"
#include <stack>

void Digit(char c) {
	if (!res.empty()) {
		formula.clear();
		res.clear();
	}
	if (c == '.') {
		if (expr.find('.') == tstring::npos)
			expr += (TCHAR)c;
	} else {
		expr += (TCHAR)c;
	}
}

void Operator(char op) {
	// 如果刚计算过有结果，把结果作为新算式的起始值
	if (!res.empty()) {
		formula = res;
		res.clear();
	}
	if (!expr.empty()) {
		formula += expr;
		expr.clear();
	}
	formula += (TCHAR)op;
}

void leftParen() {
	// 如果刚计算过有结果，开始新算式时清空旧算式
	if (!res.empty()) {
		formula.clear();
		res.clear();
	}
	if (!expr.empty()) {
		formula += expr;
		expr.clear();
	}
	formula += (TCHAR)'(';
}

void rightParen() {
	if (!expr.empty()) {
		formula += expr;
		expr.clear();
	}
	formula += (TCHAR)')';
}


void Clear() {
	expr.clear();
	formula.clear();
	res.clear();
}

void Delete() {
	if (!expr.empty()) {
		expr.pop_back();
	}
	else if (!formula.empty()) {
		formula.pop_back();
	}
}
int priority(char ch) {
	if (ch == '+' || ch == '-')return 1;
	else if (ch == '*' || ch == '/')return 2;
	else return 0;
}
void Equal() {
	tstring full = formula + expr;

	if (full.empty()) {
		res = _T("Empty input!");
		return;
	}

	std::stack<char> ops;
	tstring newformula;
	for (int i = 0; i < full.length(); ++i) {
		TCHAR ch = full[i];

		// 处理一元负号
		if (ch == '-' && (i == 0 || full[i-1] == '(' || full[i-1] == '+' || full[i-1] == '-' || full[i-1] == '*' || full[i-1] == '/')) {
			if (i + 1 < full.length() && ((full[i+1] >= '0' && full[i+1] <= '9') || full[i+1] == '.')) {
				newformula += '-';
				i++;
				while (i < full.length() && ((full[i] >= '0' && full[i] <= '9') || full[i] == '.')) {
					newformula += full[i];
					i++;
				}
				newformula += ' ';
				i--;
				continue;
			}
		}
		// 数字
		if ((ch >= '0' && ch <= '9') || ch == '.') {
			while (i + 1 < full.length() && ((full[i+1] >= '0' && full[i+1] <= '9') || full[i+1] == '.')) {
				newformula += full[i];
				i++;
			}
			newformula += full[i];
			newformula += ' ';
		}
		// 左括号
		else if (ch == '(') {
			ops.push('(');
		}
		// 右括号
		else if (ch == ')') {

			while (!ops.empty() && ops.top() != '(') {
				newformula += ops.top();
				ops.pop();
				newformula += ' ';
			}
			if (ops.empty()) {
				res = _T("Right parentheses error!");
				return;
			}
			else ops.pop();
		}
		// 运算符
		else {
			while (!ops.empty() && priority(ops.top()) >= priority(ch)) {
				newformula += ops.top();
				newformula += ' ';
				ops.pop();
			}
			ops.push(ch);
		}
		}

		// 先检查左括号是否多于右括号
		while (!ops.empty()) {
			if (ops.top() == '(') {
				res = _T("Left parentheses error!");
				return;
			}
			newformula += ops.top();
			newformula += ' ';
			ops.pop();
		}

		// 再弹出剩余运算符
		while (!ops.empty()) {
			newformula += ops.top();
			newformula += ' ';
			ops.pop();
		}

		std::stack<double>nums;
		for (int i = 0; i < newformula.length(); ++i) {
			//遇到数字时继续往后读取完整的数字
			if (newformula[i] >= '0' && newformula[i] <= '9') {
				tstring numstr;
				while (i < newformula.length() && (newformula[i] >= '0' && newformula[i] <= '9' || newformula[i] == '.')) {
					numstr += newformula[i];
					i++;
				}
				i--;//回退一位
				//把字符串转换成double
				double num = _wtof(numstr.c_str());
				nums.push(num);
			}
			else if (newformula[i] == ' ')continue;
			else {
				//检查 一个符号至少有两个数
				if (nums.size() < 2) {
					res = _T("Invalid expression!");
					return;
				}

				if (newformula[i] == '+') {
					double x1 = nums.top(); nums.pop();
					double x2 = nums.top(); nums.pop();
					nums.push(x2 + x1);
				}

				else if (newformula[i] == '*') {
					double x1 = nums.top(); nums.pop();
					double x2 = nums.top(); nums.pop();
					nums.push(x2 * x1);
				}

				else if (newformula[i] == '-') {
					double x1 = nums.top(); nums.pop();
					double x2 = nums.top(); nums.pop();
					nums.push(x2 - x1);
				}

				else if (newformula[i] == '/') {
					double x1 = nums.top(); nums.pop();
					if (x1 == 0) {
						res = _T("Divisor error!");
						return;
					}
					double x2 = nums.top(); nums.pop();
					nums.push(x2 / x1);

				}
			}
		}

		double result = nums.top();
		res = std::to_wstring(result);
		formula = full + _T("=");
		expr.clear();
	}
