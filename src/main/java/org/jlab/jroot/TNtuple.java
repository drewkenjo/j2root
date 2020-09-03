package org.jlab.jroot;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class TNtuple {
  private JRootJNI jroot = new JRootJNI();
  private String fname;
  private String path = "";
  private String id;
  private int nvars;
  private ConcurrentLinkedQueue<double[]> data = new ConcurrentLinkedQueue<>();
  private ReentrantLock lock = new ReentrantLock();


  private TNtuple() {
  }

  TNtuple(String fname, String path, String name, String title, String varlist) {
    this.fname = fname;
    this.path = path;
    this.nvars = varlist.split(":").length;
    this.id = fname+":"+path+":"+name;
    jroot.createNtuple(id, fname, path, name, title, varlist);
  }

  private void save(boolean isFinished) {
    int saveSize = data.size();
    while(saveSize>0) {
      int bucketSize = Math.min(saveSize, 2000) * nvars;
      saveSize -= 2000;
      float[] arr = new float[bucketSize];

      for(int iar=0; iar<bucketSize; iar+=nvars) {
        double[] vars = data.poll();
        for(int ivar=0; ivar<nvars; ivar++) {
          arr[iar+ivar] = (float) vars[ivar];
        }
      }

      jroot.fillNtuple(id, nvars, arr);
    }

    if(isFinished) {
      jroot.writeNtuple(id, fname, path);
    }
  }

  public void write() {// throws InterruptedException {
    lock.lock();
    try {
      save(true);
    } finally {
      lock.unlock();
    }
  }

  public void fill(double... x) {
    data.add(x);
    if(data.size() > 10000 && lock.tryLock()) {
      try {
        save(false);
      } finally {
        lock.unlock();
      }
    }
  }
}
