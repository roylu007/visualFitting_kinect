#ifndef CGALTRI_H
#define CGALTRI_H

#include <QtWidgets/QApplication>
#include <iostream>
#include <Windows.h>
#include <opencv2/imgproc/imgproc_c.h>  
#include <opencv2/legacy/legacy.hpp>  
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2/nonfree/nonfree.hpp>    
#include <opencv2/nonfree/features2d.hpp>   
#include <opencv2\opencv.hpp>
#include <boost/format.hpp> 
#include <QtGui>  
#include <CGAL/Qt/GraphicsViewNavigation.h> 
#include <QLineF>
#include <QRectF>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <list>
#include "featurePoint.h"
#include "Model.h"
#include "Garment.h"

using namespace std;
using namespace cv;
//#define MINLEN 40

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Point CdtPoint;
typedef CDT::Face_handle Face_handle;
typedef CDT::Edge Edge;

const cv::Scalar colorIn(0, 255, 0);
const cv::Scalar colorOut(0, 0, 255);

class CGALTri{
public:

	static void InsertPolygonInCDT(CDT& cdt, vector<CdtPoint> polygon)
	{
		int s = polygon.size();
		for (int i = 0; i < s; i++)
		{
			Vertex_handle v_pt = cdt.insert(polygon[i]);
			Vertex_handle v_pt2 = cdt.insert(polygon[(i + 1) % s]);//依次插入两个相邻的关键点  

			cdt.insert_constraint(v_pt, v_pt2);//将相邻点作为一个边约束插入CDT  
		}
	}

	static void DrawEdge(cv::Mat& img, Edge e, cv::Scalar color)
	{
		Face_handle fh = e.first;
		int e_id = e.second;

		Vertex_handle vh1 = fh->vertex(fh->ccw(e_id));
		Vertex_handle vh2 = fh->vertex(fh->cw(e_id));

		CdtPoint p1 = vh1->point();
		CdtPoint p2 = vh2->point();

		Point sp1 = Point(p1.x(), p1.y());
		Point sp2 = Point(p2.x(), p2.y());

		line(img, sp1, sp2, color);
		//DrawEdge(img, vh1, vh2, color);
	}

	static void DrawEdge(Mat& img, Vertex_handle v1, Vertex_handle v2, cv::Scalar color){

	}

	static void DrawEdgesInCDT(cv::Mat& img, Scalar color, CDT& cdt)
	{
		/*CDT::Finite_edges_iterator eit;
		for (eit = cdt.finite_edges_begin(); eit != cdt.finite_edges_end(); eit++)
		{
		DrawEdge(img, *eit, color);
		}*/

		for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
			fit != cdt.finite_faces_end(); ++fit)
		{
			for (int i = 0; i < 3; i++)
			{
				CdtPoint p1 = fit->vertex(i)->point();
				//cout << "(" << p.x() << "," << p.y() << ")" << endl;
				CdtPoint p2 = fit->vertex((i + 1) % 3)->point();

				Point sp1 = Point(p1.x(), p1.y());
				Point sp2 = Point(p2.x(), p2.y());
				if (fit->is_in_domain())
					line(img, sp1, sp2, colorIn);
				//else
				//line(img, sp1, sp2, colorOut);
			}
		}
	}

	static int getmain(int MINLEN, cv::Mat img, vector<cv::Point>pts, char* imgname)
	{
		
		Mat imgL = img.clone();
		CDT cdt;
		int lens = pts.size();
		cout << "lens=" << lens << endl;
		vector<CdtPoint>garmentPolygon;
		for (int i = 0; i < lens; i++)
		{
			cv::Point p = pts[i];
			garmentPolygon.push_back(CdtPoint(p.x, p.y));
		}


		InsertPolygonInCDT(cdt, garmentPolygon);
		std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
		std::cout << "Meshing the domain..." << std::endl;
		CGAL::refine_Delaunay_mesh_2(cdt, Criteria(0.125, MINLEN));

		DrawEdgesInCDT(imgL, colorIn, cdt);

		/*Vertex_handle va = cdt.insert(Point(2, 0));
		Vertex_handle vb = cdt.insert(Point(0, 2));
		Vertex_handle vc = cdt.insert(Point(-2, 0));
		Vertex_handle vd = cdt.insert(Point(0, -2));
		cdt.insert_constraint(va, vb);
		cdt.insert_constraint(vb, vc);
		cdt.insert_constraint(vc, vd);
		cdt.insert_constraint(vd, va);
		va = cdt.insert(Point(3, 3));
		vb = cdt.insert(Point(-3, 3));
		vc = cdt.insert(Point(-3, -3));
		vd = cdt.insert(Point(3, 0 - 3));
		cdt.insert_constraint(va, vb);
		cdt.insert_constraint(vb, vc);
		cdt.insert_constraint(vc, vd);
		cdt.insert_constraint(vd, va);
		std::list<Point> list_of_seeds;
		list_of_seeds.push_back(Point(0, 0));
		std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
		std::cout << "Meshing the domain..." << std::endl;
		CGAL::refine_Delaunay_mesh_2(cdt, list_of_seeds.begin(), list_of_seeds.end(),
		Criteria());*/
		std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
		std::cout << "Number of finite faces: " << cdt.number_of_faces() << std::endl;
		int mesh_faces_counter = 0;
		for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
			fit != cdt.finite_faces_end(); ++fit)
		{
			if (fit->is_in_domain()) ++mesh_faces_counter;
		}
		cout << "Number of faces in the mesh domain: " << mesh_faces_counter << endl;

		namedWindow(imgname);
		imshow(imgname, imgL);
		//waitKey(0);
		//system("pause");
		return 0;
	}

};


#endif