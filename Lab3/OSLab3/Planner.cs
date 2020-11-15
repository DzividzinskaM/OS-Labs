using System;
using System.Collections.Generic;
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

        public Queue<Process> _zeroQueue;
        private Queue<Process> _firstQueue;
        private Queue<Process> _secondQueue;

        private List<Process> _waitingList;





        public Planner(List<Process> processes)
        {
            _zeroQueue = new Queue<Process>();
            _firstQueue = new Queue<Process>();
            _secondQueue = new Queue<Process>();

            _waitingList = new List<Process>();

            foreach(var process in processes)
            {
                _zeroQueue.Enqueue(process);
            }
        }

        public void AddProcess(Process process)
        {
            _waitingList.Add(process);
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
            else
                ExecuteProcesses();


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
                    () =>
                    {
                        if (_waitingList.Count != 0)
                        {
                            foreach (var item in _waitingList)
                            {
                                _zeroQueue.Enqueue(item);
                            }
                            _waitingList.Clear();
                        }
                    }
                    );
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
                    () => {
                        Thread.Sleep(currentProcess.ExecutionTime);
                        if (cancelToken.IsCancellationRequested)
                        {
                            _secondQueue.Enqueue(currentProcess);
                        }
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
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"Quantum time {quantumTime}");
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

    }
}
