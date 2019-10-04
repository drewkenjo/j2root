package org.jlab.jroot;
import org.jlab.groot.data.H1F;
import org.jlab.groot.data.H2F;
import org.jlab.groot.data.GraphErrors;

public class ROOTFile {
  JRootJNI jroot = new JRootJNI();
  private String fname;
  private String path = "";

  public ROOTFile(String fname) {
    this.fname = fname;
    jroot.createFile(fname);
  }

  public void close() {
    jroot.closeFile(fname);
  }

  public void mkdir(String path) {
    path = path.replaceFirst("^/*","").replaceAll("/*\\$","");
    jroot.mkdir(fname, path);
    this.path = path;
  }

  public void cd(String path) {
    this.path = path.replaceFirst("^/*","").replaceAll("/*\\$","");
  }

  public TNtuple makeNtuple(String name, String title, String varlist) {
    TNtuple tpl = new TNtuple(fname, path, name, title, varlist);
    return tpl;
  }

  public void writeDataSet(H1F h1) {
    String fullpath = path + "/" + h1.getName();
    int ind = fullpath.lastIndexOf("/");
    String relpath = fullpath.substring(0,ind);
    relpath = relpath.replaceFirst("^/*","").replaceAll("/*\\$","");
    String name = fullpath.substring(ind+1);
    String title = h1.getTitle()+";"+h1.getTitleX()+";"+h1.getTitleY();
    jroot.writeH1F(fname, relpath, name, title,
      h1.getXaxis().getNBins(), h1.getXaxis().min(), h1.getXaxis().max(),
      h1.getData());
  }

  public void writeDataSet(H2F h2) {
    String fullpath = path + "/" + h2.getName();
    int ind = fullpath.lastIndexOf("/");
    String relpath = fullpath.substring(0,ind);
    relpath = relpath.replaceFirst("^/*","").replaceAll("/*\\$","");
    String name = fullpath.substring(ind+1);
    String title = h2.getTitle()+";"+h2.getTitleX()+";"+h2.getTitleY();
    int xsize = h2.getDataSize(0), ysize = h2.getDataSize(1), ii = 0;
    float[] data = new float[xsize*ysize];
    for(int ix=0;ix<xsize;ix++)
      for(int iy=0;iy<ysize;iy++)
        data[ii++] = (float) h2.getData(ix,iy);

    jroot.writeH2F(fname, relpath, name, title,
      h2.getXAxis().getNBins(), h2.getXAxis().min(), h2.getXAxis().max(),
      h2.getYAxis().getNBins(), h2.getYAxis().min(), h2.getYAxis().max(),
      data);
  }

  public void writeDataSet(GraphErrors gr) {
    String fullpath = path + "/" + gr.getName();
    int ind = fullpath.lastIndexOf("/");
    String relpath = fullpath.substring(0,ind);
    relpath = relpath.replaceFirst("^/*","").replaceAll("/*\\$","");
    String name = fullpath.substring(ind+1);
    String title = gr.getTitle()+";"+gr.getTitleX()+";"+gr.getTitleY();
    int size = gr.getDataSize(0);
    double[] xx = new double[size];
    double[] yy = new double[size];
    double[] ex = new double[size];
    double[] ey = new double[size];
    for(int ib=0;ib<size;ib++) {
      xx[ib] = gr.getDataX(ib);
      yy[ib] = gr.getDataY(ib);
      ex[ib] = gr.getDataEX(ib);
      ey[ib] = gr.getDataEY(ib);
    }
    jroot.writeGraphErrors(fname, relpath, name, title,
      xx, yy, ex, ey);
  }
}
