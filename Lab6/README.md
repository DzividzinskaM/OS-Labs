# Lab 6: Profiling


**Formulation of the problem** Profile initial code, write parameters and try to get better profiling parameters via code optimization

### **Initial code**

Whole code is attach on file, but pay attention on main function 

    Console.WriteLine("\n Inside main()\n");
    string numoffunc;
    numoffunc = Console.ReadLine();

    int i = 0;

    for (; i < 0xffffff; i++) ;

 
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

In this code, we have a loop, in which don't do anything. When we delete it, our profiling parameters become better(See it in the example section).

## **Examples**

*Picture 1. Result of executing program*

![execute-result](./img/result-execute-program.png)

*Picture 2. Result of profiling initial code*

![init-result-1](./img/result-init-1.png)

*Picture 3. Result of profiling initial code*

![init-result-2](./img/result-init-2.png)

*Picture 4. Result of profiling optimized code* 

![optimezed-result-1](./img/result-optimized-1.png)

*Picture 5. Result of profiling optimized code* 

![optimezed-result-2](./img/result-optimized-2.png)

In picture 2 and 3 we see, that empty loop execute 14.06%, and main function execute 19.17%.
After optimization main function execute 8.1% (picture 4-5).