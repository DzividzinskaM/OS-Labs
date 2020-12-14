using System;
using System.Threading;

namespace OSLab6
{
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine("\n Inside main()\n");
            string numoffunc;
            numoffunc = Console.ReadLine();

            int i = 0;

          // for (; i < 0xffffff; i++) ;

          
                switch (numoffunc)
                {
                    case "first":
                        FirstFunction();
                        break;
                    case "second":
                        SecondFunction();
                        break;

                    case "third":
                        ThirdFunction();
                        break;
                }
           

        }

        void new_func1() { }

        static void FirstFunction()
        {
            for (int i = 0; i < 10; i++)
            {
                //process 
                Thread.Sleep(1000);
            }

            return;
        }

        static void SecondFunction()
        {
            for (int i = 0; i < 10; i++)
            {
                //process 
                Thread.Sleep(1000);
            }
            return;
        }

        static void ThirdFunction()
        {
            for (int i = 0; i < 10; i++)
            {
                //process 
                Thread.Sleep(1000);
            }
            return;
        }

    }
}
