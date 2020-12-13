# Memory optimization

**Formulation of the problem** There are some initial code and we need optimize it.



### **Initial code**


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



There is an array with arrays. In a loop, we increment the first element of the first array twice. I don't think this is an optimized option because there are two element and memory calls. To solve this problem, you need to add two units to the element at once




### **Optimized code**

    int[][] AD = new int[2][];
    AD[0] = new int[2];
    AD[1] = new int[2];

    int res = 0;

    for (int j = 500000000; j > 0; j--)
    {
        AD[0][0] +=2;
    }

    AD[1][1] = AD[0][0];

    Console.WriteLine(AD[0][0]);

    

## Examples

*Picture 1. Result of executing initial code*

![result-1](./img/result-1.png)

*Picture 2. Result of executing optimized code*

![result-2](./img/result-2.png)

So we get the same results but different execution times. Code optimezed in 1.5 times by reducing access to elements and memory from two to one time.