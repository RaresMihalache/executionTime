package abc;

public class accesare {
	static int numbers1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // stored on the heap
	public static void main(String args[]) {
		int numbers2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // stored on the stack
		
		// Memory Access
		long startTime = System.nanoTime();
		int[] a = new int[10000000];
		for(int i=0; i < a.length; i++) {
			a[i] = 5;
		}
		long endTime = System.nanoTime();
		double timeTaken = endTime - startTime; // nanoseconds
		timeTaken /= 1000000000; // nano -> sec
		System.out.println("Time elapsed (memory access): " + timeTaken);
		
		// Dynamic memory allocation
		startTime = System.nanoTime();
		for(int i=0; i < a.length; i++) {
			int x = numbers1[i % 10];
		}
		endTime = System.nanoTime();
		timeTaken = endTime - startTime; // nanoseconds
		timeTaken /= 1000000000; // nano -> sec
		System.out.println("Time elapsed (dynamic allocation): " + timeTaken);
		
		// Static memory allocation
		startTime = System.nanoTime();
		for(int i=0; i < a.length; i++) {
			int x = numbers2[i % 10];
		}
		endTime = System.nanoTime();
		timeTaken = endTime - startTime; // nanoseconds
		timeTaken /= 1000000000; // nano -> sec
		System.out.println("Time elapsed (static allocation): " + timeTaken);
		
		// Create threads
		startTime = System.nanoTime();
		Thread t[] = new Thread[10000];
		for(int i=0; i< t.length; i++) {
			t[i] = new Thread();
		}
		endTime = System.nanoTime();
		timeTaken = endTime - startTime; // nanoseconds
		timeTaken /= 1000000000; // nano -> sec
		System.out.println("Time elapsed (thread creation): " + timeTaken);
		
		// Thread context switch
		startTime = System.nanoTime();
		MyThread mt1 = new MyThread("1");
		MyThread mt2 = new MyThread("2");
		for(;;) {
			if(mt1.getFlag() == true && mt2.getFlag() == true) {
				endTime = System.nanoTime();
				break;
			}
			try {
				Thread.sleep(1);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		timeTaken = endTime - startTime; // nanoseconds
		timeTaken /= 1000000000; // nano -> sec
		System.out.println("Time elapsed (thread context switch): " + timeTaken);
		
		
		// Test to check if it is in Release or Debug mode.
		boolean isDebug = java.lang.management.ManagementFactory.getRuntimeMXBean().
			    getInputArguments().toString().indexOf("-agentlib:jdwp") > 0;
		System.out.println(isDebug);
	}
}

// Thread class
class MyThread implements Runnable{

	private boolean flag;
	private String name;
	Thread t;
	
	public MyThread(String threadName){
		name = threadName;
		t = new Thread(this, name);
		//System.out.println("New Thread " + name);
		flag = false;
		t.start();
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		int i = 0;
		while(flag == false) {
			//System.out.println(name + " : " + i);
			i++;
			if(i == 10) {
				stop();
			}
			try {
				Thread.sleep(100);
			} catch(Exception ex) {
				System.out.println("Caught " + ex);
			}
		}
		//System.out.println(name + " stopped");
	}
	
	public void stop()
    {
        flag = true;
    }
	
	public boolean getFlag() {
		return flag;
	}
}
	