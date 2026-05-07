#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589793
int degreeMode = 0;
double lastResult = 0;

// ================= HELPER =================

double toRadians(double x) 
{
    return (degreeMode ? x * PI / 180.0 : x);
}

double toDegrees(double x) 
{
    return (degreeMode ? x * 180.0 / PI : x);
}

long double factorial(int n) 
{
    if (n < 0) return -1.0; // Error
    if (n == 0 || n == 1) return 1.0;
    long double result = 1.0L;
    for (int i = 2; i <= n; i++) 
    {
        result *= i;
    }
    return result;
}

// ================= BASIC =================

void basicMenu() 
{
    int choice, n;
    double result, x;
    while (1) 
    {
        printf("\n--- BASIC CALCULATIONS ---\n");
        printf("1.Add\n2.Subtract\n3.Multiply\n4.Divide\n0.Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input!\n");
            return;
        }
        if (choice == 0) return;
        printf("Enter number of values: ");
        if (scanf("%d", &n) != 1 || n < 2) 
        {
            printf("Please enter at least two values.\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        printf("Enter %d numbers: ", n);
        if (scanf("%lf", &result) != 1) 
        {
            printf("Invalid number input!\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int valid = 1;
        for (int i = 1; i < n; i++) 
        {
            if (scanf("%lf", &x) != 1) 
            {
                printf("Invalid number input!\n");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                valid = 0;
                break;
            }
            switch (choice) 
            {
                case 1:
                    result += x;
                    break;
                case 2:
                    result -= x;
                    break;
                case 3:
                    result *= x;
                    break;
                case 4:
                    if (x == 0) 
                    {
                        printf("Error: Division by zero!\n");
                        valid = 0;
                        break;
                    }
                    result /= x;
                    break;
                default:
                    printf("Invalid choice!\n");
                    valid = 0;
                    break;
            }
            if (!valid) break;
        }
        if (valid) 
        {
            lastResult = result;
            printf("Result = %.4lf\n", lastResult);
        }
    }
}

// ================= TRIGONOMETRY =================

void trigonometry() 
{
    int choice;
    double x, result;
    while (1) 
    {
        printf("\n--- TRIGONOMETRY ---\n");
        printf("Mode: %s\n", degreeMode ? "DEGREE" : "RADIAN");
        printf("1.sin 2.cos 3.tan \n4.asin 5.acos 6.atan \n0.Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        printf("Enter value: ");
        scanf("%lf", &x);
        switch (choice)
        {
            case 1: result = sin(toRadians(x)); break;
            case 2: result = cos(toRadians(x)); break;
            case 3:
                if (fabs(cos(toRadians(x))) < 1e-6) 
                {
                    printf("tan undefined at this angle!\n");
                    continue;
                }
                result = tan(toRadians(x));
                break;
            case 4:
                if (x < -1 || x > 1) 
                {
                    printf("Domain error!\n");
                    continue;
                }
                result = toDegrees(asin(x));
                break;
            case 5:
                if (x < -1 || x > 1) {
                    printf("Domain error!\n");
                    continue;
                }
                result = toDegrees(acos(x));
                break;
            case 6:
                result = toDegrees(atan(x));
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        lastResult = result;
        printf("Result = %.4lf\n", result);
    }
}

// ================= HYPERBOLIC =================

void hyperbolic() 
{
    int choice;
    double x, result;
    while (1) 
    {
        printf("\n--- HYPERBOLIC FUNCTIONS ---\n");
        printf("1.sinh 2.cosh 3.tanh\n4.asinh 5.acosh 6.atanh\n0.Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        printf("Enter value: ");
        scanf("%lf", &x);
        switch (choice) 
        {
            case 1: result = sinh(x); break;
            case 2: result = cosh(x); break;
            case 3: result = tanh(x); break;
            case 4: result = asinh(x); break;
            case 5:
                if (x < 1) 
                {
                    printf("Domain error!\n");
                    continue;
                }
                result = acosh(x);
                break;
            case 6:
                if (x <= -1 || x >= 1) 
                {
                    printf("Domain error!\n");
                    continue;
                }
                result = atanh(x);
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        lastResult = result;
        printf("Result = %.4lf\n", result);
    }
}

// ================= EXPONENTIATION =================

void exponentiation() 
{
    int choice;
    double a, b, result;
    while (1) 
    {
        printf("\n--- EXPONENTIATION ---\n");
        printf("1.Power  2.Sqrt  3.Nth Root  4.Logarithm  5.Natural Logarithm\n0.Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        switch (choice) 
        {
            case 1:
                printf("Enter base and exponent: ");
                scanf("%lf %lf", &a, &b);
                result = pow(a, b);
                break;
            case 2:
                printf("Enter number: ");
                scanf("%lf", &a);
                if (a < 0) 
                {
                    printf("Error: Negative input!\n");
                    continue;
                }
                result = sqrt(a);
                break;
            case 3:
                printf("Enter number and n: ");
                scanf("%lf %lf", &a, &b);
                if (b == 0) {
                    printf("Error!\n");
                    continue;
                }
                result = pow(a, 1.0 / b);
                break;
            case 4:
                printf("Enter number: ");
                scanf("%lf", &a);
                if (a <= 0) 
                {
                    printf("Error: Non-positive input!\n");
                    continue;
                }
                result = log10(a); // Base-10 logarithm
                break;
            case 5:
                printf("Enter number: ");
                scanf("%lf", &a);
                if (a <= 0) 
                {
                    printf("Error: Non-positive input!\n");
                    continue;
                }
                result = log(a); 
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        lastResult = result;
        printf("Result = %.4lf\n", result);
    }
}


// ================= COMBINATORIAL =================

void combinatorial() 
{
    int choice, n, r;
    long double result;
    while (1) 
    {
        printf("\n--- combinatorial ---\n");
        printf("1.Factorial\n2.Permutation (nPr)\n3.Combination (nCr)\n0.Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input!\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        if (choice == 0) return;
        switch (choice) 
        {
            case 1:
                printf("Enter n: ");
                if (scanf("%d", &n) != 1) 
                {
                    printf("Invalid input!\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    continue;
                }
                result = factorial(n);
                if (result < 0) 
                {
                    printf("Error: Invalid input!\n");
                    continue;
                }
                printf("Factorial of %d = %f\n", n, (double)result);
                lastResult = result;
                break;
            case 2:
                printf("Enter n and r: ");
                if (scanf("%d %d", &n, &r) != 2) 
                {
                    printf("Invalid input!\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    continue;
                }
                if (n < 0 || r < 0 || r > n) 
                {
                    printf("Error: Invalid values!\n");
                    continue;
                }
                long double fact_n = factorial(n);
                long double fact_nr = factorial(n - r);
                if (fact_n < 0 || fact_nr < 0) 
                {
                    printf("Error: Invalid input!\n");
                    continue;
                }
                result = fact_n / fact_nr;
                printf("Permutation P(%d,%d) = %f\n", n, r, (double)result);
                lastResult = result;
                break;
            case 3:
                printf("Enter n and r: ");
                if (scanf("%d %d", &n, &r) != 2) 
                {
                    printf("Invalid input!\n");
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    continue;
                }
                if (n < 0 || r < 0 || r > n) 
                {
                    printf("Error: Invalid values!\n");
                    continue;
                }
                long double fact_n_c = factorial(n);
                long double fact_r = factorial(r);
                long double fact_nr_c = factorial(n - r);
                if (fact_n_c < 0 || fact_r < 0 || fact_nr_c < 0) 
                {
                    printf("Error: Invalid input!\n");
                    continue;
                }
                result = fact_n_c / (fact_r * fact_nr_c);
                printf("Combination C(%d,%d) = %f\n", n, r, (double)result);
                lastResult = result;
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ================= ADVANCED =================

void advancedMenu() 
{
    int choice;
    while (1) 
    {
        printf("\n--- ADVANCED CALCULATIONS ---\n");
        printf("1.Trigonometry\n2.Hyperbolic\n3.Exponentiation\n4.Combinatorial\n5.Toggle Degree/Radian\n0.Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        if (choice == 0) return;
        switch (choice) 
        {
            case 1: trigonometry(); break;
            case 2: hyperbolic(); break;
            case 3: exponentiation(); break;
            case 4: combinatorial(); break;
            case 5:
                degreeMode = !degreeMode;
                printf("Mode switched to %s\n", degreeMode ? "DEGREE" : "RADIAN");
                break;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ================= MAIN =================

int main() 
{
    int choice;
    while (1) 
    {
        printf("\n===== MAIN MENU =====\n");
        printf("1.Basic Calculations\n2.Advanced Calculations\n");
        printf("3.Show Last Result\n4.GUI Calculator\n0.Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            printf("Invalid input!\n");
            break;
        }
        switch (choice) 
        {
            case 1: basicMenu(); break;
            case 2: advancedMenu(); break;
            case 3:
                printf("Last Result = %.4lf\n", lastResult);
                break;
            case 4:
                printf("Launching GUI Calculator...\n");
                system("GUI_Calculator.exe");
                break;
            case 0:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
