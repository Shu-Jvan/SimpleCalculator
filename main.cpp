#include "Calculator.h"

using T = double;

int main()
{
	try
	{
		Calculator<T> calu;
		T ans = calu.calculate();

		cout << "\nThe result of caculation is: ";
		if (floor(ans) == ans)
		{
			long long ival = static_cast<long long>(ans);
			cout << ival << endl;
		}
		else
		{
			cout << ans << endl;
		}
	}
	catch (const std::runtime_error& e)
	{
		cerr << "Error: " << e.what() << endl;
	}
	return 0;
}