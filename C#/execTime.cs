using System;
using System.Diagnostics;
using System.Threading;

public class ExThread
{
    public void mythread1()
    {
        for(int z = 0; z < 3; z++)
        {
            //Console.WriteLine("First Thread");
        }
    }
    public void count()
    {
        for(int i = 0; i < 10; i++)
        {
            Console.WriteLine(i);
        }
    }
}

public class ThreadContext
{
    private int a;
    private int b;

    // lock objects
    private Object aLock = new Object();
    private Object bLock = new Object();

    public ThreadContext()
    {
        a = 100;
        b = 100;
    }

    #region DeadLock Code

    public void transferFromAToB(int amount)
    {
        lock(aLock)
        {
            // Simulate computing time to get the value;
            Thread.Sleep(100);
            a = a - amount;

            lock (bLock)
            {
                // Simulate computing time to get the value;
                Thread.Sleep(100);
                b = b + amount;
            }
        }
        Console.WriteLine(amount + " was transfered from A to B");
    }

    public void transferFromBToA(int amount)
    {
        lock (aLock)
        {
            //Simulate computing time to get the value;
            Thread.Sleep(100);
            a = a + amount;

            lock (bLock)
            {
                //Simulate computing time to get the value;
                Thread.Sleep(100);
                b = b - amount;
            }
        }

        Console.WriteLine(amount + " was transfered from B to A.");
    }

    #endregion
}

namespace accesareCS
{

    class Program
    {
        static void Main(string[] args)
        {
            // Memory access
            int[] a = new int[10000000];
            Stopwatch stopWatch1 = new Stopwatch();
            stopWatch1.Start();
            for (int i = 0; i < 10000000; i ++)
            {
                a[i] = 5;
            }
            stopWatch1.Stop();

            TimeSpan ts = stopWatch1.Elapsed;

            Console.WriteLine("RunTime Access Memory " + (double) ts.Milliseconds / 1000);

            // Dynamic Memory allocation
            Stopwatch stopWatch2 = new Stopwatch();
            stopWatch2.Start();
            int[] a2 = new int[100000000];
            stopWatch2.Stop();

            TimeSpan ts2 = stopWatch2.Elapsed;

            Console.WriteLine("RunTime Dynamic Memory allocation " + (double)ts2.Milliseconds / 1000);

            // Static Memory allocation
            Stopwatch stopWatch3 = new Stopwatch();

            stopWatch3.Start();
            Span<int> a3 = stackalloc int[100000];
            stopWatch3.Stop();

            TimeSpan ts3 = stopWatch3.Elapsed;

            Console.WriteLine("RunTime Static Memory allocation " + (double)ts3.Milliseconds / 1000);


            // Create 10000 threads
            Stopwatch stopWatch4 = new Stopwatch();

            stopWatch4.Start();
            ExThread obj = new ExThread();
            Thread[] my_thrPool = new Thread[10000];
            for (int i = 0; i < 10000; i++)
            {
                my_thrPool[i] = new Thread(new ThreadStart(obj.mythread1));
            }
            stopWatch4.Stop();

            TimeSpan ts4 = stopWatch4.Elapsed;

            Console.WriteLine("RunTime Create 10000 threads: " + (double)ts4.Milliseconds / 1000);

            // Thread context switch
             Stopwatch stopWatch5 = new Stopwatch();

             stopWatch5.Start();

            ThreadContext thContext = new ThreadContext();
            Thread t1 = new Thread(() => thContext.transferFromAToB(10));
            Thread t2 = new Thread(() => thContext.transferFromBToA(10));

            t1.Start();
            t2.Start();
           // Console.ReadLine();

            stopWatch5.Stop();

            TimeSpan ts5 = stopWatch5.Elapsed;

            Console.WriteLine("RunTime Thread context switch: " + (double)ts5.Milliseconds / 1000);


            // Thread migration

            Stopwatch stopWatch6 = new Stopwatch();

            stopWatch6.Start();
            Process[] notepads = Process.GetProcessesByName("notepad");
            if(notepads.Length == 0)
            {
                Process.Start("notepad");
            }
            ProcessThreadCollection threadsCollection;
            // Process[] notepads
            // Retrieve the notepad processes.
            notepads = Process.GetProcessesByName("Notepad");
            // Get the ProcessThread collection for the first instance
            threadsCollection = notepads[0].Threads;
            // Set the properties of the first ProcessThread in the collection
            threadsCollection[0].IdealProcessor = 0;
            threadsCollection[0].ProcessorAffinity = (IntPtr)1;
            stopWatch6.Stop();

            TimeSpan ts6 = stopWatch6.Elapsed;

            Console.WriteLine("RunTime Thread migration: " + (double)ts6.Milliseconds / 1000);
        }

    }
}
