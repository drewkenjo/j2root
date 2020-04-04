#include <iostream>
#include <unordered_map>
#include <mutex>
#include <jni.h>
#include <stdio.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMap.h>
#include <TGraphErrors.h>
#include "org_jlab_jroot_JRootJNI.h"

//TMap* objects = new TMap();
class concurrentMap
{
  std::mutex m_;
  std::unordered_map<std::string, TObject*> objs;

public:
    TObject* get(std::string k) {
        std::unique_lock<decltype(m_)> lock(m_);
        return objs[k];
    }

    void set(std::string k, TObject* v) {
        std::unique_lock<decltype(m_)> lock(m_);
        objs[k] = v;
    }
};

concurrentMap objects;

static __attribute__((constructor)) void init() {
  gSystem->ResetSignals();
  ROOT::EnableThreadSafety();
  gROOT->SetBatch(true);
}

JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_createFile (JNIEnv *env, jobject thisObj, jstring jfname) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  TFile* ff = new TFile(fname, "RECREATE");
  objects.set(fname, ff);
  env->ReleaseStringUTFChars(jfname, fname);
}

JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_closeFile (JNIEnv *env, jobject thisObj, jstring jfname) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  ((TFile*) objects.get(fname))->Close();
  env->ReleaseStringUTFChars(jfname, fname);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_mkdir( JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  ((TFile*) objects.get(fname))->mkdir(path);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_writeH1F (JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath,
	jstring jname, jstring jtitle, jint nbins, jdouble xmin, jdouble xmax, jfloatArray jdata) {

  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  const char *name = env->GetStringUTFChars(jname, NULL);
  const char *title = env->GetStringUTFChars(jtitle, NULL);

  float *cdata = env->GetFloatArrayElements(jdata, NULL);

  TFile* ff = (TFile*) objects.get(fname);
  if(!ff->Get(path))
    ff->mkdir(path);
  ff->cd(path);

  TH1F* h1 = new TH1F(name, title, nbins, xmin, xmax);
  jsize length = env->GetArrayLength(jdata);
  double nentries = 0;

  for(int ib=0;ib<length;ib++) {
    h1->SetBinContent(ib+1, cdata[ib]);
    nentries += cdata[ib];
  }

  h1->SetEntries(nentries);
  h1->Write("",TObject::kOverwrite);

  env->ReleaseFloatArrayElements(jdata, cdata, 0);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
  env->ReleaseStringUTFChars(jname, name);
  env->ReleaseStringUTFChars(jtitle, title);
}



JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_writeH2F (JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath,
	jstring jname, jstring jtitle, jint nxbins, jdouble xmin, jdouble xmax, jint nybins, jdouble ymin, jdouble ymax, jfloatArray jdata) {

  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  const char *name = env->GetStringUTFChars(jname, NULL);
  const char *title = env->GetStringUTFChars(jtitle, NULL);

  float *cdata = env->GetFloatArrayElements(jdata, NULL);

  TFile* ff = (TFile*) objects.get(fname);
  if(!ff->Get(path))
    ff->mkdir(path);
  ff->cd(path);

  TH2F* h2 = new TH2F(name, title, nxbins, xmin, xmax, nybins, ymin, ymax);
  double nentries = 0;
  int ii = 0;

  for(int ix=0;ix<nxbins;ix++)
  for(int iy=0;iy<nybins;iy++) {
    h2->SetBinContent(ix+1, iy+1, cdata[ii]);
    nentries += cdata[ii++];
  }

  h2->SetEntries(nentries);
  h2->Write("",TObject::kOverwrite);

  env->ReleaseFloatArrayElements(jdata, cdata, 0);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
  env->ReleaseStringUTFChars(jname, name);
  env->ReleaseStringUTFChars(jtitle, title);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_writeGraphErrors (JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath,
	jstring jname, jstring jtitle, jdoubleArray jxx, jdoubleArray jyy, jdoubleArray jex, jdoubleArray jey) {

  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  const char *name = env->GetStringUTFChars(jname, NULL);
  const char *title = env->GetStringUTFChars(jtitle, NULL);

  double *xx = env->GetDoubleArrayElements(jxx, NULL);
  double *yy = env->GetDoubleArrayElements(jyy, NULL);
  double *ex = env->GetDoubleArrayElements(jex, NULL);
  double *ey = env->GetDoubleArrayElements(jey, NULL);

  TFile* ff = (TFile*) objects.get(fname);
  if(!ff->Get(path))
    ff->mkdir(path);
  ff->cd(path);

  jsize length = env->GetArrayLength(jxx);
  TGraphErrors* gr = new TGraphErrors(length, xx, yy, ex, ey);
  gr->SetName(name);
  gr->SetTitle(title);
  gr->Write();

  env->ReleaseDoubleArrayElements(jxx, xx, 0);
  env->ReleaseDoubleArrayElements(jyy, yy, 0);
  env->ReleaseDoubleArrayElements(jex, ex, 0);
  env->ReleaseDoubleArrayElements(jey, ey, 0);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
  env->ReleaseStringUTFChars(jname, name);
  env->ReleaseStringUTFChars(jtitle, title);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_createNtuple (JNIEnv *env, jobject thisObj, jstring jid, jstring jfname, jstring jpath,
     jstring jname, jstring jtitle, jstring jvarlist) {
  const char *id = env->GetStringUTFChars(jid, NULL);
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  const char *name = env->GetStringUTFChars(jname, NULL);
  const char *title = env->GetStringUTFChars(jtitle, NULL);
  const char *varlist = env->GetStringUTFChars(jvarlist, NULL);

  TFile* ff = (TFile*) objects.get(fname);
  ff->cd(path);
  TNtuple* tpl = new TNtuple(name, title, varlist);
  objects.set(id, tpl);

  env->ReleaseStringUTFChars(jid, id);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
  env->ReleaseStringUTFChars(jname, name);
  env->ReleaseStringUTFChars(jtitle, title);
  env->ReleaseStringUTFChars(jvarlist, varlist);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_writeNtuple (JNIEnv *env, jobject thisObj, jstring jid, jstring jfname, jstring jpath) {
  const char *id = env->GetStringUTFChars(jid, NULL);
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);

  TFile* ff = (TFile*) objects.get(fname);
  if(!ff->Get(path))
    ff->mkdir(path);
  ff->cd(path);
  TNtuple* tpl = (TNtuple*) objects.get(id);
  tpl->Write();

  env->ReleaseStringUTFChars(jid, id);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_fillNtuple (JNIEnv *env, jobject thisObj, jstring jid, jint nvars, jfloatArray jarr) {
  const char *id = env->GetStringUTFChars(jid, NULL);
  float *arr = env->GetFloatArrayElements(jarr, NULL);

  TNtuple* tpl = (TNtuple*) objects.get(id);
  jsize length = env->GetArrayLength(jarr);
  for(int i=0; i<length; i+=nvars)
    tpl->Fill(&arr[i]);

  env->ReleaseFloatArrayElements(jarr, arr, 0);
  env->ReleaseStringUTFChars(jid, id);
}


