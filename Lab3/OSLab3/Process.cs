using System;
using System.Collections.Generic;
using System.Text;

namespace OSLab3
{
    public class Process
    {
        public int ID { get; set; }

        public int ExecutionTime { get; set; }

        public Process(int id, int time)
        {
            ID = id;
            ExecutionTime = time;
        }
    }
}
