package oneProducerOneConsumer;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a;
	AtomicBoolean bufferEmpty = new AtomicBoolean(true);
    Object p = new Object();
    Object c = new Object();

	void put(int value) {
        try {
            if (!bufferEmpty.get()) {
                synchronized (p) {
                    p.wait();
                }
            }

            a = value;
            bufferEmpty.set(false);

			synchronized (c) {
                c.notify();
            }
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	int get() {
	    int b = -1;
		try {
		    if (bufferEmpty.get()) {
                synchronized (c) {
                    c.wait();
                }
            }

            b = a;
            bufferEmpty.set(true);

            synchronized (p){
                p.notify();
            }
		} catch (Exception e) {
			e.printStackTrace();
		}
		return b;
	}
}
