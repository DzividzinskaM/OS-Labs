using System;
using System.Diagnostics;

namespace OSLab5
{
    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch stopwatch = new Stopwatch();

            stopwatch.Start();


            int[][] AD = new int[2][];
            AD[0] = new int[2];
            AD[1] = new int[2];

            int res = 0;

            for (int j = 500000000; j > 0; j--)
            {
                AD[0][0]++;
                AD[0][0]++;
            }

            AD[1][1] = AD[0][0];

            Console.WriteLine(AD[0][0]);

            stopwatch.Stop();

            Console.WriteLine($"Number of milliseconds {stopwatch.ElapsedMilliseconds}");





         /*   Stopwatch stopwatch = new Stopwatch();

            stopwatch.Start();

            int[][] AD = new int[2][];
            AD[0] = new int[2];
            AD[1] = new int[2];

            int res = 0;

            for (int j = 500000000; j > 0; j--)
            {
                AD[0][0] += 2;
            }


            AD[1][1] = AD[0][0];


            Console.WriteLine(AD[0][0]);

            stopwatch.Stop();

            Console.WriteLine($"Number of milliseconds {stopwatch.ElapsedMilliseconds}");*/

        }




       
    }
}
