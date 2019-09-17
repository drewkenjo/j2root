#include <iostream>
#include <map>
#include <jni.h>
#include <stdio.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMap.h>
#include <TGraphErrors.h>
#include <TObjString.h>
#include "org_jlab_jroot_JRootJNI.h"

TMap* objects = new TMap();

static __attribute__((constructor)) void init() {
  gSystem->ResetSignals();
}

JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_createFile (JNIEnv *env, jobject thisObj, jstring jfname) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  TFile* ff = new TFile(fname, "RECREATE");
  TObjString* objname = new TObjString(fname);
  objects->Add(objname, ff);
  env->ReleaseStringUTFChars(jfname, fname);
}

JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_closeFile (JNIEnv *env, jobject thisObj, jstring jfname) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  TObjString* objname = new TObjString(fname);
  ((TFile*) objects->GetValue(objname))->Close();
  objects->DeleteEntry(objname);
  env->ReleaseStringUTFChars(jfname, fname);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_mkdir( JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath) {
  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  TObjString* objname = new TObjString(fname);
  ((TFile*) objects->GetValue(objname))->mkdir(path);
  env->ReleaseStringUTFChars(jfname, fname);
  env->ReleaseStringUTFChars(jpath, path);
}


JNIEXPORT void JNICALL Java_org_jlab_jroot_JRootJNI_writeH1F (JNIEnv *env, jobject thisObj, jstring jfname, jstring jpath,
	jstring jname, jstring jtitle, jint nbins, jdouble xmin, jdouble xmax, jfloatArray jdata) {

  const char *fname = env->GetStringUTFChars(jfname, NULL);
  const char *path = env->GetStringUTFChars(jpath, NULL);
  const char *name = env->GetStringUTFChars(jname, NULL);
  const char *title = env->GetStringUTFChars(jtitle, NULL);

  TObjString* objname = new TObjString(fname);
  float *cdata = env->GetFloatArrayElements(jdata, NULL);

  TFile* ff = (TFile*) objects->GetValue(objname);
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

  TObjString* objname = new TObjString(fname);
  float *cdata = env->GetFloatArrayElements(jdata, NULL);

  TFile* ff = (TFile*) objects->GetValue(objname);
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

  TObjString* objname = new TObjString(fname);
  TFile* ff = (TFile*) objects->GetValue(objname);
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


