package org.jlab.jroot;

public class JRootJNI {  // Save as HelloJNI.java
    static {
      System.loadLibrary("jrootJNI");
    }

   public native void createFile(String fname);
   public native void closeFile(String fname);
   public native void mkdir(String fname,String path);
   public native void writeH1F(String fname, String path, String hname, String title,
     int nbins, double xmin, double xmax, float[] arr);
   public native void writeH2F(String fname, String path, String hname, String title,
     int nxbins, double xmin, double xmax, int nybins, double ymin, double ymax, float[] arr);
   public native void writeGraphErrors(String fname, String path, String grname, String title,
     double[] xx, double[] yy, double[] ex, double[] ey);

   public native void createNtuple(String id, String fname, String path, String name, String title, String varlist);
   public native void writeNtuple(String id, String fname, String path);
   public native void fillNtuple(String id, int nvars, float[] arr);

   public native void createJTree(String fname, String name, String varlist);
   public native void closeJTree(String fname);
   public native void fillJTree(String fname, float[] jflts, int[] jints);
}
