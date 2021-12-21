package org.jlab.jroot;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class JTree {
  private JRootJNI jroot = new JRootJNI();
  private String fname;
  private String path = "";
  private String id;
  private int nvars;
  private int nflts=0;
  private int nints=0;
  private int[] types = new int[1000];
  private ConcurrentLinkedQueue<Number[]> data = new ConcurrentLinkedQueue<>();
  private ReentrantLock lock = new ReentrantLock();


  private JTree() {
  }

  JTree(String fname, String path, String name, String title, String varlist) {
    this.fname = fname;
    this.path = path;
    this.id = fname+":"+path+":"+name;
    jroot.createJTree(fname, name, varlist);

    String[] vnames = varlist.split(":");
    this.nvars = vnames.length;
    for(int ivar=0;ivar<nvars;ivar++) {
      if(vnames[ivar].contains("/I")) {
        types[ivar] = 1;
        nints++;
      } else {
        types[ivar] = 0;
        nflts++;
      }
    }
  }

  private void save(boolean isFinished) {
    int saveSize = data.size();
    while(saveSize>0) {
      int nevs = Math.min(saveSize, 2000);
      saveSize -= nevs;

      int fltSize = nevs * nflts;
      int intSize = nevs * nints;
      float[] flts = new float[fltSize];
      int[] ints = new int[intSize];

      int iflt=0, iint=0;
      for(int iev=0; iev<nevs; iev++) {
        Number[] vars = data.poll();
        for(int ivar=0; ivar<nvars; ivar++) {
          if(types[ivar]==1) {
            ints[iint] = vars[ivar].intValue();
            iint++;
          } else {
            flts[iflt] = vars[ivar].floatValue();
            iflt++;
          }
        }
      }

      jroot.fillJTree(fname, flts, ints);

    }

    if(isFinished) {
      jroot.closeJTree(fname);
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

  public void fill(Number... x) {
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
