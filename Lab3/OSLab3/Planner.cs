using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace OSLab3
{
    public class Planner
    {
        private int _zeroQuantumTime = 8;
        private int _firstQuantumTime = 16;
        private int _intencity;
        private int _number_process;
        private double _downtime = 0;
        private double _generalTime = 0;


        public Queue<Process> _zeroQueue;
        private Queue<Process> _firstQueue;
        private Queue<Process> _secondQueue;

        private List<Process> _waitingList;



        public Planner(List<Process> processes, int intencity, bool report = false)
        {
            _intencity = intencity;
            _number_process = processes.Count;
            _zeroQueue = new Queue<Process>();
            _firstQueue = new Queue<Process>();
            _secondQueue = new Queue<Process>();

            _waitingList = processes;


        }

   
        public void Execute()
        {
            
            CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
            CancellationToken cancelToken = cancellationTokenSource.Token;

            Parallel.Invoke(
                () => {

                    while(_zeroQueue.Count > 0)
                    {
                        ExecuteProcesses();
                    }
                    cancellationTokenSource.Cancel();
                },
                () => {
                  
                    _downtime += _intencity;
                    int processIndex = 0;
                    while(_waitingList.Count > 0)
                    {

                        _zeroQueue.Enqueue(_waitingList[processIndex]);
                        _waitingList.RemoveAt(processIndex);

                        if (cancelToken.IsCancellationRequested)
                            return;

                        Thread.Sleep(_intencity);

                    }
                }
                );

            if (_waitingList.Count == 0)
                ShortReport();
            else
                Execute();
        }

     


        public void ExecuteProcesses()
        {
            
            QuantumTimeReport(0);
            if (_zeroQueue.Count != 0)
                ExecuteZeroQueueProcesses();

            if (_firstQueue.Count != 0)
                ExecuteFirstQueueProcesses();

            if (_secondQueue.Count != 0)
                ExecuteSecondQueueProcesses();

            if (_zeroQueue.Count == 0 || _firstQueue.Count == 0 || _secondQueue.Count == 0)
                return;

        }

     

        private void ExecuteZeroQueueProcesses()
        {
            
            while (_zeroQueue.Count != 0)
            {
                CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
                CancellationToken cancelToken = cancellationTokenSource.Token;

                Process currentProcess = _zeroQueue.Dequeue();

                Parallel.Invoke(
                    () =>
                    {

                        Thread.Sleep(_zeroQuantumTime);
                        cancellationTokenSource.Cancel();
                       
                    },
                    () =>   {
                        Thread.Sleep(currentProcess.ExecutionTime);
                        if(cancelToken.IsCancellationRequested)
                        {
                            _firstQueue.Enqueue(currentProcess);
                        }                 
                    },
                    () => {
                        if(_zeroQuantumTime - currentProcess.ExecutionTime > 0)
                            _downtime += (_zeroQuantumTime - currentProcess.ExecutionTime);
                    });
                QuantumTimeReport(_zeroQuantumTime);

            }


        }

       
        private void ExecuteFirstQueueProcesses()
        {
            while (_firstQueue.Count != 0)
            {
                CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
                CancellationToken cancelToken = cancellationTokenSource.Token;

                Process currentProcess = _firstQueue.Dequeue();

                Parallel.Invoke(
                    () =>
                    {
                        Thread.Sleep(_firstQuantumTime);
                        cancellationTokenSource.Cancel();
                    },
                    () =>
                    {
                        Thread.Sleep(currentProcess.ExecutionTime);
                        if (cancelToken.IsCancellationRequested)
                        {
                            _secondQueue.Enqueue(currentProcess);
                        }
                    },
                    () => {
                        if (_firstQuantumTime - currentProcess.ExecutionTime > 0)
                            _downtime += (_firstQuantumTime - currentProcess.ExecutionTime);
                    });
                    QuantumTimeReport(_firstQuantumTime);


                if (_zeroQueue.Count != 0)
                    ExecuteZeroQueueProcesses();
                
            }
        }

        private void ExecuteSecondQueueProcesses()
        {
            while (_secondQueue.Count != 0)
            {

                Process currentProcess = _secondQueue.Dequeue();

                Thread.Sleep(currentProcess.ExecutionTime);
                

                QuantumTimeReport(currentProcess.ExecutionTime);


                if (_zeroQueue.Count != 0)
                    ExecuteZeroQueueProcesses();
                if (_firstQueue.Count != 0)
                    ExecuteFirstQueueProcesses();
               

            }
        }

        public void QuantumTimeReport(int quantumTime)
        {
            _generalTime += quantumTime;
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"Quantum time {quantumTime}");
            Console.WriteLine($"General time {_generalTime}");

            Console.WriteLine();

            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("Zero queue");
            Console.ResetColor();

            writeQueue(_zeroQueue);

            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("First queue");
            Console.ResetColor();


            writeQueue(_firstQueue);

            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine("Second queue");
            Console.ResetColor();

            writeQueue(_secondQueue);

            Console.WriteLine();


        }

        private void writeQueue(Queue<Process> queue)
        {
           
            foreach (var process in queue)
            {
                Console.WriteLine($"Process - {process.ID}, time - {process.ExecutionTime}");
            }
           
        }

        private void ShortReport()
        {
            _downtime -= _intencity;
            _generalTime += _downtime;
            double percent = Math.Round(((_downtime / _generalTime)*100), 3);
            double averageTime = Math.Round((_generalTime / _number_process), 3);

            Console.WriteLine($"Intencity input flow {_intencity}");
            Console.WriteLine($"Downtime  {_downtime}");
            Console.WriteLine($"Time  {_generalTime}");
            Console.WriteLine($"Downtime percent {percent}");
            Console.WriteLine($"Average time {averageTime}");
        }

    }
}
