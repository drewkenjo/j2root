package org.jlab.jroot;
import java.util.List;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class TNtuple {
  private JRootJNI jroot = new JRootJNI();
  private String fname;
  private String path = "";
  private String id;
  private int nvars;
  private boolean isFinished = false;
  private ConcurrentLinkedQueue<double[]> data = new ConcurrentLinkedQueue<>();
  private ScheduledExecutorService executor = Executors.newScheduledThreadPool(1);

  private TNtuple() {
  }

  TNtuple(String fname, String path, String name, String title, String varlist) {
    this.fname = fname;
    this.path = path;
    this.nvars = varlist.split(":").length;
    this.id = fname+":"+path+":"+name;
    jroot.createNtuple(id, fname, path, name, title, varlist);
    executor.scheduleWithFixedDelay(this::save, 200, 200, TimeUnit.MILLISECONDS);
  }

  private void save() {
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

    if(isFinished && data.isEmpty()) {
      jroot.writeNtuple(id, fname, path);
      executor.shutdown();
    }
  }

  public void write() throws InterruptedException {
    isFinished = true;
    executor.awaitTermination(10, TimeUnit.MINUTES);
  }

  public void fill(double... x) {
    data.add(x);
  }
}
