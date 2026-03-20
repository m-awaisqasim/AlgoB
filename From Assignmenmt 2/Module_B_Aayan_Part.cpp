#include <iostream>
using namespace std;

// Jsut build SMA & EMA trading Indicator
const int MAX = 1000;

double price_data[MAX];
int signals[MAX];
int days_count;

double SMA(int day, int period)
{
    double total = 0;

    for (int i = day - period + 1; i <= day; i++)
    {
        total = total + price_data[i];
    }

    return total / period;
}

int main()
{
    cout << "Enter number of days: ";
    cin >> days_count;

    if (days_count < 50)
    {
        cout << "Need at least 50 days of data." << endl;
        return 0;
    }


    for (int i = 0; i < days_count; i++)
    {
        cout << "Enter price for day " << i + 1 << ": ";
        cin >> price_data[i];
        signals[i] = 0;
    }

    int short_period = 20;
    int long_period = 50;

    for (int day = long_period; day < days_count; day++)
    {
        double short_avg = SMA(day, short_period);
        double long_avg = SMA(day, long_period);

        if (day == long_period)
        {
            signals[day] = 0;
        }
        else
        {
            double prev_short = SMA(day - 1, short_period);
            double prev_long = SMA(day - 1, long_period);

            if (prev_short <= prev_long && short_avg > long_avg)
            {
                signals[day] = 1;
            }
            else if (prev_short >= prev_long && short_avg < long_avg)
            {
                signals[day] = -1;
            }
            else
            {
                signals[day] = 0;
            }
        }
    }

    cout << "\nDay   Price   Signal\n";

    for (int i = 50; i < days_count; i++)
    {
        cout << i + 1 << "   " << price_data[i] << "   " << signals[i] << endl;
    }

    return 0;
}