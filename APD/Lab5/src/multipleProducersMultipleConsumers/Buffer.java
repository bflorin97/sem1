package multipleProducersMultipleConsumers;

import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
    int a;
    Semaphore pr = new Semaphore(1);
    Semaphore co = new Semaphore(0);

	void put(int value) {
		try {
            pr.acquire();
            a = value;
            co.release();
        } catch (Exception e) {
            e.printStackTrace();
        }
	}

	int get() {
	    int b = -1;

	    try {
            co.acquire();
            b = a;
            pr.release();
        } catch (Exception e) {
	        e.printStackTrace();
        }

		return b;
	}
}
