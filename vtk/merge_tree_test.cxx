/*
 * merge_tree_test.cxx
 *
 *  Created on: Jan 8, 2016
 *      Author: bremer5
 */

#include "vtkSmartPointer.h"
#include <vtkXMLPolyDataWriter.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include <vtkFloatArray.h>
#include <vtkPointData.H>
#include "vtkMutableDirectedGraph.h"

#include "vtkMergeTreeGenerator.h"
#include "vtkMergeTree.h"

int main(int argc, const char* argv[])
{

  if (argc < 2) {
    fprintf(stderr,"Usage: %s <surface.obj>", argv[0]);
    return 0;
  }

  int length = strlen(argv[1]);
  const char* ext = argv[1] + length - 3;

  vtkSmartPointer<vtkPolyData> surface;

  if (strcmp(ext,"obj") == 0) {
    vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    surface = reader->GetOutput();
  }
  else if (strcmp(ext,"stl") == 0) {
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    surface = reader->GetOutput();
  }
  else {
    fprintf(stderr,"Unkown file format %s\n",ext);
    return 0;
  }

  vtkSmartPointer<vtkMergeTreeGenerator> mt = vtkSmartPointer<vtkMergeTreeGenerator>::New();
  vtkSmartPointer<vtkMergeTree> tree;

  vtkSmartPointer<vtkFloatArray> func = vtkSmartPointer<vtkFloatArray>::New();
  func->SetNumberOfComponents(1);
  func->SetName("Function");
  func->SetNumberOfTuples(surface->GetNumberOfPoints());


  for (int i=0;i<surface->GetNumberOfPoints();i++) {
    func->SetTuple1(i,surface->GetPoint(i)[2]);
  }

  surface->GetPointData()->SetScalars(func);

  mt->SetInputDataObject(surface);
  mt->Update();


  vtkSmartPointer<vtkPointSet> transformation;
  transformation = vtkPointSet::SafeDownCast(mt->GetOutput(0));
  tree = mt->GetTree();

  surface->GetPointData()->AddArray(transformation->GetPointData()->GetScalars());

  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("seg.vtp");
  writer->SetInputData(surface);
  writer->Write();

  return 1;
}





