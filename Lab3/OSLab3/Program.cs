﻿using Microsoft.FSharp.Data.UnitSystems.SI.UnitNames;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace OSLab3
{

    class Program
    {
        static void Main(string[] args)
        {

            List<Process> processes = new List<Process>();

            processes.Add(new Process(1, 3));
            processes.Add(new Process(2, 7));
            processes.Add(new Process(3, 4));
            processes.Add(new Process(4, 10));
            processes.Add(new Process(5, 18));
            processes.Add(new Process(6, 15));
            processes.Add(new Process(7, 5));

            Planner planner = new Planner(processes);



            List<Process> additionProceses = new List<Process>();

            additionProceses.Add(new Process(8, 4));
            additionProceses.Add(new Process(9, 5));
            additionProceses.Add(new Process(10, 10));
            additionProceses.Add(new Process(11, 20));
            additionProceses.Add(new Process(12, 3));



            Parallel.Invoke(
                () => {
                    planner.ExecuteProcesses();
                },
                ()=> { 
                    foreach(var process in additionProceses)
                    {
                        Thread.Sleep(50);
                        planner.AddProcess(process);
                        
                    }
                });


        }
    }
}
