#!/usr/bin/env run-groovy

import org.jlab.jroot.ROOTFile
import org.jlab.groot.data.H1F
import org.jlab.groot.data.H2F
import org.jlab.groot.data.GraphErrors

def h1 = new H1F("h1","title h1",100,-3,3)
def h2 = new H2F("h2","title h2",100,-3,3,100,0,10)
def gr = new GraphErrors("gr")
def h3 = new H2F("h3/test/h2","title h2",100,-3,3,100,0,10)
gr.addPoint(20,30,3,4)
gr.addPoint(30,50,3,4)
gr.addPoint(40,60,3,4)
h1.fill(1,100)

def ff = new ROOTFile('test0.root')

ff.mkdir('test')
ff.cd('test') //optional, mkdir in line above does creates and 'cd' to newly created directory
ff.writeDataSet(h1)
ff.writeDataSet(h3)
ff.mkdir('abc')
ff.cd('abc') //optional, mkdir in line above does creates and 'cd' to newly created directory
ff.writeDataSet(h2)
ff.writeDataSet(gr)
ff.close()
